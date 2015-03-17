#ifndef BLOCKINGCLIENT_H
#define BLOCKINGCLIENT_H

#include <QWidget>
#include "fortunethread.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;

class Blockingclient : public QWidget
{
    Q_OBJECT

public:
    Blockingclient(QWidget *parent = 0);
    ~Blockingclient();

private slots:
    void requestNewFortune();
    void showFortune(const QString &nextFortune);
    void displayError(int socketError, const QString &message);
    void enableGetFortuneButton();

private:
    QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *getFortuneButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    FortuneThread thread;
    QString currentFortune;
};

#endif // BLOCKINGCLIENT_H
