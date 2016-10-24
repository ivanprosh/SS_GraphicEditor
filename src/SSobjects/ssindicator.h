#ifndef SSINDICATOR_H
#define SSINDICATOR_H

class QGraphicsPixmapItem;

class SSindicator: public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    SSindicator(const QPixmap &pixmap = QPixmap(":images/ind_default.png"),QGraphicsItem *parent = Q_NULLPTR);
protected:

};

#endif // SSINDICATOR_H
