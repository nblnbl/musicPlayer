#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QMouseEvent>
#include <QIcon>
#include <QPixmap>
#include <QPainter>

class ButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject* parent = nullptr);

    // 创建按钮
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    // 绘制按钮
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)override;

    // 设置按钮数据
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    // 将按钮数据写回模型
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    // 更新按钮的几何位置和大小
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    //改变table2按钮图片
    void chageTableIcon(QString s);

    QString iconImage = ":/images/add.png";

signals:
    void sendPlayInfo(int = -1);
    void sendAddInfo(int = -1);

};

#endif
