# 多媒体搜索播放引擎（持续更新中.......）
本项目实现一个功能丰富的MP3播放器及搜索引擎，项目利用QTcpServer和QTcpSocket搭建了服务端与客户端的通信架构，利用QThread实现多线程搜索，显著提升搜索效率，使用QNetworkAccessManager进行网络请求，支持异步操作，优化用户体验并借助QMediaPlayer实现音乐播放、进度控制等功能。
项目功能：
1.	服务端：处理客户端的登录和注册请求，并使用数据库存储用户账号密码管理及收藏音乐信息。
2.	客户端：
登录与注册：与服务端通信，支持用户登录、注册及收藏音乐同步。
搜索功能：利用多线程技术同时访问多个音乐数据源API，进行音乐搜索。 
播放功能：实现音乐播放、进度控制、音量调节等功能，支持播放列表管理、收藏音乐操作。
用户交互：支持播放/暂停、导入本地音乐、切换播放模式、当前播放音乐高亮显示等功能。

登录界面：
![image](https://github.com/user-attachments/assets/d407587c-72bb-4112-9eb2-56f82369486f)

搜索界面：
![image](https://github.com/user-attachments/assets/0b5dd395-9f59-49e5-896b-4077ee570cef)

![image](https://github.com/user-attachments/assets/f55cd1d8-5b12-4d52-93c8-05cb1624720a)


播放
![5b935ac3-0147-4c9f-93aa-dedfe4871cfa](https://github.com/user-attachments/assets/f6d6cd09-759d-43bd-93f3-5aa93510cccc)



后续准备优化一下ui，有更好的用户交互体验。
加入视频的搜索和播放功能，同时利用FFmpeg的高效编解码能力，支持多种视频格式，减少播放延迟.......................
