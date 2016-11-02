#include "ssindicator.h"
#include "templmodelinfo.h"
#include "standardtablemodel.hpp"
#include <QGraphicsScene>
#include "../global.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

SSindicator::SSindicator(const QPoint &position, QGraphicsScene *scene,
                         const StandardTableModel& sourceModel,const QModelIndex &Template_Index, QGraphicsItem *parent):QGraphicsObject(parent),
                        model(sourceModel),m_TemplateName(Template_Index.data().toString()),image(Template_Index.data(Qt::DecorationRole).value<QPixmap>())
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable);
    setPos(position);
    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);

}

void SSindicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(option->rect.x(),option->rect.y(),image.width(),image.height(),image);
}

QRectF SSindicator::boundingRect() const
{
    return QRect(0,0,image.width(),image.height());
}

QPainterPath SSindicator::shape() const
{
   QPainterPath path;
   path.addRect(image.rect());
   return path;
}

void SSindicator::paintSelectionOutline(QPainter *painter)
{
    QPen pen(Qt::DashLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath path;
    path.addRect(image.rect());
    painter->drawPath(path);
}

void SSindicator::setTemplateName(const QString &newName)
{
    if (isSelected() && newName != m_TemplateName) {
        m_TemplateName = newName;
        emit dirty();
    }
}

QVariant SSindicator::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (isDirtyChange(change))
        emit dirty();
    return QGraphicsItem::itemChange(change, value);
}

