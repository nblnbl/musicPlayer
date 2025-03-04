#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QPainter>
#include <QMouseEvent>  // 需要包含鼠标事件相关的头文件
#include <QTableView>
#include <thread>

#include <ButtonDelegate.h>
#include <QStandardItemModel>

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QMessageBox>
#include <QSet>

#include "musiclistwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);

    void chageMediaToWidget(int index);//对列表中的播放按钮操作

    void deleteMediaToWidget(int index);//对列表中的删除按钮操作

    int getPlayerCurrentIndex();//获取当前播放歌曲在播放列表中的索引

signals:
    void sendPlaylistCurrentIndexToWidget(int);

protected:
    bool isDragging = false;  // 是否正在拖动
    QPoint dragPosition;      // 鼠标拖动的起始位置
    QPoint dragPosition1;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);


public slots:

protected slots:
    void setSearchTable(QVector<int> &musicIdList,QStringList &musicNameList,QStringList &singerNameList,QStandardItemModel *model);//绘画搜索表格

    void seacherThreadSlotSend(std::tuple<QVector<int>, QStringList, QStringList, QStringList>);//用于处理获取的歌曲信息

    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status, int index);//用于判断歌曲准备情况

    void handleStateChanged(QMediaPlayer::State state);

    void dealPlaySlot(int index);//处理表格播放

    void dealAddSlot(int index);//处理表格播放

    void on_soundBtn_clicked();

    void on_searchBtn_clicked();

    void setDuration(qint64 duration);//获取当前歌曲时长

    void updateTimeLabel(qint64 position, qint64 duration);//将歌曲时间更新到标签

    void updateSliderPosition(qint64 position);//更新滑动条的位置

    void setPosition(int position);//寻找当前歌曲位置

private slots:
    void on_openBtn_clicked();

    void on_closeWindowBtn_clicked();

    void on_soundSlider_valueChanged(int value);

    void on_nextSongBtn_clicked();

    void on_lastSongBtn_clicked();

    void on_musicListBtn_clicked();

    void on_playModeBtn_clicked();


private:
    Ui::MainWindow *ui;
    musicListwidget *listWidget;//创建歌曲列表窗口

    //下面3个参数用于多线程（多信号源）获取资源
    int tableViewRowCount;
    QStandardItemModel *model;
    ButtonDelegate* delegate;


    int playListCount;  //当前列表中歌曲的数目
    int index;//当前歌曲索引
    QSet<QUrl> errorMusic;
    QMediaPlayer* player;          // 音频播放器
    QMediaPlaylist* playlist;      // 播放列表
    QMediaPlaylist::PlaybackMode currentMode;//当前音乐播放模式
    QStringList playUrlList;       //播放链接
    QVector<int> musicIdList;       // 存储歌曲 ID
    QStringList musicNameList;
    QStringList singerNameList;



};
#endif // MAINWINDOW_H
