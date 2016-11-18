#include "propmanager.h"
#include "global.hpp"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "itemtypes.hpp"
#include <QMetaProperty>
#include <QStringList>
#include <QMap>

TPropManager::TPropManager(QObject *parent, QtAbstractPropertyBrowser *curBrowser):QtVariantPropertyManager(parent),
    browser(curBrowser)
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
         * С учетом наличия динамических свойств (для команд, например),
         * которые не попадают в MetaProperty (их индекс в m_propertyToIndex превышает object->metaObject()->propertyCount()),
         * обработка будет отличной
        */
        if(idx < object->metaObject()->propertyCount()){
            QMetaProperty metaProperty = metaObject->property(idx);
            if (metaProperty.isEnumType()) {
                if (metaProperty.isFlagType())
                    metaProperty.write(object, intToFlag(metaProperty.enumerator(), changedContent.toInt()));
                else
                    metaProperty.write(object, intToEnum(metaProperty.enumerator(), changedContent.toInt()));
            } else {
                metaProperty.write(object, changedContent);
            }
        } else {
            int indexObjectProp = idx - object->metaObject()->propertyCount();
            Q_ASSERT(object->dynamicPropertyNames().size() >= indexObjectProp + 1);
            object->setProperty(object->dynamicPropertyNames().at(indexObjectProp), changedContent);
        }
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

        property->addSubProperty(b.color);
        property->addSubProperty(b.style);

        ColorToProperty[b.color] = property;
        StyleToProperty[b.style] = property;
        propertyToData[property] = b;
    } else if (propertyType(property) == qMetaTypeId<SScommandProperty>()) {
        SScommand prop;

        prop.name = this->addProperty(QVariant::String);
        prop.name->setPropertyName(tr("Name"));
        prop.tag = this->addProperty(QVariant::String);
        prop.tag->setPropertyName(tr("Tag Name"));

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
        return QString(tr("C:")+v.toString());
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

    QtProperty *classProperty = m_classToProperty.value(metaObject);
    if (!classProperty)
        return;

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
}
void TPropManager::addCurObjectDynProperties(){
        //QList<QByteArray> DynPropsList = object->dynamicPropertyNames();
        QListIterator<QByteArray> DynPropIter(object->dynamicPropertyNames());
        QtProperty *classProperty = addProperty(QtVariantPropertyManager::groupTypeId(), tr("Commands"));
        int idx = object->metaObject()->propertyCount();
        while(DynPropIter.hasNext()){
            QtVariantProperty *subProperty = 0;
            QByteArray name = DynPropIter.next();
            subProperty = addProperty(qMetaTypeId<SScommandProperty>(), QLatin1String(name));
            subProperty->setValue(QVariant::fromValue(object->property(name)));
            classProperty->addSubProperty(subProperty);
            m_propertyToIndex[subProperty] = idx;
            m_classToIndexToProperty[object->metaObject()][idx] = subProperty;
            idx++;
        }
        m_topLevelProperties.append(classProperty);
        browser->addProperty(classProperty);
        /*
        QListIterator<QGraphicsItem*> i(items);
        while (i.hasNext()) {
            QScopedPointer<QGraphicsItem> item(i.next());
            scene->removeItem(item.data());
        }
        */
}

void TPropManager::addClassProperties(const QMetaObject* metaObject){

    //const QMetaObject* metaObject = object->metaObject();

    if (!metaObject)
        return;

    addClassProperties(metaObject->superClass());

    QtProperty *classProperty = m_classToProperty.value(metaObject);
    if (!classProperty) {
        QString className = QLatin1String(metaObject->className());
        //Стандартные классы нас не интересуют
        if(ignoreClassNames.contains(className)) return;

        classProperty = addProperty(QtVariantPropertyManager::groupTypeId(), className);
        //addProperty(classProperty, className);
        m_classToProperty[metaObject] = classProperty;
        m_propertyToClass[classProperty] = metaObject;

        //qDebug() <<  "Property count of:" << metaObject->className() << " :" << metaObject->propertyCount();

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
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (Non Writable)"));
                if (!metaProperty.isDesignable())
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()) + QLatin1String(" (Non Designable)"));
                else
                    subProperty = addProperty(type, QLatin1String(metaProperty.name()));
                subProperty->setValue(metaProperty.read(object));
            } else {
                subProperty = addProperty(QVariant::String, QLatin1String(metaProperty.name()));
                subProperty->setValue(QLatin1String("< Unknown Type >"));
                subProperty->setEnabled(false);
            }
            //addProperty(subProperty, QLatin1String(metaProperty.name()));
            classProperty->addSubProperty(subProperty);
            m_propertyToIndex[subProperty] = idx;
            m_classToIndexToProperty[metaObject][idx] = subProperty;
        }      

    } else {
        updateClassProperties(metaObject, false);
    }
    m_topLevelProperties.append(classProperty);
    browser->addProperty(classProperty);
    //QtBrowserItem *item = browser->addProperty(classProperty);
    //проверка
    //if (idToExpanded.contains(id))
    //  dynamic_cast<QtTreePropertyBrowser*>(browser)->setExpanded(item, 0);
}


void TPropManager::itemChanged(QObject *curobject)
{
    ExpandState(&TPropManager::updateExpandState);
    //меняем текущий item
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
    QListIterator<QtProperty *> it(m_topLevelProperties);
    while (it.hasNext()) {
        browser->removeProperty(it.next());
    }
    m_topLevelProperties.clear();

    if(object){
        addClassProperties(object->metaObject());
        if(!object->dynamicPropertyNames().isEmpty())
            addCurObjectDynProperties();
        ExpandState(&TPropManager::SetExpandState);
    }
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
        const QMetaObject* metaObject=m_propertyToClass[item->property()];
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
