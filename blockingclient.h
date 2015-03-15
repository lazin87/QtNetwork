#ifndef BLOCKINGCLIENT_H
#define BLOCKINGCLIENT_H

#include <QWidget>

class Blockingclient : public QWidget
{
    Q_OBJECT

public:
    Blockingclient(QWidget *parent = 0);
    ~Blockingclient();
};

#endif // BLOCKINGCLIENT_H
