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
class QFile;

class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    HttpWindow(QWidget * parent = 0);
    ~HttpWindow();

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void enabeDownloadButton();
    void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *);
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);
#endif

private:
    QLabel *statusLabel;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QDialogButtonBox *buttonBox;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;

};

#endif // HTTPWINDOW_H
