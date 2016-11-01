#include "ssindicator.h"
#include "templmodelinfo.h"
#include "standardtablemodel.hpp"
#include <QGraphicsScene>
#include "../global.hpp"
#include <QGraphicsPixmapItem>
#include <QPainter>

SSindicator::SSindicator(const QPoint &position, QGraphicsScene *scene,
                         const StandardTableModel& sourceModel,const QString &n_TemplateName, QGraphicsItem *parent):QGraphicsObject(parent),
                        model(sourceModel),m_TemplateName(n_TemplateName)
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable);
    setPos(position);
    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);

    QList<QStandardItem*> items = model.findItems(TemplateName(),Qt::MatchExactly,Name);
    Q_ASSERT(!items.isEmpty());
    image = &(items.at(0)->data(Qt::DecorationRole).value<QPixmap>());
}

void SSindicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(image->rect(),*image);
    if (isSelected())
        paintSelectionOutline(painter);
}

QRectF SSindicator::boundingRect() const
{
   return image->rect();
}

QPainterPath SSindicator::shape() const
{
   QPainterPath path;
   path.addRect(image->rect());
   return path;
}

void SSindicator::paintSelectionOutline(QPainter *painter)
{
    QPen pen(Qt::DashLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath path;
    path.addRect(image->rect());
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

