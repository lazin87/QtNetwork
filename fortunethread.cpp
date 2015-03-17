#include "fortunethread.h"

#include<QTcpSocket>
FortuneThread::FortuneThread(QObject *parent)
    : QThread(parent)
    , quit(false)
{

}

FortuneThread::~FortuneThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
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
        const int timeout = 5 * 1000;

        QTcpSocket socket;
        socket.connectToHost(serverName, serverPort);

        if(false == socket.waitForConnected(timeout) )
        {
            emit error(socket.error(), socket.errorString() );
            return;
        }

        while(socket.bytesAvailable() < static_cast<int>(sizeof(quint16) ) )
        {
            if(!socket.waitForReadyRead(timeout) )
            {
                emit error(socket.error(), socket.errorString() );
                return;
            }
        }

        quint16 blockSize;
        QDataStream in(&socket);
        in.setVersion(QDataStream::Qt_4_0);
        in >> blockSize;

        while(socket.bytesAvailable() < blockSize)
        {
            if(!socket.waitForReadyRead(timeout) )
            {
                emit error(socket.error(), socket.errorString() );
                return;
            }
        }

        mutex.lock();
        QString fortune;
        in >> fortune;
        emit newFortune(fortune);

        cond.wait(&mutex);
        serverName = hostName;
        serverPort = port;
        mutex.unlock();
    }
}

