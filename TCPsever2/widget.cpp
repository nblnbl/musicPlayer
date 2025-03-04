#include "widget.h"
#include "ui_widget.h"
#include "mythread.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    server = new QTcpServer;

    server->listen(QHostAddress::AnyIPv4, 8000);
    qDebug() << "myThread is running in thread:" << QThread::currentThreadId();

    //客户端发起连接，server发出信号
    connect(server, &QTcpServer::newConnection, this, &Widget::newClientHeader);
}

Widget::~Widget()
{
    delete ui;
    delete server;
}
void Widget::newClientHeader()
{
//    QTcpSocket *socket = server->nextPendingConnection();
    //获取客户端的端口和地址
//    socket->peerAddress();
//    socket->peerPort();

//    ui->ipLineEdit->setText(socket->peerAddress().toString());
//    ui->portLineEdit->setText(QString::number(socket->peerPort()));

    //接受客户端消息, socket发出readyRead信号
//    connect(socket, &QTcpSocket::readyRead, this, &Widget::newClientInformtion);


//    启动线程

    myThread *t = new myThread(server);
    t->moveToThread(t);//修改依赖
//    qDebug() << "Socket is running in thread:" << socket->thread();
//    socket->setParent(nullptr);
//    qDebug() << "Socket is running in thread:" << socket->thread();
    t->start();


    //接受自定义信号(同时将自定义信号中的ba传给槽函数)
    connect(t, &myThread::sendToWidget, this, &Widget::threadSlotSend);
}

void Widget::newClientInformtion()
{
    //获取信号的发出者
    QTcpSocket *socket = (QTcpSocket *)sender();
    ui->lineEdit->setText(QString::fromUtf8(socket->readAll()));
}

void Widget::threadSlotSend(QByteArray ba)
{
    ui->lineEdit->setText(QString::fromUtf8(ba));
}

