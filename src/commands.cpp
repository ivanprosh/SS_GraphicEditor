/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "commands.h"
#include "itemtypes.hpp"
#include <QGraphicsItem>
#include <QGraphicsScene>


MoveCommand::MoveCommand(QHash<QGraphicsItem*, QPointF>& m_sceneBefore,
                 QUndoCommand *parent)
    : QUndoCommand(parent)
{
    foreach (QGraphicsItem* item, m_sceneBefore.keys()) {
        itemsCoordinates[item] = qMakePair(m_sceneBefore[item],item->pos());
    }

}

//позволяет сделать вывод о том, можно ли объединить текущую команду с прошлой
//возвращает истину, если список итемов совпадает
bool MoveCommand::mergeWith(const QUndoCommand *command)
{
    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
    QMap<QGraphicsItem*, QPair<QPointF,QPointF> > newitemsCoordinates = moveCommand->itemsCoordinates;

    if(newitemsCoordinates.size()!=itemsCoordinates.size())
        return false;

    foreach (QGraphicsItem* item, newitemsCoordinates.keys()) {
        if (!itemsCoordinates.contains(item))
            return false;
    }

    QMapIterator<QGraphicsItem*, QPair<QPointF,QPointF> > i(newitemsCoordinates);
    while (i.hasNext()) {
        i.next();
        itemsCoordinates[i.key()].second = i.value().second;
    }

    setText(QObject::tr("Move %1")
        .arg(createCommandString(itemsCoordinates.keys().first(), itemsCoordinates.first().second)));

    return true;
}

void MoveCommand::undo()
{
    foreach (QGraphicsItem* item, itemsCoordinates.keys()) {
        item->setPos(itemsCoordinates.value(item).first);
    }
    //itemsCoordinates.keys().first()->scene()->update();

    setText(QObject::tr("Move %1")
        .arg(createCommandString(itemsCoordinates.keys().first(), itemsCoordinates.first().second)));
}

/*
    метод вызывается автоматически после конструктора, а также по команде пользователя
*/
void MoveCommand::redo()
{
    foreach (QGraphicsItem* item, itemsCoordinates.keys()) {
        item->setPos(itemsCoordinates.value(item).second);
    }
    setText(QObject::tr("Move %1")
        .arg(createCommandString(itemsCoordinates.keys().first(), itemsCoordinates.first().second)));
}

DeleteCommand::DeleteCommand(QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    curGraphicsScene = scene;
    curGraphicsItems = curGraphicsScene->selectedItems();
    foreach (QGraphicsItem *item, curGraphicsItems) {
        item->setSelected(false);
    }

    setText(QObject::tr("Delete %1")
        .arg(createCommandString(curGraphicsItems.first(), curGraphicsItems.first()->pos())));
}

void DeleteCommand::undo()
{
    foreach (QGraphicsItem *item, curGraphicsItems) {
        curGraphicsScene->addItem(item);
    }
    curGraphicsScene->update();
}

void DeleteCommand::redo()
{
    foreach (QGraphicsItem *item, curGraphicsItems) {
        curGraphicsScene->removeItem(item);
    }
}

AddCommand::AddCommand(QGraphicsItem *item,
                       QGraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    static int itemCount = 0;

    curGraphicsScene = scene;
    curGraphicsItem = item;
    initialPosition = item->pos();

    scene->update();
    ++itemCount;
    setText(QObject::tr("Add %1")
        .arg(createCommandString(curGraphicsItem, initialPosition)));
}

AddCommand::~AddCommand()
{
    //if (!curGraphicsItem->scene())
    if (curGraphicsScene->items().contains(curGraphicsItem))
        delete curGraphicsItem;
}

void AddCommand::undo()
{
    curGraphicsScene->removeItem(curGraphicsItem);
    curGraphicsScene->update();
}

void AddCommand::redo()
{
    curGraphicsScene->clearSelection();
    curGraphicsScene->addItem(curGraphicsItem);
    curGraphicsItem->setFocus();

    curGraphicsScene->update();
}

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
    QString It_type;
    switch (item->type()) {
    case BoxItemType:
        It_type = QObject::tr("Rect");
        break;
    case TextItemType:
        It_type = QObject::tr("Text");
        break;
    case SSAnparItemType:
        It_type = QObject::tr("Anpar");
        break;
    case SSIndItemType:
        It_type = QObject::tr("Indicator");
        break;
    default:
        break;
    }

    return QObject::tr("%1 at (%2, %3)")
        .arg(It_type)
        .arg(pos.x()).arg(pos.y());
}
