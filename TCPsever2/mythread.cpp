#include "mythread.h"

myThread::myThread(QTcpServer *s)
{

    server = s;
}



void myThread::run()

{
    // 将 socket 对象移动到子线程中
    QTcpSocket *socket = server->nextPendingConnection();

    qDebug() << "myThread is running in thread:" << QThread::currentThreadId();
    connect(socket, &QTcpSocket::readyRead, this, [this,socket](){
        QByteArray ba = socket->readAll();
        emit sendToWidget(ba);
        qDebug() << "myThread is running in thread:" << QThread::currentThreadId();
    });

    exec();
}

//void myThread::clientInfo()
//{
//    qDebug()<< QString::fromUtf8(socket->readAll());
//    QByteArray ba = socket->readAll();
//    emit sendToWidget(ba);
//    qDebug() << "myThread is running in thread:" << QThread::currentThreadId();
//}
