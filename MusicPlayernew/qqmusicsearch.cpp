#include "qqmusicsearch.h"

qqMusicSearch::qqMusicSearch(QString seachInfo):s(seachInfo)
{
    musicHashList.clear();
    alum_id.clear();
    musicNameList.clear();
    singerNameList.clear();
    musicUrlList.clear(); //存放歌曲的播放链接
    musicIdList.clear();
    int songCount = 0;
}

void qqMusicSearch::run()
{
    network = new QNetworkAccessManager(this);//初始化网络对象

    QString apiUrl = QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json&keyword={%1}").arg(s);//歌曲搜索列表，从中提取歌曲的hash值
//    QString apiUrl = QString("https://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash=64b39a6828913e58e1ed1f7f6061a380");
    QNetworkRequest networkrequest;//定义请求对象
    networkrequest.setUrl(apiUrl);//请求格式设置给请求对象


//    networkrequest->setRawHeader("Cookie","kg_mid=2333");
//    networkrequest->setHeader(QNetworkRequest::CookieHeader, 2333);
    network->get(networkrequest);// 发送 GET 请求
    connect(network, &QNetworkAccessManager::finished, this, &qqMusicSearch::infoBack);


    QMediaPlayer* player = new QMediaPlayer(this);
    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置循环播放
    QString purl =
            "https://sharefs.tx.kugou.com/202503021255/630e6a2e56cd7e1edc28b4f893ea7a78/v3/07bc472acde477702a36bf02306c40f9/yp/full/ap1000_us0_pi409_s1049419750.mp3";
//    player->setMedia(QUrl(purl));
//    player->play();
    exec();
}

void qqMusicSearch::infoBack(QNetworkReply *reply)
{


    if (reply->error() != QNetworkReply::NoError) { // 检查请求是否成功
        qDebug() << "亲求失败";
        return;
    }
    else
    {
       QByteArray data = reply->readAll();
       getMusicHash(data);

       for(int i=0;i<musicHashList.size(); ++i)
       {
           QNetworkAccessManager *music_network = new QNetworkAccessManager(this);
           QString musicUrl = QString("https://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash=%1&album_id=%2").arg(musicHashList[i]).arg(alum_id[i]);//歌曲搜索列表，从中提取歌曲的hash值
    //       QString musicUrl = QString("https://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash=%1").arg(musicHashList[0]);
           QNetworkRequest *networkrequest = new QNetworkRequest();//定义请求对象
           networkrequest->setUrl(musicUrl);//请求格式设置给请求
           music_network->get(*networkrequest);// 发送 GET 请求
           connect(music_network, &QNetworkAccessManager::finished, this, [this, i](QNetworkReply *reply){
               musicInfoBack(reply, i);
           });
           qDebug() << musicUrl;
       }



    }


}

void qqMusicSearch::musicInfoBack(QNetworkReply *reply, int index)
{
    songCount += 1;
    if (reply->error() != QNetworkReply::NoError) { // 检查请求是否成功
        qDebug() << "请求失败";
        return;
    }
    else
    {
        // 解析 JSON 数据
        QString url;
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull() && doc.isObject())
        {
            QJsonObject rootObject = doc.object();

            // 提取 url 字段
            if (rootObject.contains("url") && rootObject["url"].isString())
            {
                url = rootObject["url"].toString();
                if(!url.isEmpty())
                {
//                    QMediaPlayer* player = new QMediaPlayer(this);
//                    QMediaPlaylist *playlist = new QMediaPlaylist(this);
//                    playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置循环播放
//                    player->setMedia(QUrl(url));
//                    player->play();
                    musicIdList.append(index);
                    musicUrlList.append(url);
                    qDebug() << "Song URL:" << url;
                    //提取歌名
                    QString songName = rootObject.value("songName").toString();
                    if (!songName.isEmpty()) {
                        musicNameList.append(songName);
                        qDebug()<<songName;
                    }
                    // 提取歌手名
                    QString singerName = rootObject.value("author_name").toString();
                    if (!singerName.isEmpty()) {
                        singerNameList.append(singerName);
                        qDebug()<<singerName;
                    }
                }

            }
            else
            {
//                qDebug() << "URL 字段不存在或格式不正确";
            }


        }
        else
        {
            qDebug() << "无效的 JSON 数据。";
        }

    }
    qRegisterMetaType<std::tuple<QVector<int>, QStringList, QStringList, QStringList>>(
        "std::tuple<QVector<int>, QStringList, QStringList, QStringList>");
    qDebug()<<index;
    if(songCount == musicHashList.size())
    {
        musicIdList.append(1);
        musicNameList.append("dafdsaf");
        singerNameList.append("adsfasdf");
        musicUrlList.append("https://sharefs.tx.kugou.com/202503021315/a7d1bd05143e5bdab6f42cf466de811e/v3/07bc472acde477702a36bf02306c40f9/yp/full/ap1000_us0_pi409_s1049419750.mp3");
        emit sendToWidget(std::make_tuple(this->musicIdList, this->musicNameList, this->singerNameList, this->musicUrlList));
    }

}

void qqMusicSearch::getMusicHash(QByteArray data)
{
    QString json(data);
    //解析json文件
    QString songname_original; //歌曲名
    QString singername;        //歌手
    QString hashStr;           //hash
    QString album_name;        //专辑
    int duration;          	   //时间
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (parse_doucment.isObject())
        {
            QJsonObject rootObj = parse_doucment.object();
            if (rootObj.contains("data"))
            {
                QJsonValue valuedata = rootObj.value("data");
                if (valuedata.isObject())
                {
                    QJsonObject valuedataObject = valuedata.toObject();
                    if (valuedataObject.contains("info"))
                    {
                        QJsonValue valueArray = valuedataObject.value("info");
                        if (valueArray.isArray())
                        {
                            QJsonArray array = valueArray.toArray();
                            int size = array.size();
                            qDebug() << size;
                            for (int i = 0; i < size; i++)
                            {
                                QJsonValue value = array.at(i);
                                if (value.isObject())
                                {
                                    QJsonObject object = value.toObject();
                                    if (object.contains("songname_original"))//歌曲名
                                    {
//                                        musicNameList.append(songname_original);
                                        QJsonValue AlbumID_value = object.take("songname_original");
                                        if (AlbumID_value.isString())
                                        {
                                            songname_original = AlbumID_value.toString();
                                        }
                                    }
                                    if (object.contains("singername"))//歌手
                                    {
//                                        singerNameList.append(singername);
                                        QJsonValue AlbumID_value = object.take("singername");
                                        if (AlbumID_value.isString())
                                        {
                                            singername = AlbumID_value.toString();
                                        }
                                    }
                                    if (object.contains("album_name"))//专辑
                                    {
                                        QJsonValue AlbumID_value = object.take("album_name");
                                        if (AlbumID_value.isString())
                                        {
                                            album_name = AlbumID_value.toString();
                                        }
                                    }
                                    if (object.contains("hash")) //hash
                                    {
                                        QJsonValue FileHash_value = object.take("hash");
                                        if (FileHash_value.isString())
                                        {
                                            hashStr = FileHash_value.toString();
//                                            qDebug() << hashStr;
                                            musicHashList.append(FileHash_value.toString());
                                        }
                                    }
                                    if (object.contains("album_id"))
                                    {
                                        QJsonValue FileHash_value = object.take("album_id");
                                        if (FileHash_value.isString())
                                        {
//                                            qDebug() << FileHash_value;
                                            //用Vector保存每首歌曲的album_id
                                            alum_id.append(FileHash_value.toString());
                                        }
                                    }
                                    if (object.contains("duration"))//时长
                                    {
                                       QJsonValue AlbumID_value = object.take("duration").toInt();
                                       duration = AlbumID_value.toInt();
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
//        qDebug() << json_error.errorString();
    }
}
