#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QDataStream>
#include "ssrectitem.h"
#include "global.hpp"

SSRectItem::SSRectItem(const QRect &_rect) : QObject(),QGraphicsRectItem(),m_resizing(false)
{
    setFlags(QGraphicsItem::ItemIsSelectable|
  //           QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemIgnoresTransformations);
    setPos(_rect.center());
    setRect(QRectF(QPointF(-_rect.width() / 2.0,
                           -_rect.height() / 2.0), _rect.size()));
}

void SSRectItem::paintSelectionOutline(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath bound;
    bound.addRect(rect());
    painter->drawPath(bound);
}

void SSRectItem::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier ||
        event->modifiers() & Qt::ControlModifier) {
        bool move = event->modifiers() & Qt::ControlModifier;
        bool changed = true;
        double dx1 = 0.0;
        double dy1 = 0.0;
        double dx2 = 0.0;
        double dy2 = 0.0;
        switch (event->key()) {
            case Qt::Key_Left:
                if (move)
                    dx1 = -1.0;
                dx2 = -1.0;
                break;
            case Qt::Key_Right:
                if (move)
                    dx1 = 1.0;
                dx2 = 1.0;
                break;
            case Qt::Key_Up:
                if (move)
                    dy1 = -1.0;
                dy2 = -1.0;
                break;
            case Qt::Key_Down:
                if (move)
                    dy1 = 1.0;
                dy2 = 1.0;
                break;
            default:
                changed = false;
        }
        if (changed) {
            setRect(rect().adjusted(dx1, dy1, dx2, dy2));
            event->accept();
            emit dirty();
            return;
        }
    }
    QGraphicsRectItem::keyPressEvent(event);
}


void SSRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}


void SSRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
#ifdef ALTERNATIVE_RESIZING
        qreal dx = event->pos().x() - event->lastPos().x();
        qreal dy = event->pos().y() - event->lastPos().y();
        setRect(rect().adjusted(0, 0, dx, dy).normalized());
#else
        QRectF rectangle(rect());
        if (event->pos().x() < rectangle.x())
            rectangle.setBottomLeft(event->pos());
        else
            rectangle.setBottomRight(event->pos());
        setRect(rectangle);
#endif
        //qDebug() << rectangle.width() << rectangle.height();
        //ButtonTitleimage->scaled(rectangle.width(),rectangle.height(),Qt::IgnoreAspectRatio);
        scene()->update();
        //qDebug() << ButtonTitleimage->width() << ButtonTitleimage->height();
    }
    else
        QGraphicsRectItem::mouseMoveEvent(event);
}


void SSRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
    }
    else
        QGraphicsRectItem::mouseReleaseEvent(event);
}
//общие свойства для объектов типа SSRectItem
QDataStream &SSRectItem::print(QDataStream &out) const
{
    qDebug() << "In base class";
    return out;
}
QDataStream &operator<<(QDataStream &out, const SSRectItem &rectItem)
{
   return rectItem.print(out);
}
QDataStream &operator>>(QDataStream &in, const SSRectItem &rectItem)
{
   return in;
}
QSettings &operator<<(QSettings &out, const SSRectItem &rectItem)
{
   return out;
}
QSettings &operator>>(QSettings &in, SSRectItem &rectItem)
{
   return in;
}
