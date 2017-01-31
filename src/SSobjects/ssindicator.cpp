#include "ssindicator.h"
#include "DialogIndSource/templmodelinfo.h"
#include "DialogIndSource/standardtablemodel.hpp"
#include <QGraphicsScene>
#include "../global.hpp"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTimer>
#include <QDynamicPropertyChangeEvent>

SSindicator::SSindicator(const QPoint &position, QGraphicsScene *scene,
                         const QModelIndex &Template_Index,QGraphicsItem *parent):QGraphicsObject(parent),
                        m_commandsCount(0),
                        m_TemplateName(Template_Index.isValid() ? Template_Index.data().toString():QString()),
                        image(Template_Index.isValid() ? Template_Index.data(Qt::DecorationRole).value<QPixmap>():QPixmap(":/images/obj_icons/default_obj.png"))
{
    setFlags(QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable);
    setPos(position);
    scene->clearSelection();
    scene->addItem(this);
    setSelected(true);
    setStatesCount(Template_Index.isValid() ? Template_Index.data(StatesCountRole).toInt():0);

    initializeProperties();
    createInternalConnections();

}
void SSindicator::initializeProperties(){

    setAutoCloseDUWnd(0);
    setBorderCtrlDigPar_Tag(QString());
    setBorderCtrlDigPar_Unit(QString());
    setBorderCtrlDigParInvert(0);
    setBorderColor(Qt::yellow);
    setBorderWidth(1);
    setBorderBlinkFreq(1000);
    setMnemoNum(0);
    //setProperty("test", QVariant::fromValue(qMakePair(QString(),QString())));
    //QTimer::singleShot(0,this,SLOT(debinfo()));
    //commands.push_back(qMakePair(QString("First name"),QString("First Tag")));
}

void SSindicator::createInternalConnections()
{
    ;
}

void SSindicator::updateDynamicPropView(QString propSingleName, QHash<QString,SScommandProperty>& hash,int count){

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
/*
void SSindicator::updateCommandsView(){

    //if(commands.size() == commandsCount()) return;
    int index;
    for(index=0;index<commandsCount();index++){
       QString propName("Command"+QString::number(index+1));
       if(!commands.contains(propName))
           commands.insert(propName,qMakePair(QString(),QString()));
       setProperty(propName.toUtf8(),QVariant::fromValue(commands.value(propName)));
    }
    QRegExp rgxPattern("Command([0-9]{1,2})");
    QListIterator<QByteArray> iter(dynamicPropertyNames());
    while(iter.hasNext()){
        if(rgxPattern.exactMatch(iter.next()) && rgxPattern.cap(1).toInt() > commandsCount())
        {
            setProperty(QString("Command"+rgxPattern.cap(1)).toUtf8(),QVariant());
        }
    }

    qDebug() << dynamicPropertyNames();

}
*/
void SSindicator::debinfo(){
    qDebug() << this->metaObject()->propertyCount();
    qDebug() << static_cast<QObject*>(this)->dynamicPropertyNames();
}

void SSindicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(option->rect.x(),option->rect.y(),image.width(),image.height(),image);
    if(isSelected())
        paintSelectionOutline(painter);
}

QRectF SSindicator::boundingRect() const
{
    return QRect(0,0,image.width(),image.height());
}

QPainterPath SSindicator::shape() const
{
   QPainterPath path;
   path.addRect(image.rect());
   return path;
}
/*
bool SSindicator::saveToFile(QSettings &file)
{

}
*/
void SSindicator::paintSelectionOutline(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    QPainterPath bound;
    bound.addRect(image.rect());
    painter->drawPath(bound);
}

void SSindicator::setTemplateName(const QString &newName)
{
    if (newName != m_TemplateName) {
        m_TemplateName = newName;
        emit dirty();
    }
}

void SSindicator::TemplateNameChanged(const QString& oldName,const QString& newName)
{
    if(oldName==TemplateName())
       setTemplateName(newName);
}

void SSindicator::ImageChanged(const QString& m_TemplateName,const QPixmap &pix)
{
    if(m_TemplateName==TemplateName() && image.cacheKey()!=pix.cacheKey()){
        image = pix;
        emit dirty();
    }
}

void SSindicator::StatesCountChanged(QString m_TemplateName, int value)
{
    if(m_TemplateName==TemplateName()){
        setStatesCount(value);
        emit dirty();
    }
}

QVariant SSindicator::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (isDirtyChange(change))
        emit dirty();
    return QGraphicsItem::itemChange(change, value);
}

void SSindicator::dynPropertyChanged(QByteArray propName){

    if(commands.contains(propName)){
        updateHashDynProperties(commands,propName);
        //qDebug() << "DynPropChanged: " << propName << ": " << value.first << " " << value.second;
    } else if(states.contains(propName)) {
        updateHashDynProperties(states,propName);
    }
}
void SSindicator::updateHashDynProperties(QHash<QString,SScommandProperty>& hash,const QString& propName){
    SScommandProperty value = property(propName.toUtf8()).value<SScommandProperty>();
    if(!value.first.isNull())
        hash[propName].first = value.first;
    if(!value.second.isNull())
        hash[propName].second = value.second;
}

bool SSindicator::event(QEvent *event)
{
    if (event->type() == QEvent::DynamicPropertyChange)
    {
        QDynamicPropertyChangeEvent *const propEvent = static_cast<QDynamicPropertyChangeEvent*>(event);
        QByteArray propName = propEvent->propertyName();
        dynPropertyChanged(propName);
        return true;
    }

    return QGraphicsObject::event(event);
}

QDataStream &operator<<(QDataStream &out, const SSindicator &indicator)
{
    out << indicator.pos()
        << indicator.zValue()
        //<< indicator.rect()
        //<< indicator.model
        << indicator.TemplateName()
        << indicator.image
        << indicator.statesCount()
        << indicator.commandsCount()
        << indicator.states
        << indicator.commands;
    return out;
}


QDataStream &operator>>(QDataStream &in, SSindicator &indicator)
{
    QPointF position;
    double z;
    QRectF rect;
    //StandardTableModel* mod;
    QString TemplName;
    QPixmap pix;

    //карта состояний и команд
    QHash<QString,SScommandProperty> commands,states;
    int statesCount,commandsCount;

    in >> position >> z >> TemplName >> pix >> statesCount >> commandsCount >> states >> commands;

    indicator.setPos(position);
    indicator.setZValue(z);
    //indicator.model = mod;
    indicator.setTemplateName(TemplName);
    indicator.image = pix; 
    indicator.states = states;
    indicator.commands = commands;
    indicator.setStatesCount(statesCount);
    //indicator.updateDynamicPropView(tr("State"),indicator.states,indicator.statesCount());
    indicator.setCommandsCount(commandsCount);

    return in;
}
QSettings &operator<<(QSettings &out, const SSindicator &indicator)
{
   return out;
}

QSettings &operator>>(QSettings &in, SSindicator &indicator)
{
   return in;
}
