#include <QtWidgets>

#include "graphicscene.h"
#include "commands.h"

GraphicScene::GraphicScene(QObject *parent)
    : QGraphicsScene(parent)
{
    ;//itemList = 0;
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

    //const QList<QGraphicsItem *> itemList = selectedItems();
    //movingItem = itemList.isEmpty() ? 0 : itemList.first();

    //Кэш нужен для запоминания координат объектов (QUndoFramework)
    //очищать кэш нужно постоянно, так как могут возникнуть недействительные ключи,
    //например, если удалены Instance's шаблона
    sceneBeforeMove.clear();
    if (!selectedItems().isEmpty() && event->button() == Qt::LeftButton) {
        foreach (QGraphicsItem *item, selectedItems()) {
            sceneBeforeMove[item] = item->pos();
        }
    }

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
