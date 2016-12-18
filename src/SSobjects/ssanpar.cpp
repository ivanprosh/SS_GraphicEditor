#include "ssanpar.h"
#include "../global.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

QPixmap* SSAnpar::anparTitleimage = 0;

SSAnpar::SSAnpar(const QRect &rect, QGraphicsScene *scene) : QObject(), QGraphicsRectItem()
{
  setFlags(QGraphicsItem::ItemIsSelectable|
//           QGraphicsItem::ItemSendsGeometryChanges|
           QGraphicsItem::ItemIsMovable|
           QGraphicsItem::ItemIsFocusable|
           QGraphicsItem::ItemIgnoresTransformations);
  setPos(rect.center());

  if(!anparTitleimage)
    anparTitleimage = new QPixmap(":/images/obj_icons/anpar_title.bmp");

  /*
     Добавление на сцену производится через команды QUndoCommand для
     реализации механизма повтора-отмены действий
  */

  setSelected(true);

}

void SSAnpar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(option->rect.x(),option->rect.y(),anparTitleimage->width(),anparTitleimage->height(),*anparTitleimage);
    if(isSelected())
        paintSelectionOutline(painter);
}

QRectF SSAnpar::boundingRect() const
{
    return QRect(0,0,anparTitleimage->width(),anparTitleimage->height());
}

QPainterPath SSAnpar::shape() const
{
   QPainterPath path;
   path.addRect(anparTitleimage->rect());
   return path;
}

void SSAnpar::paintSelectionOutline(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath bound;
    bound.addRect(anparTitleimage->rect());
    painter->drawPath(bound);
}
