#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTcpServer>

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QTcpServer *s);
    void run() override; //线程处理函数

signals:
    void sendToWidget(QByteArray ba);

private slots:
//    void clientInfo();

private:
    QTcpServer *server;

};

#endif // MYTHREAD_H
