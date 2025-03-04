#ifndef QQMUSICSEARCH_H
#define QQMUSICSEARCH_H


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
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkRequest>

class qqMusicSearch:public QThread
{
    Q_OBJECT
public:
    qqMusicSearch(QString s);
    void run() override; //线程处理函数

protected slots:
    void infoBack(QNetworkReply* reply);
    void musicInfoBack(QNetworkReply* reply, int index);


signals:
    void sendToWidget(std::tuple<QVector<int>, QStringList, QStringList, QStringList>);


private:
    QString s;
    QNetworkAccessManager* network;
    void getMusicHash(QByteArray data);
    QStringList musicHashList;
    QStringList alum_id;
    QStringList musicNameList;
    QStringList singerNameList;
    QStringList musicUrlList; //存放歌曲的播放链接
    QVector<int> musicIdList;
    int songCount;
};

#endif // QQMUSICSEARCH_H
