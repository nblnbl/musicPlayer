#ifndef MUSICSEARCH_H
#define MUSICSEARCH_H

#define MAXSIZE 50

#include <QWidget>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include<QNetworkAccessManager>//使用该类获取网络请求
#include<QNetworkReply>//请求访问后返回的响应类
#include <QMessageBox>
#include<QNetworkAccessManager>//使用该类获取网络请求
#include<QNetworkReply>//请求访问后返回的响应类
#include <QThread>



class MusicSearch:public QThread
{
    Q_OBJECT

public:
    MusicSearch(QString s);
    void getApiConnect();
    std::tuple<QVector<int>, QStringList, QStringList> getSongsInfo();
    bool flag = false;
    void run() override; //线程处理函数

protected slots:
    void infoBack(QNetworkReply* reply);
signals:
    void sendToWidget(std::tuple<QVector<int>, QStringList, QStringList, QStringList>);

private:

    QNetworkAccessManager* network;
    QJsonArray songs; //歌曲信息
    QString songUrl; //获取到的歌曲url

    QVector<int> musicIdList;       // 存储歌曲 ID
    QStringList musicNameList;
    QStringList singerNameList;
    QStringList musicUrlList; //存放歌曲的播放链接

    QString searchIfo;
    QString searchUrl;//搜索的url
    void dealJson(QByteArray data);

};

#endif // MUSICSEARCH_H
