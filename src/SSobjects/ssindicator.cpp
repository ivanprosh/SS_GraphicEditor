#include "ssindicator.h"
#include "templmodelinfo.h"
#include "standardtablemodel.hpp"
#include <QGraphicsScene>
#include "../global.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

SSindicator::SSindicator(const QPoint &position, QGraphicsScene *scene,
                         const QModelIndex &Template_Index, QGraphicsItem *parent):QGraphicsObject(parent),
                        m_TemplateName(Template_Index.isValid() ? Template_Index.data().toString():QString()),
                        image(Template_Index.isValid() ? Template_Index.data(Qt::DecorationRole).value<QPixmap>():QPixmap(":/images/obj_icons/default_obj.png"))
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
    if (newName != m_TemplateName) {
        m_TemplateName = newName;
        emit dirty();
    }
}

void SSindicator::TemplateNameChanged(const QString& oldName,const QString& newName)
{
    if(oldName==TemplateName())
       setTemplateName(newName);
}

void SSindicator::ImageChanged(const QString& m_TemplateName,const QPixmap &pix)
{
    if(m_TemplateName==TemplateName() && image.cacheKey()!=pix.cacheKey()){
        image = pix;
        emit dirty();
    }
}

QVariant SSindicator::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (isDirtyChange(change))
        emit dirty();
    return QGraphicsItem::itemChange(change, value);
}

QDataStream &operator<<(QDataStream &out, const SSindicator &indicator)
{
    out << indicator.pos()
        << indicator.zValue()
        //<< indicator.rect()
        //<< indicator.model
        << indicator.TemplateName()
        << indicator.image;
    return out;
}


QDataStream &operator>>(QDataStream &in, SSindicator &indicator)
{
    QPointF position;
    double z;
    QRectF rect;
    //StandardTableModel* mod;
    QString TemplName;
    QPixmap pix;

    in >> position >> z >> TemplName >> pix;

    indicator.setPos(position);
    indicator.setZValue(z);
    //indicator.model = mod;
    indicator.setTemplateName(TemplName);
    indicator.image = pix;

    return in;
}
