#ifndef SSINDICATOR_H
#define SSINDICATOR_H

#include <QGraphicsPixmapItem>

class SSindicator: public QGraphicsPixmapItem
{

public:
    SSindicator(const QPixmap &pixmap = QPixmap(":images/ind_default.png"),QGraphicsItem *parent = Q_NULLPTR);

};

#endif // SSINDICATOR_H
