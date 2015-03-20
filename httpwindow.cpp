#include "httpwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QNetworkReply>

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

