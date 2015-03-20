#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>
#include <QUrl>
#include <QNetworkAccessManager>

class QLabel;
class QLineEdit;
class QProgressDialog;
class QDialogButtonBox;
class QNetworkReply;

class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    HttpWindow(QWidget * parent = 0);
    ~HttpWindow();

private:
    QLabel *statusLabel;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QDialogButtonBox *buttonBox;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

};

#endif // HTTPWINDOW_H
