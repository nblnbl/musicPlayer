#include <mainwindow.h>


//实现在搜索表格中点击播放
void MainWindow::dealPlaySlot(int index)
{
    this->index = index;
    bool flag = false;//设定标志因为遇到错误可能会重复执行多次槽函数


    // 播放歌曲并添加到列表

//    qDebug()<<flag;
//    connect(player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
//            this, [this,&flag](){
//       QMessageBox::warning(this, "错误", "资源无效（没ViP）");
//        qDebug()<<flag;

//        if(flag == false)
//        {
//            flag = true;
//            playlist->removeMedia(playListCount-1);
//            playListCount -= 1;
//            qDebug()<<playListCount<<"2222222222222222222222222222222";
//            QMessageBox::warning(this, "错误", "资源无效（没ViP）");
//        }
//    });
//    QMediaPlayer* player_table = new QMediaPlayer(this);

//    qDebug()<<playListCount<<"111111111111111111111111111111111";

    bool isAlreadyAdded = false;
    int i = 0;
    for (; i < playlist->mediaCount(); ++i) {
        if (playlist->media(i).canonicalUrl() == QUrl(playUrlList[index])) {
            isAlreadyAdded = true;
            break;
        }
    }
    if (!isAlreadyAdded)
    {
//        player->stop();
        player->setMedia(QUrl(playUrlList[index]));
        player->play();
    }
    else
    {
//        player->stop();
        player->setPlaylist(playlist);
        playlist->setCurrentIndex(i);
        player->play();
    }



}
//实现在//实现在搜索表格中点击添加播放列表
void MainWindow::dealAddSlot(int index)
{
    qDebug()<< index;
    playlist->addMedia(QUrl(playUrlList[index]));
}
//点击播放整个列表
void MainWindow::on_openBtn_clicked()
{
    if(playListCount == 0)
    {
        QMessageBox::warning(this, "错误", "当前列表无歌曲");
        return;
    }
    else
    {

        if (player->state() == QMediaPlayer::PlayingState)
        {
            // 如果当前正在播放，暂停播放
            player->pause();
        }
        else if(player->state() == QMediaPlayer::PausedState)
        {
            // 如果当前处于暂停状态，恢复播放
            player->play();
        }
        else if(player->state() == QMediaPlayer::StoppedState)
        {
            //如果出于未播状态，直接播放playlist
            player->setPlaylist(playlist);
            player->play();
        }
    }

}

// 槽函数(暂定)
void MainWindow::handleMediaStatusChanged(QMediaPlayer::MediaStatus status, int index)
{
    if(this->index == -1)
    {
        return;
    }
    switch (status) {
        case QMediaPlayer::InvalidMedia:

            errorMusic.insert(QUrl(playUrlList[index]));
            return;  // 提前退出函数

        case QMediaPlayer::BufferedMedia:
//             检查播放列表中是否已存在该 URL
            bool isAlreadyAdded = false;
            int i = 0;
            for (; i < playlist->mediaCount(); ++i) {
                if (playlist->media(i).canonicalUrl() == QUrl(playUrlList[index])) {
                    isAlreadyAdded = true;
                    break;  // 提前退出循环
                }
            }
            if(!errorMusic.contains(QUrl(playUrlList[index])))
            {
                if (!isAlreadyAdded) {
    //              if (!errorMusic.contains(QUrl(playUrlList[index]))) {
                    playlist->addMedia(QUrl(playUrlList[index]));
    //                errorMusic.insert(QUrl(playUrlList[index]));
//                    player->stop();
                    player->setPlaylist(playlist);
                    //将歌曲信息写入播放table中
                    listWidget->model->setItem(playListCount, 0, new QStandardItem(musicNameList[index]));
                    listWidget->model->setItem(playListCount, 1, new QStandardItem(singerNameList[index]));
                    listWidget->model->setItem(playListCount, 2, new QStandardItem(""));
                    listWidget->model->item(playListCount, 0)->setBackground(QColor(255, 200, 200));
                    listWidget->model->item(playListCount, 1)->setBackground(QColor(255, 200, 200));
                    listWidget->model->item(playListCount, 2)->setBackground(QColor(255, 200, 200));
                    qDebug()<<playListCount;
                    playlist->setCurrentIndex(playListCount);
                    player->play();
                    this->playListCount += 1;
                    this->index = -1;
    //                qDebug() << "Added media to playlist:" <<QUrl(playUrlList[index])<<index;
                    return;
                }

            }
            else
            {
//                QMessageBox::warning(this, "错误", "资源无效（没VIP）");
            }


            break;

    }

    qDebug()<<playListCount<<"2222222222222222222222222222222";


}


