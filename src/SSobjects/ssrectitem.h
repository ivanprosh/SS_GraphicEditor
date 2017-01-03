#ifndef SSRECTITEM_H
#define SSRECTITEM_H

#include "itemtypes.hpp"
#include <QGraphicsRectItem>

class QDataStream;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QSettings;

class SSRectItem : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
    Q_CLASSINFO("Name", tr("Geometry properties"))
    //свойства геометрические
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
/*
    friend QDataStream &operator<<(QDataStream &out, const SSRectItem &item);
    friend QDataStream &operator>>(QDataStream &in, SSRectItem &item);
    friend QSettings &operator<<(QSettings &out, const SSRectItem &item);
    friend QSettings &operator>>(QSettings &in, SSRectItem &item);
 */
public:
    explicit SSRectItem(const QRect &_rect);
    virtual QDataStream& print(QDataStream& out) const;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paintSelectionOutline(QPainter *painter);

    //доп.св-ва
    bool m_resizing;
signals:
    void dirty();
public slots:
};

QDataStream &operator<<(QDataStream &out, const SSRectItem &item);
QDataStream &operator>>(QDataStream &in, SSRectItem &item);

QSettings &operator<<(QSettings &out, const SSRectItem &item);
QSettings &operator>>(QSettings &in, SSRectItem &item);

#endif // SSRECTITEM_H
