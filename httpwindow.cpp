#include "httpwindow.h"
#include "ui_authenticationdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QAuthenticator>

HttpWindow::HttpWindow(QWidget *parent)
    : QDialog(parent)
    , httpGetId(0)
    , httpRequestAborted(false)
{
#ifndef QT_NO_SSL
    urlLineEdit = new QLineEdit("https://qt-project.org/");
#else
    urlLineEdit = new QLineEdit("http://qt-project.org/");
#endif

    urlLabel = new QLabel(tr("&URL") );
    urlLabel->setBuddy(urlLineEdit);
    statusLabel = new QLabel(tr("Please enter the URL of a file you want to "
                                "download.") );
    statusLabel->setWordWrap(true);

    downloadButton = new QPushButton(tr("Download") );
    downloadButton->setDefault(true);
    quitButton = new QPushButton(tr("Quit") );
    quitButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    progressDialog = new QProgressDialog(this);

    connect( urlLineEdit, SIGNAL(textChanged(QString) )
           , this, SLOT(enableDownloadButton() )
           );
    connect( &qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*))
           , this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*))
           );
#ifndef QT_NO_SSL
    connect( &qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>) )
           , this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>) )
           );
#endif
    connect( progressDialog, SIGNAL(canceled() )
           , this, SLOT(cancelDownload() )
           );
    connect( downloadButton, SIGNAL(clicked() )
           , this, SLOT(downloadFile() )
           );
    connect( quitButton, SIGNAL(clicked() )
           , this, SLOT(close() )
           );
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(urlLabel);
    topLayout->addWidget(urlLineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("HTTP") );
    urlLineEdit->setFocus();
}

HttpWindow::~HttpWindow()
{

}

void HttpWindow::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url) );

    connect( reply, SIGNAL(finished() )
           , this, SLOT(httpFinished() )
           );
    connect( reply, SIGNAL(readRead() )
           , this, SLOT(httpReadyRead() )
           );
    connect( reply, SIGNAL(downloadProgress(qint64,qint64) )
           , this, SLOT(updateDataReadProgress(qint64,qint64) )
           );
}

void HttpWindow::downloadFile()
{
    url = urlLineEdit->text();

    QFileInfo fileInfo(url.path() );
    QString fileName = fileInfo.fileName();

    if(fileName.isEmpty() )
    {
        fileName = "index.html";
    }

    if(QFile::exists(fileName) )
    {
        int iResult = QMessageBox::question( this
                                            , tr("HTTP")
                                            , tr("There already exists a file called %1 in "
                                                 "the current directory. Overwrite?").arg(fileName)
                                            , QMessageBox::Yes | QMessageBox::No
                                            , QMessageBox::No
                                            );
        if(QMessageBox::No == iResult)
        {
            return;
        }

        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if(!file->open(QIODevice::WriteOnly) )
    {
        QMessageBox::information( this
                                , tr("HTTP")
                                , tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString() )
                                );
        delete file;
        file = 0;
        return;
    }

    progressDialog->setWindowTitle(tr("HTTP") );
    progressDialog->setLabelText(tr("Downloading %1").arg(fileName) );
    downloadButton->setEnabled(false);

    //schedule the request
    httpRequestAborted = false;
    startRequest(url);
}

void HttpWindow::cancelDownload()
{
    statusLabel->setText(tr("Download canceled.") );
    httpRequestAborted = true;
    reply->abort();
    downloadButton->setEnabled(true);
}

void HttpWindow::httpFinished()
{
    if(httpRequestAborted)
    {
        if(file)
        {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }

        reply->deleteLater();
        progressDialog->hide();

        return;
    }

    progressDialog->hide();
    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if(reply->error() )
    {
        file->remove();
        QMessageBox::information( this
                                , tr("HTTP")
                                , tr("Download faild: %1.").arg(reply->errorString() )
                                );
        downloadButton->setEnabled(true);
    }
    else if(!redirectionTarget.isNull() )
    {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl() );
        int iResult = QMessageBox::question( this
                                           , tr("HTTP")
                                           , tr("Redirect to %1 ?").arg(newUrl.toString() )
                                           , QMessageBox::Yes | QMessageBox::No
                                           );

        if(QMessageBox::Yes == iResult)
        {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    }
    else
    {
        QString fileName = QFileInfo(QUrl(urlLineEdit->text() ).path() ).fileName();
        statusLabel->setText(tr("Downloaded %1 to %2.").arg(fileName).arg(QDir::currentPath() ) );
        downloadButton->setEnabled(true);
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}

void HttpWindow::httpReadyRead()
{
    if(file)
    {
        file->write(reply->readAll() );
    }
}

void HttpWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if(httpRequestAborted)
    {
        return;
    }

    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void HttpWindow::enableDownloadButton()
{
    downloadButton->setEnabled(!urlLineEdit->text().isEmpty() );
}

void HttpWindow::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    QDialog dlg;
    Ui::Dialog ui;

    ui.setupUi(&dlg);
    dlg.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm() ).arg(url.host() ) );

    ui.userEdit->setText(url.userName() );
    ui.passwordEdit->setText(url.password() );

    if(QDialog::Accepted == dlg.exec() )
    {
        authenticator->setUser(ui.userEdit->text() );
        authenticator->setPassword(ui.passwordEdit->text() );
    }
}

#ifndef QT_NO_SSL
void HttpWindow::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    QString errorString;
    foreach(const QSslError &error, errors)
    {
        if(!errorString.isEmpty() )
        {
            errorString += ", ";
        }
        errorString += error.errorString();
    }

    int iResult = QMessageBox::warning( this
                                      , tr("HTTP")
                                      , tr("One or more SSL errors has occured: %1").arg(errorString)
                                      , QMessageBox::Ignore | QMessageBox::Abort
                                      );
    if(QMessageBox::Ignore == iResult)
    {
        reply->ignoreSslErrors();
    }
}
#endif

