#include "ssanpar.h"


SSAnpar::SSAnpar(const QRect &rect, QGraphicsScene *scene) : QObject(), QGraphicsRectItem()
{
  setFlags(QGraphicsItem::ItemIsSelectable|
//           QGraphicsItem::ItemSendsGeometryChanges|
           QGraphicsItem::ItemIsMovable|
           QGraphicsItem::ItemIsFocusable|
           QGraphicsItem::ItemIgnoresTransformations);
  setPos(rect.center());


  //setRect(QRectF(QPointF(-rect.width() / 2.0,
  //                       -rect.height() / 2.0), rect.size()));
  //QGraphicsRectItem::setBrush(QBrush(Qt::black,Qt::SolidPattern));

  /*
     Добавление на сцену производится через команды QUndoCommand для
     реализации механизма повтора-отмены действий
  */
  //scene->clearSelection();
  //scene->addItem(this);
  setSelected(true);
  //setFocus();
}

void SSAnpar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap image(":/images/obj_icons/anpar_title.bmp");
    painter->drawPixmap(option->rect.x(),option->rect.y(),image.width(),image.height(),image);
    if(isSelected())
        paintSelectionOutline(painter);
}

QRectF SSAnpar::boundingRect() const
{
    return QRect(0,0,rect().width(),rect().height());
}

QPainterPath SSAnpar::shape() const
{
   QPainterPath path;
   path.addRect(image.rect());
   return path;
}
/*
bool SSindicator::saveToFile(QSettings &file)
{

}
*/
void SSAnpar::paintSelectionOutline(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath bound;
    bound.addRect(image.rect());
    painter->drawPath(bound);
}
