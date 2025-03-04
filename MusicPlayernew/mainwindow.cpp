#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicsearch.h"
#include "qqmusicsearch.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);//去掉标题
    ui->soundSlider->hide();//隐藏音量条
    ui->searchLineEdit->setPlaceholderText("歌曲\\歌手\\用户");
    tableViewRowCount = 0;

    //初始化播放列表窗口
    listWidget = new musicListwidget(this);
    listWidget->getMainWindowPtr(this);
    listWidget->move(this->pos()+ QPoint(640, 210));

    listWidget->close();
    connect(listWidget, &musicListwidget::sendToMainWindow, this, [this](int){
        player->stop();
        this->playlist->clear();
//        playUrlList.clear();
//        this->tableViewRowCount = 0;
        playListCount = 0;
    });

    //设置搜索表格
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"歌名", "歌手", "操作"});
    delegate = new ButtonDelegate(ui->searchTableView);// 设置自定义代理
    ui->searchTableView->setItemDelegateForColumn(2, delegate);
    ui->searchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选中时为整行选中
    ui->searchTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格的单元为只读属性，即不能编辑
    ui->searchTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // 初始化播放器
    currentMode = QMediaPlaylist::Loop;
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(currentMode);//设置循环播放
    playListCount = 0;
    index = 0;
    connect(ui->soundSlider, &QSlider::valueChanged, this, &MainWindow::on_soundSlider_valueChanged);//音量监控
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::setDuration);//获取当前歌曲总时长
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSliderPosition);//获取当前歌曲进度
    connect(ui->musicSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);//实现拖动进度条改变歌曲进度
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {//监控表格播放信息
//        qDebug()<< index <<QUrl(playUrlList[index]);
        handleMediaStatusChanged(status, this->index);
    });
    connect(player, &QMediaPlayer::currentMediaChanged, this, [this](){


        emit sendPlaylistCurrentIndexToWidget(this->playlist->currentIndex());
    });
}





MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    //绘制主窗口
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/images/background.png"));

//    QPainter painter2(listWidget);
//    painter2.drawPixmap(0,0,width(),height(),QPixmap(":/images/background.png"));


}

void MainWindow::chageMediaToWidget(int index)
{
    playlist->setCurrentIndex(index);
    player->play();
}

void MainWindow::deleteMediaToWidget(int index)
{
//    player->stop();
    int currentIndex = playlist->currentIndex();  // 获取当前播放的索引
    bool wasPlaying = player->state() == QMediaPlayer::PlayingState;  // 检查是否正在播放
    qint64 currentPosition = player->position();//获取当前播放时间
    player->pause();
    if (index < currentIndex)// 如果移除的索引小于当前播放索引，需要调整当前索引
    {
        playlist->setCurrentIndex(currentIndex - 1);  // 将当前索引向前移动一位
    }
    else if(index == currentIndex)
    {

        int nextIndex = currentIndex + 1;
        if (nextIndex >= playlist->mediaCount()) {
            nextIndex = 0;  // 回到第一曲
            playlist->setCurrentIndex(nextIndex);
        }
        else
        {
           playlist->setCurrentIndex(nextIndex);
        }

    }
    playlist->removeMedia(index);
    playListCount -= 1;
    tableViewRowCount -= 1;
    if (wasPlaying && playlist->mediaCount() > 0 && playlist->currentIndex() != -1&&index < currentIndex)// 恢复播放状态
    {
        player->setPosition(currentPosition);
        player->play();
    }
    //    player->play();
}

int MainWindow::getPlayerCurrentIndex()
{
    return playlist->currentIndex();
}

void MainWindow::setSearchTable(QVector<int> &musicIdList,QStringList &musicNameList,QStringList &singerNameList,QStandardItemModel *model)
{

       // 填充数据
//    qDebug() << musicNameList[0];
//    model->setItem(0, 0, new QStandardItem(musicNameList[0]));
    //往表格中添加按钮控件
    for (int i = 0;i < musicIdList.size() ; ++this->tableViewRowCount,++i) {
        qDebug()<<"tableView"<<this->tableViewRowCount<<i<<musicNameList[i];
        model->setItem(i, 0, new QStandardItem(musicNameList[i]));
        model->setItem(i, 1, new QStandardItem(singerNameList[i]));
        model->setItem(i, 2, new QStandardItem(""));
    }

}



//处理搜索点击
void MainWindow::on_searchBtn_clicked()
{
    //清空搜索表
    this->tableViewRowCount = 0;
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->searchTableView->model());
    if (model) {
            // 清空模型
            model->clear();
            musicIdList.clear();       // 存储歌曲 ID
            musicNameList.clear();
            singerNameList.clear();
            playUrlList.clear();
        } else {
            qDebug() << "Failed to cast model to QStandardItemModel.";
    }
    errorMusic.clear();
    connect(player, &QMediaPlayer::stateChanged, this,[this](QMediaPlayer::State status) {
        handleStateChanged(status);
    });//监听音乐是否播放

    QString searchIfo = ui->searchLineEdit->text(); // 获取用户在搜索框中输入的关键词
    if (searchIfo.isEmpty()) { // 检查关键词是否为空
        return;
    }
    else
    {   //与查询表格中的自定义委托按钮建立连接
        connect(delegate, &ButtonDelegate::sendPlayInfo, this, &MainWindow::dealPlaySlot);
        connect(delegate, &ButtonDelegate::sendAddInfo, this, &MainWindow::dealAddSlot);


        //创建搜索线程（酷狗音乐）
        qqMusicSearch *searcher_kugou = new qqMusicSearch(searchIfo);
        searcher_kugou->moveToThread(searcher_kugou);
        connect(searcher_kugou, &qqMusicSearch::sendToWidget, this, &MainWindow::seacherThreadSlotSend);
        searcher_kugou->start();

        //创建搜索线程（网易云163）
        MusicSearch *searcher_163 = new MusicSearch(searchIfo);
        searcher_163->moveToThread(searcher_163);
        connect(searcher_163, &MusicSearch::sendToWidget, this, &MainWindow::seacherThreadSlotSend);
        searcher_163->start();



    }
}
void MainWindow::handleStateChanged(QMediaPlayer::State state)
{
    switch (state) {

        case QMediaPlayer::PlayingState:
//            qDebug() << "Media is now playing";
            ui->openBtn->setIcon(QIcon(":/images/stop.png"));  // 替换为你的图标路径
//            ui->openBtn->setIconSize(QSize(32, 32));
            break;
        case QMediaPlayer::PausedState:
//            qDebug() << "Media is paused";
            ui->openBtn->setIcon(QIcon(":/images/begin.png"));
            break;
        case QMediaPlayer::StoppedState:
//            qDebug() << "Media is stopped";
            ui->openBtn->setIcon(QIcon(":/images/begin.png"));
            break;
        default:
            break;
    }
}
void MainWindow::seacherThreadSlotSend(std::tuple<QVector<int>, QStringList, QStringList, QStringList> info)
{

    QVector<int> musicIdList1;       // 存储歌曲 ID
    QStringList musicNameList1;
    QStringList singerNameList1;
    QStringList musicPlayList;
    std::tie(musicIdList1, musicNameList1, singerNameList1, musicPlayList) =  info;

    for(int i=0; i < musicPlayList.size(); ++i)
    {
        if (!playUrlList.contains(musicPlayList[i]))
        {
            playUrlList.append(musicPlayList[i]);
            musicIdList.append(musicIdList1[i]);
            singerNameList.append(singerNameList1[i]);
            musicNameList.append(musicNameList1[i]);
            qDebug()<<playUrlList.size()<<musicIdList.size()<< musicNameList1[i]<<"44444444444444444444444444444444444444444";
        }

    }
//    qDebug()<<playUrlList.size()<<musicIdList.size()<< <<"44444444444444444444444444444444444444444";
    if(musicIdList.size() == 0)
    {
        QMessageBox::warning(this, "错误", "搜索不到相关音乐");
        return;
    }

    setSearchTable(musicIdList, musicNameList, singerNameList, model);
//    setSearchTable(musicIdList, musicNameList, singerNameList, model);
    ui->searchTableView->setModel(model);
}
void MainWindow::setDuration(qint64 duration)
{
    ui->musicSlider->setRange(0, duration);
    updateTimeLabel(player->position(), duration);
}

void MainWindow::updateSliderPosition(qint64 position)
{
        if (!ui->musicSlider->isSliderDown())
        {
            ui->musicSlider->setValue(position);
        }
        updateTimeLabel(position, ui->musicSlider->maximum());
}

void MainWindow::setPosition(int position)
{
    player->setPosition(position);
}
void MainWindow::updateTimeLabel(qint64 position, qint64 duration)
{
    QTime positionTime(0, (position / 60000) % 60, (position / 1000) % 60);
    QTime durationTime(0, (duration / 60000) % 60, (duration / 1000) % 60);
    ui->musicTimeLabel->setText(positionTime.toString("mm:ss") + " / " + durationTime.toString("mm:ss"));
}

void MainWindow::on_playModeBtn_clicked()
{
    switch (currentMode) {
        case QMediaPlaylist::Loop:
            currentMode = QMediaPlaylist::Sequential;
            ui->playModeBtn->setIcon(QIcon(":/images/play.png"));

            break;
        case QMediaPlaylist::Sequential:
            currentMode = QMediaPlaylist::CurrentItemInLoop;
            ui->playModeBtn->setIcon(QIcon(":/images/onefor.png"));
            break;

        case QMediaPlaylist::CurrentItemInLoop:
            currentMode = QMediaPlaylist::Random;
            ui->playModeBtn->setIcon(QIcon(":/images/randam.png"));
            break;
        case QMediaPlaylist::Random:
            currentMode = QMediaPlaylist::Loop;
            ui->playModeBtn->setIcon(QIcon(":/images/while.png"));
            break;
        default:
            QMessageBox::warning(this, "错误", "未知的播放模式");
            return;
     }

    playlist->setPlaybackMode(currentMode);
}
void MainWindow::on_soundSlider_valueChanged(int value)
{
    // 更新音量
    player->setVolume(value);
}

void MainWindow::on_nextSongBtn_clicked()
{
    //下一曲
    int currentIndex = playlist->currentIndex();
    qDebug()<<currentIndex;
    int nextIndex = currentIndex + 1;
    if (nextIndex >= playlist->mediaCount()) {
        nextIndex = 0;  // 回到第一曲
    }

    playlist->setCurrentIndex(nextIndex);
    player->play();
}

void MainWindow::on_lastSongBtn_clicked()
{
    //上一曲
   int currentIndex = playlist->currentIndex();
   int previousIndex = currentIndex - 1;

   if (previousIndex < 0) {
       previousIndex = playlist->mediaCount() - 1;  // 回到最后一曲
   }

   playlist->setCurrentIndex(previousIndex);
   player->play();
}



//实现窗口拖动和音量进度条的显示和控制
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;  // 开始拖动
        dragPosition = event->globalPos() - this->pos();  // 记录鼠标位置


        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging) {
        // 更新窗口位置
        this->move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;  // 停止拖动
        event->accept();
    }
}



void MainWindow::on_soundBtn_clicked()
{
    // 判断 soundSlider 当前是否可见
    if (ui->soundSlider->isVisible()) {
        // 如果当前可见，则隐藏
        ui->soundSlider->hide();
    } else {
        // 如果当前隐藏，则显示
        ui->soundSlider->show();
    }
}

void MainWindow::on_closeWindowBtn_clicked()
{
    QApplication::quit();
}



void MainWindow::on_musicListBtn_clicked()
{


    // 设置窗口标志为无边框
    listWidget->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
//    listWidget->setStyleSheet("QWidget { background-image: url(:/images/background.png);}");
    listWidget->setStyleSheet(
        "QWidget { "
        "background-image: url(:/images/background.png); "

        "}"
    );

    if (listWidget->isVisible()) {
        // 如果当前可见，则隐藏
        listWidget->hide();
    } else {
        // 如果当前隐藏，则显示
        listWidget->show();
    }
//    listWidget->move(this->pos()+ QPoint(183, 100));// 设置子窗口在主窗口的右下角
//    listWidget->show();
}
