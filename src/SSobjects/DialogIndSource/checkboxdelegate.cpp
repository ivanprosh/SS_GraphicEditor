#include <QPixmapCache>
#include "checkboxdelegate.h"

CheckBoxDelegate::CheckBoxDelegate(QObject *parent): QStyledItemDelegate(parent)
{
    che�kbox = new QCheckBox;
    che�kbox->setFixedSize(
                qRound(1.3 * che�kbox->sizeHint().height()),
                che�kbox->sizeHint().height());
}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool selected = option.state & QStyle::State_Selected;
    QPalette palette(option.palette);
    palette.setColor(QPalette::Active, QPalette::Window,
                     selected ? option.palette.highlight().color()
                              : option.palette.base().color());

     int yOffset = che�kbox->height() < option.rect.height() ? (option.rect.height() - che�kbox->height()) / 2 : 0;
     QRect checkboxRect(option.rect.x(), option.rect.y() + yOffset, che�kbox->width(), che�kbox->height());
     che�kbox->setPalette(palette);
     bool checked = index.model()->data(index, Qt::CheckStateRole).toInt() == Qt::Checked;
     che�kbox->setChecked(checked);
     QString checkboxKey = QString("CHECKBOX:%1.%2").arg(selected).arg(checked);
     paintWidget(painter, checkboxRect, checkboxKey, che�kbox);
}
/*
QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
*/
void CheckBoxDelegate::paintWidget(QPainter *painter,
        const QRect &rect, const QString &cacheKey,
        QWidget *widget) const
{
    QPixmap pixmap(widget->size());

    if (!QPixmapCache::find(cacheKey, &pixmap)) {
        widget->render(&pixmap);
        QPixmapCache::insert(cacheKey, pixmap);
    }
    painter->drawPixmap(rect, pixmap);
}
