#include "httpwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QNetworkReply>
#include <QFile>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
           , this, SLOT(enabeDownloadButton() )
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

    setWindowTitle(tr("HTTP") );
    urlLineEdit->setFocus();
}

HttpWindow::~HttpWindow()
{

}

void HttpWindow::downloadFile()
{

}

void HttpWindow::cancelDownload()
{

}

void HttpWindow::httpFinished()
{

}

void HttpWindow::httpReadyRead()
{

}

void HttpWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{

}

void HttpWindow::enabeDownloadButton()
{

}

void HttpWindow::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *)
{

}

void HttpWindow::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{

}

