#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QObject>
#include <QGraphicsScene>

class QGraphicsSceneDragDropEvent;
class QGraphicsViewItem;

//! [0]
class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicScene(QObject *parent = 0);
    //void removeItem(QGraphicsItem *item);
signals:
    void itemMoved(QHash<QGraphicsItem *,QPointF> sceneBefore);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    //QUndoStack* undostack;
    //QGraphicsItem *movingItem;
    QHash<QGraphicsItem *,QPointF> sceneBeforeMove;
    //QPointF oldPos;
};


#endif // GRAPHICSCENE_H
