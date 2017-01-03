#include <QDynamicPropertyChangeEvent>
#include "ssdyntext.h"
#include "global.hpp"

SSDynText::SSDynText(const QRect &_rect) : SSRectItem(_rect)
{
    //светло-серый цвет фона
    //отрисовка хранится в кэше
    QGraphicsItem::setCacheMode(DeviceCoordinateCache);
    setBackColor(QColor(212,212,212));
    setForeColor(Qt::darkGreen);
    QGraphicsRectItem::setBrush(QBrush(BackColor(),Qt::SolidPattern));
    QGraphicsRectItem::setPen(QPen(ForeColor()));

  /*
     Добавление на сцену производится через команды QUndoCommand для
     реализации механизма повтора-отмены действий
  */

  setSelected(true);

}
void SSDynText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black));
    painter->setBrush(QGraphicsRectItem::brush());
    painter->drawRect(rect());

    if (option->state & QStyle::State_Selected)
        paintSelectionOutline(painter);
    //рисуем идентификатор кнопки
    painter->setPen(QGraphicsRectItem::pen());
    if (VisibleIndex()<=StatesCount()){
        painter->setFont(Font());
        painter->drawText(option->rect,XAlignment()|YAlignment(),states.value(tr("State")+QString::number(VisibleIndex())).second);
    }
}
void SSDynText::updateDynamicPropView(QString propSingleName, QHash<QString,SScommandProperty>& hash,int count){

    int index;
    for(index=0;index<count;index++){
       QString propName(propSingleName+QString::number(index+1));
       if(!hash.contains(propName)){
           hash.insert(propName,qMakePair(QString(),QString()));
       }
       setProperty(propName.toUtf8(),QVariant::fromValue(hash.value(propName)));
    }
    QRegExp rgxPattern(propSingleName+"([0-9])");
    QListIterator<QByteArray> iter(dynamicPropertyNames());
    while(iter.hasNext()){
        if(rgxPattern.exactMatch(iter.next()) && rgxPattern.cap(1).toInt() > count)
        {
            setProperty(QString(propSingleName+rgxPattern.cap(1)).toUtf8(),QVariant());
        }
    }

    qDebug() << dynamicPropertyNames();

}
void SSDynText::dynPropertyChanged(QByteArray propName){

    if(states.contains(propName)) {
        updateHashDynProperties(states,propName);
    }
}
void SSDynText::updateHashDynProperties(QHash<QString,SScommandProperty>& hash,const QString& propName){
    SScommandProperty value = property(propName.toUtf8()).value<SScommandProperty>();
    if(!value.first.isNull())
        hash[propName].first = value.first;
    if(!value.second.isNull())
        hash[propName].second = value.second;
}

bool SSDynText::event(QEvent *event)
{
    if (event->type() == QEvent::DynamicPropertyChange)
    {
        QDynamicPropertyChangeEvent *const propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
        QByteArray propName = propEvent->propertyName();
        dynPropertyChanged(propName);
    }
    return true;
    //return accept;
}
/*
QDataStream &operator<<(QDataStream &out, const SSDynText &dynTextItem)
{
   return out;
}
QDataStream &operator>>(QDataStream &in, const SSDynText &dynTextItem)
{
   return in;
}
QSettings &operator<<(QSettings &out, const SSDynText &dynTextItem)
{
   return out;
}

QSettings &operator>>(QSettings &in, SSDynText &dynTextItem)
{
   return in;
}
*/
