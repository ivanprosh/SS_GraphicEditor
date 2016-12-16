#ifndef PROPMANAGER_H
#define PROPMANAGER_H

#include <QObject>
#include "qtvariantproperty.h"
#include "itemtypes.hpp"

class QtVariantPropertyManager;
class QtVariantProperty;
class QtProperty;
class QStringList;
class QtAbstractPropertyBrowser;
//class SScommandProperty;

enum PropertyStartIndexes {CommandStartIndex = 1000,StateStartIndex=2000};

class TPropManager : public QtVariantPropertyManager
{
    Q_OBJECT

private:

    QObject *object;
    QStringList ignoreClassNames,brushesStyles;
    QtAbstractPropertyBrowser *browser;
    QString prevClassName;
    //const QString commandNamePrefix;
    //const QString stateNamePrefix;

    struct SScommand {
            QVariant value;
            QtVariantProperty *name;
            QtVariantProperty *tag;
    };

    struct brush {
            QVariant value;
            QtVariantProperty *color;
            QtVariantProperty *style;
    };

    //дл€ пользовательского свойства brush
    QMap<const QtProperty *, brush> propertyToData;
    QMap<const QtProperty *, QtProperty *> ColorToProperty;
    QMap<const QtProperty *, QtProperty *> StyleToProperty;
    //дл€ пользовательского свойства command
    QMap<const QtProperty *, SScommand> commandPropertyToData;
    QMap<const QtProperty *, QtProperty *> commandNameToProperty;
    QMap<const QtProperty *, QtProperty *> commandTagToProperty;
    QMap<const QMetaObject *, QList<QtProperty *> > m_classToProperty;
    QMap<QtProperty *, const QMetaObject *> m_propertyToClass;

    QMap<const QMetaObject *, QMap<int, bool> > idToExpanded;
    QMap<QtProperty *, int> m_propertyToIndex;
    QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;
    QList<QtProperty *>         m_topLevelProperties;

    QtBrowserItem* findchildrens(QtBrowserItem *item, QList<QtBrowserItem *> &list);

    /*
     * функци€ поиска в контейнере <QtProperty*> свойства по имени.
     * ¬озвращает указатель на найденное свойство или нулевой указатель
     */
    template<template<typename T> class S>
    QtProperty* findByName(const S<QtProperty*> &sequence, QString name){
        foreach (QtProperty* prop, sequence) {
            if(prop->propertyName()==name) {
                //qDebug() << "addPropertyOrReturnExisted : return " << prop->propertyName();
                return prop;
            }
        }
        return nullptr;
    }
    /*
     * функци€ дл€ добавлени€ в менеджер динамических свойств (верхнего уровн€).
     * ¬ыдел€ет пам€ть под новое свойство, если оно не было добавлено ранее в карту m_classToProperty
     */
    template<template<typename T> class S>
    QtProperty* addPropertyOrReturnExisted(const S<QtProperty*> &sequence, int propType, QString name){
        QtProperty* TopLevelProperty = findByName(sequence,name);
        if(!TopLevelProperty){
            qDebug() << "New addPropertyOrReturnExisted()";
            TopLevelProperty = addProperty(propType, name);

            Q_ASSERT(!m_classToProperty[object->metaObject()].isEmpty());

            if(!findByName(m_classToProperty.value(object->metaObject()),name)){
                m_classToProperty[object->metaObject()].append(TopLevelProperty);
                m_propertyToClass[TopLevelProperty] = object->metaObject();
            }
        }
        return TopLevelProperty;
    }

    void syncDynPropWithObj(QStringList &list, QtProperty *classProperty, int startIndex);
    void topLevelPropertySetVisible(QtProperty *classProperty, bool value = 1);
    void setAttributes(QtVariantProperty *prop);
private slots:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
    void updateExpandState(const QMetaObject* metaObject,int key,QtBrowserItem *subitem);
    void SetExpandState(const QMetaObject* metaObject,int key,QtBrowserItem *subitem);
    void ExpandState(void (TPropManager::*func)(const QMetaObject *, int, QtBrowserItem *));
    void clearData();
public:
    TPropManager(QObject *parent, QtAbstractPropertyBrowser *curBrowser);
    ~TPropManager(){}

    virtual QVariant value(const QtProperty *property) const;
    virtual int valueType(int propertyType) const;
    virtual bool isPropertyTypeSupported(int propertyType) const;

    QString valueText(const QtProperty *property) const;
    void addClassProperties(const QMetaObject* metaObject);
    //friend class QtTreePropertyBrowser;
    void updateClassProperties(const QMetaObject *metaObject, bool recursive);
public slots:
    virtual void setValue(QtProperty *property, const QVariant &val);
    void itemChanged(QObject* curobject);
    void editAddDynamicProperties(const QString& propSingleName, int index);
};


#endif // PROPMANAGER_H
