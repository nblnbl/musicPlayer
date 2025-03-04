#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QPainter>

#include <ButtonDelegate.h>
//#include <mainwindow.h>
#include <QStandardItemModel>

#include <QMediaPlaylist>



namespace Ui {
class musicListwidget;
}
class MainWindow;
class musicListwidget : public QWidget
{
    Q_OBJECT

public:
    explicit musicListwidget(QWidget *parent = nullptr);
    void getMainWindowPtr(MainWindow *window);
    ~musicListwidget();
    QStandardItemModel *model;

protected slots:
    void dealPlaySlot(int index);

    void dealDeleteSlot(int index);

private slots:
    void on_closeWindowBtn_clicked();

    void on_clearPlayListBtn_clicked();

private:
    Ui::musicListwidget *ui;
    MainWindow *mainWindow;
    QMediaPlaylist *playlist;

    ButtonDelegate* delegate;

signals:
    void sendToMainWindow(int i);
};

#endif // MUSICLISTWIDGET_H
