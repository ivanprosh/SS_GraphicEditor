#include "propmanager.h"
#include "global.hpp"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include <QMetaProperty>
#include <QStringList>
#include <QMap>

TPropManager::TPropManager(QObject *parent, QtAbstractPropertyBrowser *curBrowser):QtVariantPropertyManager(parent),
    browser(curBrowser)
{
    ignoreClassNames << "QObject" << "QWidget" << "QGraphicItem";
    brushesStyles << "No Brush" << "Solid" << "Dense #1" << "Dense #2" << "Dense #3" << "Dense #4" << "Dense #5" << "Dense #6" << "Dense #7"
                  << "Horizontal" << "Vertical" << "Cross";

    connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
            this, SLOT(slotPropertyDestroyed(QtProperty *)));
}
void TPropManager::slotValueChanged(QtProperty *property, const QVariant &value)
{
    const QMetaObject *metaObject = object->metaObject();

    if (ColorToProperty.contains(property)) {
        QtProperty *brushProperty = ColorToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        b.setColor(value.value<QColor>());
        setValue(brushProperty, b);

        QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(brushProperty));
        metaProperty.write(object,b);
    } else if(StyleToProperty.contains(property)) {
        QtProperty *brushProperty = StyleToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        b.setStyle((Qt::BrushStyle)value.toInt());
        setValue(brushProperty, b);

        QMetaProperty metaProperty = metaObject->property(m_propertyToIndex.value(brushProperty));
        metaProperty.write(object,b);
    } else {
        if (!m_propertyToIndex.contains(property))
            return;

        int idx = m_propertyToIndex.value(property);

        QMetaProperty metaProperty = metaObject->property(idx);

        if (metaProperty.isEnumType()) {
            if (metaProperty.isFlagType())
                metaProperty.write(object, intToFlag(metaProperty.enumerator(), value.toInt()));
            else
                metaProperty.write(object, intToEnum(metaProperty.enumerator(), value.toInt()));
        } else {
            metaProperty.write(object, value);
        }
    }

    updateClassProperties(metaObject, true);
}
void TPropManager::slotPropertyDestroyed(QtProperty *property)
{
    if (ColorToProperty.contains(property)) {
        QtProperty *brushProperty = ColorToProperty[property];
        propertyToData[brushProperty].color = 0;
        ColorToProperty.remove(property);
    } else if(StyleToProperty.contains(property)) {
        QtProperty *brushProperty = StyleToProperty[property];
        propertyToData[brushProperty].style = 0;
        StyleToProperty.remove(property);
    }
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
void TPropManager::addClassProperties(const QMetaObject* metaObject){

    //const QMetaObject* metaObject = object->metaObject();

    if (!metaObject)
        return;

    addClassProperties(metaObject->superClass());

    QtProperty *classProperty = m_classToProperty.value(metaObject);
    if (!classProperty) {
        QString className = QLatin1String(metaObject->className());
        //Стандартные классы нас не интересуют
        //if(ignoreClassNames.contains(className)) return;

        classProperty = addProperty(QtVariantPropertyManager::groupTypeId(), className);
        //addProperty(classProperty, className);
        m_classToProperty[metaObject] = classProperty;
        m_propertyToClass[classProperty] = metaObject;

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

void TPropManager::setValue(QtProperty *property, const QVariant &val)
{
    if (propertyToData.contains(property)) {
            if (val.type() != QVariant::Brush)
                return;

            QBrush curbrush = val.value<QBrush>();
            brush b = propertyToData[property];
            b.value = curbrush;
            if(b.color->value().value<QColor>() == curbrush.color()
               && b.style->value().toInt() == (int)curbrush.style()) return;

            if(b.color)
                b.color->setValue(curbrush.color());
            if(b.style)
                b.style->setValue((int)curbrush.style());

            propertyToData[property] = b;

            emit propertyChanged(property);
            emit valueChanged(property, curbrush);
            return;
        }
    QtVariantPropertyManager::setValue(property, val);
}

void TPropManager::itemChanged(QObject *curobject)
{
    //void (*goal)(const QMetaObject* metaObject,int key,QtBrowserItem *subitem);
    //goal = &updateExpandState;
    //запоминаем развернутость узлов
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

    addClassProperties(object->metaObject());
    //Выставляем развернустость узлов
    //goal = SetExpandState;
    ExpandState(&TPropManager::SetExpandState);
}

void TPropManager::initializeProperty(QtProperty *property)
{
    if (propertyType(property) == QVariant::Brush) {
        brush b;
        TPropManager *that = (TPropManager *)this;

        b.color = that->addProperty(QVariant::Color);
        b.color->setPropertyName(tr("Color"));
        property->addSubProperty(b.color);
        ColorToProperty[b.color] = property;

        b.style = that->addProperty(QtVariantPropertyManager::enumTypeId());
        b.style->setPropertyName(tr("Style"));
        b.style->setAttribute("enumNames",brushesStyles);
        //b.style->setValue();
        property->addSubProperty(b.style);
        StyleToProperty[b.style] = property;

        propertyToData[property] = b;
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
    }
    QtVariantPropertyManager::uninitializeProperty(property);
}

QVariant TPropManager::value(const QtProperty *property) const
{
    if (propertyToData.contains(property))
        return propertyToData[property].value;
    return QtVariantPropertyManager::value(property);
}

int TPropManager::valueType(int propertyType) const
{
    if (propertyType == QVariant::Brush)
         return QVariant::Brush;
    return QtVariantPropertyManager::valueType(propertyType);
}

bool TPropManager::isPropertyTypeSupported(int propertyType) const
{
    if (propertyType == QVariant::Brush)
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
    }
    return QtVariantPropertyManager::valueText(property);
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
