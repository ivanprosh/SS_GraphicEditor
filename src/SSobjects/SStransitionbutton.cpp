#include "sstransitionbutton.h"
#include "../global.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#ifdef QT_DEBUG
#include <QDebug>
#endif

QPixmap* SSTransitionButton::ButtonTitleimage = 0;

SSTransitionButton::SSTransitionButton(const QRect &rect) : QObject(), QGraphicsRectItem(),m_resizing(false),
    minRect(rect)
{
  setFlags(QGraphicsItem::ItemIsSelectable|
//           QGraphicsItem::ItemSendsGeometryChanges|
           QGraphicsItem::ItemIsMovable|
           QGraphicsItem::ItemIsFocusable|
           QGraphicsItem::ItemIgnoresTransformations);
  setPos(rect.center());

  if(!ButtonTitleimage){
    ButtonTitleimage = new QPixmap(":/images/controlbutton.svg");
    //ButtonTitleimage->
  }
  /*
     ���������� �� ����� ������������ ����� ������� QUndoCommand ���
     ���������� ��������� �������-������ ��������
  */

  setRect(rect);
  setSelected(true);

}

void SSTransitionButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //qDebug() << option->rect;
    painter->drawPixmap(option->rect.x(),option->rect.y(),rect().width(),rect().height(),*ButtonTitleimage);
    if(isSelected())
        paintSelectionOutline(painter);
}

QRectF SSTransitionButton::boundingRect() const
{
    QRect result(ButtonTitleimage->rect());
    if(rect().width() > result.width()) result.setWidth(rect().width());
    if(rect().height() > result.height()) result.setHeight(rect().height());
    return result;
}

QPainterPath SSTransitionButton::shape() const
{
   QPainterPath path;
   path.addRect(boundingRect());
   return path;
}

void SSTransitionButton::paintSelectionOutline(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath bound;
    bound.addRect(boundingRect());
    painter->drawPath(bound);
}
void SSTransitionButton::keyPressEvent(QKeyEvent *event)
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


void SSTransitionButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        m_resizing = true;
        setCursor(Qt::SizeAllCursor);
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}


void SSTransitionButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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


void SSTransitionButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
        emit dirty();
    }
    else
        QGraphicsRectItem::mouseReleaseEvent(event);
}
