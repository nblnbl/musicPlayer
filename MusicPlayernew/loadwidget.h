#ifndef LOADWIDGET_H
#define LOADWIDGET_H

#include <QWidget>
#include <QPainter>

#include <QMouseEvent>  // 需要包含鼠标事件相关的头文件

#include <QMessageBox>

#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class loadWidget;
}

class loadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit loadWidget(QWidget *parent = nullptr);
    ~loadWidget();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    bool isDragging = false;  // 是否正在拖动
    QPoint dragPosition;      // 鼠标拖动的起始位置

private slots:
    void on_closeLoadBtn_clicked();

    void on_loginBtn_clicked();

    void onConnected();

    void onDisconnected();


private:
    Ui::loadWidget *ui;
    QTcpSocket *socket;
    QString IP;
    QString port;
};

#endif // LOADWIDGET_H
