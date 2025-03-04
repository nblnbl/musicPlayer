#include "musiclistwidget.h"
#include "ui_musiclistwidget.h"
#include "mainwindow.h"

musicListwidget::musicListwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::musicListwidget)
{
    ui->setupUi(this);

//    //设置表格
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"歌名", "歌手", "操作"});
    // 设置自定义代理
    delegate = new ButtonDelegate(ui->playTableView);
    delegate->chageTableIcon(":/images/min.png");
    ui->playTableView->setItemDelegateForColumn(2, delegate);
    //与查询表格中的自定义委托按钮建立连接
    connect(delegate, &ButtonDelegate::sendPlayInfo, this, &musicListwidget::dealPlaySlot);
    connect(delegate, &ButtonDelegate::sendAddInfo, this, &musicListwidget::dealDeleteSlot);



//    ui->playTableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选中时为整行选中
    ui->playTableView->setSelectionMode(QAbstractItemView::NoSelection);// 设置选中模式为禁用选中
    ui->playTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格的单元为只读属性，即不能编辑
    ui->playTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->playTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条



    ui->playTableView->setModel(model);

}

void musicListwidget::getMainWindowPtr(MainWindow *window)
{
    this->mainWindow= window;

    connect(mainWindow, &MainWindow::sendPlaylistCurrentIndexToWidget, this, [this](int playlistIndex)
        {
        if (playlistIndex != -1&&model->rowCount()>0)
           {
               // 清除之前的高亮
                qDebug()<<"清除之前的高亮" << playlistIndex;
               for (int row = 0; row < model->rowCount(); ++row)
               {
                   model->item(row, 0)->setBackground(QColor(255, 255, 255, 50));
                   model->item(row, 1)->setBackground(QColor(255, 255, 255, 50));
                   model->item(row, 2)->setBackground(QColor(255, 255, 255, 50));
//                   QItemSelectionModel* selectionModel = ui->playTableView->selectionModel();
//                   if (selectionModel) {
//                       selectionModel->clearSelection();  // 清除所有选中状态
//                   }
               }

               // 设置当前播放行为高亮
//               model->item(playlistIndex, 0)->setData("current", Qt::DisplayRole);
               model->item(playlistIndex, 0)->setBackground(QColor(255, 200, 200));
               model->item(playlistIndex, 1)->setBackground(QColor(255, 200, 200));
               model->item(playlistIndex, 2)->setBackground(QColor(255, 200, 200));
           }
        });

}


musicListwidget::~musicListwidget()
{
    delete ui;
}

void musicListwidget::dealPlaySlot(int index)
{

    mainWindow->chageMediaToWidget(index);
}

void musicListwidget::dealDeleteSlot(int index)
{
    model->removeRow(index);
    mainWindow->deleteMediaToWidget(index);
}

void musicListwidget::on_closeWindowBtn_clicked()
{
    this->close();
}

void musicListwidget::on_clearPlayListBtn_clicked()
{
    //清空播放列表
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->playTableView->model());
    if (model) {
            // 清空模型
            model->clear();

        }
    emit sendToMainWindow(0);
}
