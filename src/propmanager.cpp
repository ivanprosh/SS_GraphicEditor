#include "propmanager.h"
#include "global.hpp"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "itemtypes.hpp"
#include "aqp.hpp"
#include <QMetaProperty>
#include <QStringList>
#include <QMap>

TPropManager::TPropManager(QObject *parent, QtAbstractPropertyBrowser *curBrowser):QtVariantPropertyManager(parent),
    browser(curBrowser),prevClassName()
{
    ignoreClassNames << "QWidget" << "QGraphicItem";
    brushesStyles << "No Brush" << "Solid" << "Dense #1" << "Dense #2" << "Dense #3" << "Dense #4" << "Dense #5" << "Dense #6" << "Dense #7"
                  << "Horizontal" << "Vertical" << "Cross";

    connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
            this, SLOT(slotPropertyDestroyed(QtProperty *)));
}

void TPropManager::slotValueChanged(QtProperty *property, const QVariant &changedContent)
{
    const QMetaObject *metaObject = object->metaObject();

    if (ColorToProperty.contains(property)) {
        QtProperty *brushProperty = ColorToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        b.setColor(changedContent.value<QColor>());
        setValue(brushProperty, b);

        //QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(brushProperty));
        //metaProperty.write(object,b);
    } else if(StyleToProperty.contains(property)) {
        QtProperty *brushProperty = StyleToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        b.setStyle((Qt::BrushStyle)changedContent.toInt());
        setValue(brushProperty, b);

        //QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(brushProperty));
        //metaProperty.write(object,b);
    } else if(commandNameToProperty.contains(property)) {
        QtProperty *parentProperty = commandNameToProperty[property];
        QVariant v = this->value(parentProperty);
        QStringList values = v.toString().split("::");
        Q_ASSERT(!values.isEmpty());
        SScommandProperty valueParentProperty;
        valueParentProperty.first = changedContent.toString();
        valueParentProperty.second = values.last();

        setValue(parentProperty, QVariant::fromValue(valueParentProperty));

        //QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(parentProperty));
        //metaProperty.write(object,QVariant::fromValue(valueParentProperty));

    } else if(commandTagToProperty.contains(property)) {
        QtProperty *parentProperty = commandTagToProperty[property];
        QVariant v = this->value(parentProperty);
        QStringList values = v.toString().split("::");
        Q_ASSERT(!values.isEmpty());
        SScommandProperty valueParentProperty;
        valueParentProperty.first = values.first();
        valueParentProperty.second = changedContent.toString();

        setValue(parentProperty, QVariant::fromValue(valueParentProperty));

        //QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(parentProperty));
        //metaProperty.write(object,QVariant::fromValue(valueParentProperty));

    } else {
        if (!m_propertyToIndex.contains(property))
            return;

        int idx = m_propertyToIndex.value(property);

        /*
         * — учетом наличи€ динамических свойств (дл€ команд, например),
         * которые не попадают в MetaProperty (их индекс в m_propertyToIndex превышает object->metaObject()->propertyCount()),
         * обработка будет отличной
        */
        //if(metaProperty.isWritable()){
        if(idx < object->metaObject()->propertyCount()){
            QMetaProperty metaProperty = metaObject->property(idx);
            if(metaProperty.isWritable()){
                if (metaProperty.isEnumType()) {
                    if (metaProperty.isFlagType())
                        metaProperty.write(object, intToFlag(metaProperty.enumerator(), changedContent.toInt()));
                    else
                        metaProperty.write(object, intToEnum(metaProperty.enumerator(), changedContent.toInt()));
                } else {
                    metaProperty.write(object, changedContent);
                }
            }

        } else {
            //int indexObjectProp = idx - CommandStartIndex;
            Q_ASSERT(!object->dynamicPropertyNames().isEmpty());
            object->setProperty(m_classToIndexToProperty[object->metaObject()][idx]->propertyName().toUtf8(), changedContent);
        }
        //}
    }

    //updateClassProperties(metaObject, true);
}
void TPropManager::slotPropertyDestroyed(QtProperty *parentProperty)
{
    if (ColorToProperty.contains(parentProperty)) {
        QtProperty *brushProperty = ColorToProperty[parentProperty];
        propertyToData[brushProperty].color = 0;
        ColorToProperty.remove(parentProperty);
    } else if(StyleToProperty.contains(parentProperty)) {
        QtProperty *brushProperty = StyleToProperty[parentProperty];
        propertyToData[brushProperty].style = 0;
        StyleToProperty.remove(parentProperty);
    } else if(commandNameToProperty.contains(parentProperty)) {
        QtProperty *curProperty = commandNameToProperty[parentProperty];
        commandPropertyToData[curProperty].name = 0;
        commandNameToProperty.remove(parentProperty);
    } else if(commandTagToProperty.contains(parentProperty)) {
        QtProperty *curProperty = commandTagToProperty[parentProperty];
        commandPropertyToData[curProperty].tag = 0;
        commandTagToProperty.remove(parentProperty);
    }
}
void TPropManager::initializeProperty(QtProperty *property)
{
    if (propertyType(property) == QVariant::Brush) {
        brush b;

        b.color = this->addProperty(QVariant::Color);
        b.color->setPropertyName(tr("Color"));   
        b.style = this->addProperty(QtVariantPropertyManager::enumTypeId());
        b.style->setPropertyName(tr("Style"));
        b.style->setAttribute("enumNames",brushesStyles);
		qDebug() << "New QVariant::Brush";

        property->addSubProperty(b.color);
        property->addSubProperty(b.style);

        ColorToProperty[b.color] = property;
        StyleToProperty[b.style] = property;
        propertyToData[property] = b;
    } else if (propertyType(property) == qMetaTypeId<SScommandProperty>()) {
        SScommand prop;

        prop.name = this->addProperty(QVariant::String);
        prop.name->setPropertyName(tr("Value"));
        prop.name->setWhatsThis("«десь будет справка");
        prop.tag = this->addProperty(QVariant::String);
        prop.tag->setPropertyName(tr("Tag"));
		qDebug() << "New qMetaTypeId<SScommandProperty>()";
		
        property->addSubProperty(prop.name);
        property->addSubProperty(prop.tag);
        commandNameToProperty[prop.name] = property;
        commandTagToProperty[prop.tag] = property;
        commandPropertyToData[property] = prop;
    }
    QtVariantPropertyManager::initializeProperty(property);
}

void TPropManager::uninitializeProperty(QtProperty *property)
{
    if (propertyToData.contains(property)) {
        brush b = propertyToData[property];
        if (b.color)
            ColorToProperty.remove(b.color);
        if (b.style)
            StyleToProperty.remove(b.style);
        propertyToData.remove(property);
    } else if(commandPropertyToData.contains(property)) {
        SScommand prop = commandPropertyToData[property];
        if (prop.name)
            commandNameToProperty.remove(prop.name);
        if (prop.tag)
            commandTagToProperty.remove(prop.tag);
        commandPropertyToData.remove(property);
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}

QVariant TPropManager::value(const QtProperty *property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    if (commandPropertyToData.contains(property))
        return commandPropertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}

int TPropManager::valueType(int propertyType) const
{
    if (propertyType == QVariant::Brush)
         return QVariant::Brush;
    if (propertyType == qMetaTypeId<SScommandProperty>())
         return qMetaTypeId<SScommandProperty>();
    return QtVariantPropertyManager::valueType(propertyType);
}

bool TPropManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::Brush)
            return true;
    if (propertyType == qMetaTypeId<SScommandProperty>())
            return true;
    return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

QString TPropManager::valueText(const QtProperty *property) const
{
    if (propertyToData.contains(property)) {
        QVariant v = propertyToData[property].value;
        QBrush b = v.value<QBrush>();
        return QString(tr("color %1,%2)").arg(b.color().name())
                       .arg(brushesStyles.at(b.style())));
    } else if(commandPropertyToData.contains(property)) {
        QVariant v = commandPropertyToData[property].value;
        return QString(v.toString());
    }
    return QtVariantPropertyManager::valueText(property);
}

void TPropManager::setValue(QtProperty *property, const QVariant &newContentValue)
{
    if (propertyToData.contains(property)) {
        if (newContentValue.type() != QVariant::Brush)
            return;

        QBrush curbrush = newContentValue.value<QBrush>();
        brush& b = propertyToData[property];
        b.value = curbrush;
        //if(b.color->value().value<QColor>() == curbrush.color()
        //        && b.style->value().toInt() == (int)curbrush.style()) return;

        if(b.color)
            b.color->setValue(curbrush.color());
        if(b.style)
            b.style->setValue((int)curbrush.style());

        //propertyToData[property] = b;

        emit propertyChanged(property);
        emit valueChanged(property, curbrush);
        return;
    }
    if (commandPropertyToData.contains(property)) {

        if (propertyType(property) != qMetaTypeId<SScommandProperty>())
            return;

        SScommandProperty newData = newContentValue.value<SScommandProperty>();
        SScommand& context = commandPropertyToData[property];

        context.value = newData.first + "::" + newData.second;

        if(context.name){
            context.name->setValue(newData.first);
        }
        if(context.tag) {
            context.tag->setValue(newData.second);
        }

        emit propertyChanged(property);
        emit valueChanged(property,QVariant::fromValue(newData));

        return;
    }
    QtVariantPropertyManager::setValue(property, newContentValue);
}

void TPropManager::updateClassProperties(const QMetaObject *metaObject, bool recursive)
{
    if (!metaObject)
        return;

    if (recursive)
        updateClassProperties(metaObject->superClass(), recursive);

    if(m_classToProperty.value(metaObject).isEmpty()) return;

    //foreach (QtProperty *TopLevelProperty, m_classToProperty.value(metaObject))

    QtProperty *TopLevelProperty = m_classToProperty.value(metaObject).first();

    for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++) {
        QMetaProperty metaProperty = metaObject->property(idx);
        if (metaProperty.isReadable()) {
            if (m_classToIndexToProperty.contains(metaObject) && m_classToIndexToProperty[metaObject].contains(idx)) {
                QtVariantProperty *subProperty = m_classToIndexToProperty[metaObject][idx];
                if (metaProperty.isEnumType()) {
                    if (metaProperty.isFlagType())
                        subProperty->setValue(flagToInt(metaProperty.enumerator(), metaProperty.read(object).toInt()));
                    else
                        subProperty->setValue(enumToInt(metaProperty.enumerator(), metaProperty.read(object).toInt()));
                } else {
                    subProperty->setValue(metaProperty.read(object));
                }
            }
        }
    }
    foreach (QtProperty *TopLevelProperty, m_classToProperty.value(metaObject)) {
        topLevelPropertySetVisible(TopLevelProperty);
    }

}

void TPropManager::syncDynPropWithObj(QStringList& list,QtProperty *classProperty, int startIndex){

    QStringListIterator DynPropIter(list);
    QRegExp rgxPattern(".*([0-9]{1,2})");
    //qDebug() << "Filtered : " << list;

    while(DynPropIter.hasNext()){
        QString name(DynPropIter.next());
        if(rgxPattern.exactMatch(name)){
            QtVariantProperty *subProperty;
            int idx = startIndex+rgxPattern.cap(1).toInt();
            if(!m_classToIndexToProperty[object->metaObject()][idx]){
                subProperty = addProperty(qMetaTypeId<SScommandProperty>(), name);
                qDebug() << "New : " << "syncDynPropWithObj(), idx " << idx;
                m_propertyToIndex[subProperty] = idx;
                m_classToIndexToProperty[object->metaObject()][idx] = subProperty;
            } else {
                subProperty = m_classToIndexToProperty[object->metaObject()][idx];
            }
            classProperty->addSubProperty(subProperty);
            subProperty->setValue(QVariant::fromValue(object->property(name.toUtf8())));
        }
        //idx++;
    }
    topLevelPropertySetVisible(classProperty);
}
void TPropManager::editAddDynamicProperties(const QString& propSingleName, int index){

    QStringList actualCommandsNameList = AQP::filterString(object->dynamicPropertyNames(),propSingleName);
    //у объекта нет динамических свойств с заданным именем
    if(actualCommandsNameList.isEmpty()){
        //qDebug() << object->dynamicPropertyNames();
        if(!m_topLevelProperties.isEmpty())
            foreach (QtProperty* prop, m_topLevelProperties) {
                if(prop->propertyName().contains(propSingleName))
                {
                    browser->removeProperty(prop);
                    //m_topLevelProperties.removeOne(prop);
                }
            }
        return;
    }

    QtProperty *TopLevelProperty = addPropertyOrReturnExisted(m_topLevelProperties,QtVariantPropertyManager::groupTypeId(), propSingleName);
    //if(m_topLevelProperties.contains())
    //= addPropertyOrReturnExisted(m_topLevelProperties,QtVariantPropertyManager::groupTypeId(), propSingleName);
    foreach (QtProperty *prop, TopLevelProperty->subProperties()) {
        TopLevelProperty->removeSubProperty(prop);
    }
    topLevelPropertySetVisible(TopLevelProperty,false);

    Q_ASSERT(!browser->properties().contains(TopLevelProperty) &&
             !m_topLevelProperties.contains(TopLevelProperty));

    syncDynPropWithObj(actualCommandsNameList,TopLevelProperty,index);
}
void TPropManager::setAttributes(QtVariantProperty *prop){
    if(prop->propertyName() == "commandsCount" || prop->propertyName() == "statesCount"){
        prop->setAttribute(QLatin1String("minimum"), 0);
        prop->setAttribute(QLatin1String("maximum"), 32);
        //prop->setAttribute(QLatin1String("readOnly"), true);
    }
}

void TPropManager::addClassProperties(const QMetaObject* metaObject){

    //const QMetaObject* metaObject = object->metaObject();

    if (!metaObject)
        return;

    addClassProperties(metaObject->superClass());

    QtProperty *TopLevelProperty;// m_classToProperty.value(metaObject).first();
    if (m_classToProperty.value(metaObject).isEmpty()) {
        QString className = QLatin1String(metaObject->className());
        //—тандартные классы нас не интересуют
        if(ignoreClassNames.contains(className)) return;

        TopLevelProperty = addProperty(QtVariantPropertyManager::groupTypeId(), className);
        qDebug() << "New addClassProperties() classProperty is " << TopLevelProperty->propertyName();
        //addProperty(classProperty, className);
        m_classToProperty[metaObject].append(TopLevelProperty);
        m_propertyToClass[TopLevelProperty] = metaObject;

        //qDebug() <<  "Property name:" << classProperty->propertyName() << " :m_propertyToClass.size() " << m_propertyToClass.size();

        for (int idx = metaObject->propertyOffset(); idx < metaObject->propertyCount(); idx++) {
            QMetaProperty metaProperty = metaObject->property(idx);
            int type = metaProperty.userType();
            QtVariantProperty *subProperty = 0;
            if (!metaProperty.isReadable()) {
                subProperty = addProperty(QVariant::String, QLatin1String(metaProperty.name()));
                subProperty->setValue(QLatin1String("< Non Readable >"));
            } else if (metaProperty.isEnumType()) {
                if (metaProperty.isFlagType()) {
                    subProperty = addProperty(QtVariantPropertyManager::flagTypeId(), QLatin1String(metaProperty.name()));
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap;
                    QStringList flagNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value) && isPowerOf2(value)) {
                            valueMap[value] = true;
                            flagNames.append(QLatin1String(metaEnum.key(i)));
                        }
                        subProperty->setAttribute(QLatin1String("flagNames"), flagNames);
                        subProperty->setValue(flagToInt(metaEnum, metaProperty.read(object).toInt()));
                    }
                } else {
                    subProperty = addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String(metaProperty.name()));
                    QMetaEnum metaEnum = metaProperty.enumerator();
                    QMap<int, bool> valueMap; // dont show multiple enum values which have the same values
                    QStringList enumNames;
                    for (int i = 0; i < metaEnum.keyCount(); i++) {
                        int value = metaEnum.value(i);
                        if (!valueMap.contains(value)) {
                            valueMap[value] = true;
                            enumNames.append(QLatin1String(metaEnum.key(i)));
                        }
                    }
                    subProperty->setAttribute(QLatin1String("enumNames"), enumNames);
                    subProperty->setValue(enumToInt(metaEnum, metaProperty.read(object).toInt()));
                }
            } else if (isPropertyTypeSupported(type)) {
                if (!metaProperty.isWritable())
                {
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (R) "));
                    subProperty->setAttribute(QLatin1String("readOnly"), true);
                } else if (!metaProperty.isDesignable())
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (Non Designable)"));
                else
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()));
                subProperty->setValue(metaProperty.read(object));               
            } else {
                continue;
                /*
                subProperty = addProperty(QVariant::String, QLatin1String(metaProperty.name()));
                subProperty->setValue(QLatin1String("< Unknown Type >"));
                subProperty->setEnabled(false);
                */
            }
            //addProperty(subProperty, QLatin1String(metaProperty.name()));
            //qDebug() << subProperty->propertyName();
            setAttributes(subProperty);
            TopLevelProperty->addSubProperty(subProperty);
            m_propertyToIndex[subProperty] = idx;
            m_classToIndexToProperty[metaObject][idx] = subProperty;
        }      

    } else {
        TopLevelProperty = m_classToProperty.value(metaObject).first();
        updateClassProperties(metaObject, false);
    }
    topLevelPropertySetVisible(TopLevelProperty);

    //QtBrowserItem *item = browser->addProperty(classProperty);
    //проверка
    //if (idToExpanded.contains(id))
    //  dynamic_cast<QtTreePropertyBrowser*>(browser)->setExpanded(item, 0);
}

void TPropManager::clearData(){
    QListIterator<QtProperty *> it(m_topLevelProperties);
    while (it.hasNext()) {
        browser->removeProperty(it.next());
    }
    m_topLevelProperties.clear();
    //m_propertyToIndex.clear();
}

void TPropManager::itemChanged(QObject *curobject)
{
    ExpandState(&TPropManager::updateExpandState);

    //мен€ем текущий item
    object=curobject;
    /*
    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();
    */
    //clearData();


    if(!m_propertyToClass.isEmpty())
        qDebug() << "Leak " << "m_propertyToClass.size() = " << m_propertyToClass.size();
    if(!m_propertyToIndex.isEmpty())
        qDebug() << "Leak " << "m_propertyToIndex.size() = " << m_propertyToIndex.size();

    clearData();

    if(object){       
        //≈сли переключились на элемент того же типа, то нет необходимости удал€ть все отображение,
        //достаточно лишь обновить данные
        if(prevClassName == object->metaObject()->className() ||
                m_classToIndexToProperty.contains(object->metaObject())){
            updateClassProperties(object->metaObject(), true);
        } else {
            addClassProperties(object->metaObject());
        }
        if(!object->dynamicPropertyNames().isEmpty()){
            editAddDynamicProperties(tr("State"),StateStartIndex);
            editAddDynamicProperties(tr("Command"),CommandStartIndex);
        }
        ExpandState(&TPropManager::SetExpandState);

        prevClassName = object->metaObject()->className();
    }
}
void TPropManager::topLevelPropertySetVisible(QtProperty *TopLevelProperty,bool value){
    if(value ^ m_topLevelProperties.contains(TopLevelProperty))
        value ? m_topLevelProperties.append(TopLevelProperty) : m_topLevelProperties.removeOne(TopLevelProperty);
    if(value ^ browser->properties().contains(TopLevelProperty))
        value ? browser->addProperty(TopLevelProperty) : browser->removeProperty(TopLevelProperty);
}

void TPropManager::SetExpandState(const QMetaObject* metaObject, int key, QtBrowserItem *subitem)
{
    dynamic_cast<QtTreePropertyBrowser*>(browser)->setExpanded(subitem, idToExpanded[metaObject].value(key));
}
QtBrowserItem* TPropManager::findchildrens(QtBrowserItem *item,QList<QtBrowserItem *>& list)
{
    foreach (QtBrowserItem *child, item->children()) {
       list.append(findchildrens(child,list));
    }
    return item;
}
void TPropManager::updateExpandState(const QMetaObject* metaObject, int key, QtBrowserItem *subitem)
{
    idToExpanded[metaObject][key] = (dynamic_cast<QtTreePropertyBrowser*>(browser))->isExpanded(subitem);
}

void TPropManager::ExpandState(void (TPropManager::*func)(const QMetaObject* ,int ,QtBrowserItem*))
{
    QList<QtBrowserItem *> list = browser->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        const QMetaObject* metaObject=m_propertyToClass.value(item->property());
        int count = 0;

        QList<QtBrowserItem *> sublist;
        findchildrens(item,sublist);

        QListIterator<QtBrowserItem *> subit(sublist);
        while (subit.hasNext()) {
            QtBrowserItem *subitem = subit.next();
            QtProperty *prop = subitem->property();

            QMap<int, QtVariantProperty *>::const_iterator i = m_classToIndexToProperty[metaObject].constBegin();
            while (i != m_classToIndexToProperty[metaObject].constEnd()) {
                if(m_classToIndexToProperty[metaObject].value(i.key())==prop){
                    //idToExpanded[metaObject][i.key()] = (dynamic_cast<QtTreePropertyBrowser*>(browser))->isExpanded(subitem);
                    int key = i.key();
                    (this->*func)(metaObject,key,subitem);
                }
                ++i;
                ++count;
            }

        }
        (this->*func)(metaObject,count,item);
     }
}
