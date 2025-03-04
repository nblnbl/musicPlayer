#include "loadwidget.h"
#include "ui_loadwidget.h"

loadWidget::loadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loadWidget)
{
    ui->setupUi(this);

    this->IP = "127.0.0.1";
    this->port = "8000";
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &loadWidget::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &loadWidget::onDisconnected);

    QPainter painter(ui->label1);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/images/audio.png"));
    this->setWindowFlag(Qt::FramelessWindowHint);//去掉标题
}

loadWidget::~loadWidget()
{
    delete ui;
}

void loadWidget::on_closeLoadBtn_clicked()
{
    QApplication::quit();
}

//实现窗口拖动和音量进度条的显示和控制
void loadWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;  // 开始拖动
        dragPosition = event->globalPos() - this->pos();  // 记录鼠标位置


        event->accept();
    }
}

void loadWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        // 更新窗口位置
        this->move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void loadWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;  // 停止拖动
        event->accept();
    }
}

void loadWidget::on_loginBtn_clicked()
{
//    if (!ui->accountEdit->text().isEmpty() && !ui->passwordEdit->text().isEmpty())
//    {

//    }
//    else
//    {
//        QMessageBox::information(this, "警告", "账号或密码为空！");                ;
//    }
    // 获取IP地址和端口号
//    QString IP = ui->ipLineEdit->text();
//    QString port = ui->portLineEdit->text();

    // 连接服务器
    socket->connectToHost(QHostAddress(IP), port.toShort());
    QByteArray ba;
    ba.append(ui->passwordEdit->text());
    socket->write(ba);

}


void loadWidget::onConnected()
{
    QMessageBox::information(this, "连接提示", "连接服务器成功");
    qDebug() << "连接成功";

}

void loadWidget::onDisconnected()
{
    QMessageBox::information(this, "连接提示", "连接服务器断开");
}
