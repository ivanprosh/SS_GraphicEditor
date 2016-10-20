#include "propmanager.h"
#include "global.hpp"
#include <QMetaProperty>
#include <QStringList>
#include <QMap>

TPropManager::TPropManager(QObject *parent, QtAbstractPropertyBrowser *curBrowser):QtVariantPropertyManager(parent),
    browser(curBrowser)
{
    ignoreClassNames << "QObject" << "QWidget" << "QGraphicItem";

    connect(this, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
            this, SLOT(slotValueChanged(QtProperty *, const QVariant &)));
    connect(this, SIGNAL(propertyDestroyed(QtProperty *)),
            this, SLOT(slotPropertyDestroyed(QtProperty *)));
}
void TPropManager::slotValueChanged(QtProperty *property, const QVariant &value)
{
    if (ColorToProperty.contains(property)) {
        QtProperty *brushProperty = ColorToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        b.setColor(value.value<QColor>());
        setValue(brushProperty, b);
    } else if(StyleToProperty.contains(property)) {
        QtProperty *brushProperty = StyleToProperty[property];
        QVariant v = this->value(brushProperty);
        QBrush b = v.value<QBrush>();
        //b.setStyle(value.value<QVariant::StringList>().);
        setValue(brushProperty, b);
    }
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
void TPropManager::addClassProperties(const QObject* object,const QMetaObject* metaObject){

    //const QMetaObject* metaObject = object->metaObject();

    if (!metaObject)
        return;

    addClassProperties(object,metaObject->superClass());

    QtProperty *classProperty = m_classToProperty.value(metaObject);
    if (!classProperty) {
        QString className = QLatin1String(metaObject->className());
        //—тандартные классы нас не интересуют
        if(ignoreClassNames.contains(className)) return;

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
            //m_propertyToIndex[subProperty] = idx;
            //m_classToIndexToProperty[metaObject][idx] = subProperty;
        }
    }
    //m_topLevelProperties.append(classProperty);
    browser->addProperty(classProperty);
}

void TPropManager::setValue(QtProperty *property, const QVariant &val)
{
    if (propertyToData.contains(property)) {
            if (val.type() != QVariant::Brush)
                return;

            QBrush curbrush = val.value<QBrush>();
            brush b = propertyToData[property];

            b.color->setValue(curbrush.color());
            //b.style->setValue(curbrush.style());

            propertyToData[property] = b;

            emit propertyChanged(property);
            emit valueChanged(property, curbrush);
            return;
        }
    QtVariantPropertyManager::setValue(property, val);
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
        b.style->setAttribute("enumNames",QStringList() << "NoBrush" << "SolidPattern");
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
        return QString(tr("color %1,type %2)").arg(b.color().name())
                       .arg(QString::number(b.style())));
    }
    return QtVariantPropertyManager::valueText(property);
}
/*
void TPropManager::addProperty(QtVariantProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyEditor->addProperty(property);
    if (idToExpanded.contains(id))
        propertyEditor->setExpanded(item, idToExpanded[id]);
}
*/
