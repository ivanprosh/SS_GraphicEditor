#include <QtWidgets>

#include "graphicscene.h"
#include "commands.h"

GraphicScene::GraphicScene(QObject *parent)
    : QGraphicsScene(parent)
{
    //itemList = 0;
}
/*
void GraphicScene::removeItem(QGraphicsItem *item)
{
    QUndoCommand *delCommand = new Delete(item, this);
    undostack->push(addCommand);
    QGraphicsScene::removeItem(item);
}
*/
void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
    QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                     event->buttonDownScenePos(Qt::LeftButton).y());
                     */
    const QList<QGraphicsItem *> itemList = selectedItems();
    //movingItem = itemList.isEmpty() ? 0 : itemList.first();

    if (!itemList.isEmpty() && event->button() == Qt::LeftButton) {
        sceneBeforeMove.clear();
        foreach (QGraphicsItem *item, itemList) {
            sceneBeforeMove[item] = item->pos();
        }
    }

    //clearSelection();
    QGraphicsScene::mousePressEvent(event);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!sceneBeforeMove.empty() && event->button() == Qt::LeftButton) {
        bool dirty;
        foreach (QGraphicsItem *item, sceneBeforeMove.keys()) {
            if (sceneBeforeMove[item] != item->pos())
                dirty=true;
        }
        if(dirty) emit itemMoved(sceneBeforeMove);
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
