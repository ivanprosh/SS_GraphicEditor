#include "ssanpar.h"
#include "global.hpp"
//QPixmap* SSAnpar::anparTitleimage = 0;

SSAnpar::SSAnpar(const QRect &_rect) : SSRectItem(_rect),label("0")
{
    //светло-серый цвет фона
    //отрисовка хранится в кэше
    QGraphicsItem::setCacheMode(DeviceCoordinateCache);
    setBackColor(QColor(212,212,212));
    setForeColor(Qt::darkGreen);
    QGraphicsRectItem::setBrush(QBrush(BackColor(),Qt::SolidPattern));
    QGraphicsRectItem::setPen(QPen(ForeColor()));
  /*
  if(!anparTitleimage)
    anparTitleimage = new QPixmap(":/images/obj_icons/anpar_title.bmp");
    */
  /*
     Добавление на сцену производится через команды QUndoCommand для
     реализации механизма повтора-отмены действий
  */

  setSelected(true);

}
void SSAnpar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QGraphicsRectItem::brush());
    painter->drawRect(rect());

    if (option->state & QStyle::State_Selected)
        paintSelectionOutline(painter);
    //рисуем идентификатор кнопки
    painter->setPen(QGraphicsRectItem::pen());
    painter->setFont(Font());
    //qDebug() << option->rect.center().x() << "_" << option->rect.width() << painter->fontMetrics().width(QString("0")) << " & " << painter->fontMetrics().width(QString("0"))/2;
    painter->drawStaticText(option->rect.center().x()-painter->fontMetrics().width(QString("0"))/2,option->rect.center().y()-painter->fontMetrics().height()/2,label);
}
/*
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

QDataStream &operator<<(QDataStream &out, const SSAnpar &anparItem)
{
   return out;
}
QDataStream &operator>>(QDataStream &in, const SSAnpar &anparItem)
{
   return in;
}
QSettings &operator<<(QSettings &out, const SSAnpar &anparItem)
{
   return out;
}

QSettings &operator>>(QSettings &in, SSAnpar &anparItem)
{
   return in;
}
*/
