#include "ButtonDelegate.h"
#include <QApplication>
#include <QStyle>
#include <QDebug>

ButtonDelegate::ButtonDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

QWidget* ButtonDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QPushButton* button = new QPushButton("开始播放", parent);
    button->setStyleSheet("QPushButton { background-color: lightblue; }");

    // 连接按钮的点击信号
    connect(button, &QPushButton::clicked, [index]() {
        qDebug() << "Button clicked for row" << index.row();
        // 在这里添加按钮点击后的逻辑
    });

    return button;
}
// 绘制按钮
void ButtonDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == 2) {  // 只在第3列绘制按钮
        QRect rect = option.rect;

        // 绘制“开始播放”按钮
        QRect playRect(rect.left(), rect.top(), rect.width() / 2, rect.height());
        QPixmap playIcon(":/images/logo.png");
        if (playIcon.isNull()) {
            qDebug() << "Failed to load play icon";
        }

        // 绘制“加入列表”按钮
        QRect addRect(rect.left() + rect.width() / 2, rect.top(), rect.width() / 2, rect.height());
        QPixmap addIcon(iconImage);
        if (addIcon.isNull()) {
            qDebug() << "Failed to load add icon";
        }

        // 绘制按钮背景（默认透明，鼠标悬停时半透明）........没作用
        if (option.state & QStyle::State_MouseOver) {
            if (playRect.contains(option.rect.center())) {
                painter->fillRect(playRect, QColor(255, 255, 255, 50));
            } else if (addRect.contains(option.rect.center())) {
                painter->fillRect(addRect, QColor(255, 255, 255, 50));
            }
        }

        // 绘制按钮图标
        QPixmap scaledPlayIcon = playIcon.scaled(QSize(35, 35), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaledAddIcon = addIcon.scaled(QSize(25, 25), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        painter->drawPixmap(playRect.center() - QPoint(scaledPlayIcon.width() / 2, scaledPlayIcon.height() / 2), scaledPlayIcon);
        painter->drawPixmap(addRect.center() - QPoint(scaledAddIcon.width() / 2, scaledAddIcon.height() / 2), scaledAddIcon);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool ButtonDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (event->type() == QEvent::MouseButtonRelease && index.column() == 2) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect rect = option.rect;

        QRect playRect(rect.left(), rect.top(), rect.width() / 2, rect.height());
        QRect addRect(rect.left() + rect.width() / 2, rect.top(), rect.width() / 2, rect.height());

        if (playRect.contains(mouseEvent->pos())) {
            qDebug() << "Play button clicked for row" << index.row();
            emit sendPlayInfo(index.row());//将要播放的歌曲索引信息传出
            return true;
        } else if (addRect.contains(mouseEvent->pos())) {
            qDebug() << "Add to playlist button clicked for row" << index.row();
            emit sendAddInfo(index.row());//将要加入列表的歌曲索引信息传出
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}


void ButtonDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    // 根据模型数据动态设置按钮文本或其他属性
}

void ButtonDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    // 将按钮的状态保存到模型中
}

void ButtonDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    editor->setGeometry(option.rect);
}

void ButtonDelegate::chageTableIcon(QString s)
{
    iconImage = s;
}
