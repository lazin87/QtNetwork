#include "httpwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QNetworkReply>
#include <QFile>

HttpWindow::HttpWindow(QWidget *parent)
    :QDialog(parent)
{
#ifndef QT_NO_SSL
#else
#endif
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

