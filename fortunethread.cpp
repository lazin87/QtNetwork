#include "fortunethread.h"

#include<QTcpSocket>
FortuneThread::FortuneThread(QObject *parent)
    : QThread(parent)
    , quit(false)
{

}

FortuneThread::~FortuneThread()
{

}

void FortuneThread::requestNewFortune(const QString &a_hostName, quint16 port)
{
    QMutexLocker locker(&mutex);
    this->hostName = a_hostName;
    this->port = port;

    if(false == isRunning() )
    {
        start();
    }
    else
    {
        cond.wakeOne();
    }
}

void FortuneThread::run()
{
    mutex.lock();
    QString serverName = hostName;
    quint16 serverPort = port;
    mutex.unlock();

    while(!quit)
    {
        const int timeout = 5 * 100;

        QTcpSocket socket;
        socket.connectToHost(serverName, serverPort);

        if(false == socket.waitForConnected(timeout) )
        {
            emit error(socket.error(), socket.errorString() );
            return;
        }
    }
}

