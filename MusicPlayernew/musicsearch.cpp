#include "musicsearch.h"

MusicSearch::MusicSearch(QString s):searchIfo(s)
{
}

/*

https://ws6.stream.qqmusic.qq.com/C400001AzzRc42NBVo.m4a?guid=3176867591&vkey=967EA1E6ADB9605335AF8E1DC9141F5F6468C01042BCCE61976B11DC92408D33FDDBFC9C688E5DEE91AD50D7DDE8C41F52CC31DAE5EFA37B__v21ebdd745&uin=2185129861&fromtag=120032&src=C400001ziKgJ3o5Ipp.m4a
{"comm":{"cv":4747474,"ct":24,"format":"json","inCharset":"utf-8","outCharset":"utf-8","notice":0,"platform":"yqq.json","needNewCode":1,"uin":2185129861,"g_tk_new_20200303":57714193,"g_tk":57714193},"req_1":{"module":"music.trackInfo.UniformRuleCtrl","method":"CgiGetTrackInfo","param":{"types":[0],"ids":[5293866]}},"req_2":{"module":"userInfo.VipQueryServer","method":"SRFVipQuery_V2","param":{"uin_list":["2185129861"]}},"req_3":{"module":"userInfo.BaseUserInfoServer","method":"get_user_baseinfo_v2","param":{"vec_uin":["2185129861"]}},"req_4":{"module":"music.lvz.VipIconUiShowSvr","method":"GetVipIconUiV2","param":{"PID":3}}}:

*/









void MusicSearch::run()
{
    network = new QNetworkAccessManager(this);//初始化网络对象
    songUrl = QString("https://music.163.com/song/media/outer/url?id=%1");
    searchUrl = QString("http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s={%1}&type=1&offset=0&total=true&limit=%0").arg(MAXSIZE);//网易云api
    //"http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s={"+str1+"}&type=1&offset=0&total=true&limit=1"


    //开始search
    getApiConnect();
    qDebug()<<musicIdList.size()<<"222222222222222222222222222222222222222222222222222";
    exec();
}
void MusicSearch::getApiConnect()
{

    QString apiUrl = searchUrl.arg(searchIfo);


//    QString apiUrl = "http://mobilecdn.kugou.com/api/v3/search/song?format=json&keyword=真的爱你&page=1";

    QNetworkRequest networkrequest;//定义请求对象
    networkrequest.setUrl(apiUrl);//请求格式设置给请求对象
    network->get(networkrequest);// 发送 GET 请求
    connect(network, &QNetworkAccessManager::finished, this, &MusicSearch::infoBack);
//    qDebug() << apiUrl;
}





// 网络请求返回时的处理函数
void MusicSearch::infoBack(QNetworkReply* reply)
{

    if (reply->error() != QNetworkReply::NoError) { // 检查请求是否成功
//        QMessageBox::information(this, "错误", "网络请求失败：" + reply->errorString());
        return;
    }
    else
    {
       this->flag = true;
    }


    QByteArray data = reply->readAll();

    dealJson(data);



}

void MusicSearch::dealJson(QByteArray data)
{
    // 动态注册类型
    qRegisterMetaType<std::tuple<QVector<int>, QStringList, QStringList, QStringList>>(
        "std::tuple<QVector<int>, QStringList, QStringList, QStringList>");
    QJsonParseError jsonError; // 用于存储 JSON 解析错误信息
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError); // 将字节流解析为 JSON 文档

    if (jsonError.error != QJsonParseError::NoError) {
        emit sendToWidget(std::make_tuple(this->musicIdList, this->musicNameList, this->singerNameList, this->musicUrlList));

        return;
    }

    // 获取 JSON 文档中的 "result" 对象
    QJsonObject result = doc.object()["result"].toObject();
    if (!result.contains("songs") || result["songs"].toArray().isEmpty()) {
        qDebug()<<"11111111111111111111111111111111111111111111111111111111";
        emit sendToWidget(std::make_tuple(this->musicIdList, this->musicNameList, this->singerNameList, this->musicUrlList));
        return;
    }
    songs = result["songs"].toArray();
//    qDebug() << songs;
    int maxSongs = qMin(songs.size(), MAXSIZE); // 防止数组越界，只提供前MAXSIZE个搜索结果
    for (int i = 0; i < maxSongs; ++i) {
        QJsonObject song = songs[i].toObject(); // 获取第 i 首歌曲的信息

        int musicId = song["id"].toInt(); // 获取歌曲 ID
        QString musicName = song["name"].toString(); // 获取歌曲名称

        // 获取歌手信息（可能有多个歌手）
        QString musicSinger;
        QJsonArray artists = song["artists"].toArray();//如果有多个歌手全列出来
        for (int j = 0; j < artists.size(); ++j) {
            if (j > 0) {
                musicSinger += " / ";
            }
            musicSinger += artists[j].toObject()["name"].toString();
        }
        QString playUrl = songUrl.arg(musicId);
        this->musicIdList.append(musicId);
        this->musicNameList.append(musicName);
        this->singerNameList.append(musicSinger);
        this->musicUrlList.append(playUrl);

//        qDebug() << i << musicName << musicNameList[i];

    }
    // 获取第一首歌曲的信息
//    QJsonObject song = result["songs"].toArray().first().toObject();
//    int musicId = song["id"].toInt(); // 获取歌曲 ID
//    QString musicName = song["name"].toString(); // 获取歌曲名称
//    QString musicSinger = song["artists"].toArray().first().toObject()["name"].toString(); // 获取歌手名称

//    // 构造歌曲的播放 URL
//    QString playUrl = songUrl.arg(musicId);
//    playlist->addMedia(QUrl(playUrl)); // 将歌曲添加到播放列表
//    ui->plainTextEdit->appendPlainText(musicName + "-" + musicSinger); // 在文本框中显示歌曲信息



    emit sendToWidget(std::make_tuple(this->musicIdList, this->musicNameList, this->singerNameList, this->musicUrlList));
    qDebug()<<musicIdList.size()<<"333333333333333333333333333333333311111";
}
std::tuple<QVector<int>, QStringList, QStringList> MusicSearch::getSongsInfo()
{
    return std::make_tuple(musicIdList, musicNameList, singerNameList);
}


