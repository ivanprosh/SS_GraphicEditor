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
    Q_CLASSINFO("Name", tr("Geometry properties scene"))

    //Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(QRectF rect READ sceneRect WRITE setSceneRect)
    Q_PROPERTY(int gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)

private:
    int m_gridSize;
    int gridSize(){ return m_gridSize;}
public:
    void setGridSize(int value) {m_gridSize = value; emit gridSizeChanged(value);}

    GraphicScene(QObject *parent = 0);
    //void removeItem(QGraphicsItem *item);
signals:
    void itemMoved(QHash<QGraphicsItem *,QPointF> sceneBefore);
    void gridSizeChanged(int);

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
