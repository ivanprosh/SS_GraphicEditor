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

class TPropManager : public QtVariantPropertyManager
{
    Q_OBJECT

private:
    enum PropertyStartIndexes {CommandStartIndex = 1000,StateStartIndex=2000};
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

    //для пользовательского свойства brush
    QMap<const QtProperty *, brush> propertyToData;
    QMap<const QtProperty *, QtProperty *> ColorToProperty;
    QMap<const QtProperty *, QtProperty *> StyleToProperty;
    //для пользовательского свойства command
    QMap<const QtProperty *, SScommand> commandPropertyToData;
    QMap<const QtProperty *, QtProperty *> commandNameToProperty;
    QMap<const QtProperty *, QtProperty *> commandTagToProperty;
    QMap<const QMetaObject *, QtProperty *> m_classToProperty;
    QMap<QtProperty *, const QMetaObject *> m_propertyToClass;

    //QMap<QtProperty *, QString> propertyToId;
    //QMap<QString, QtVariantProperty *> idToProperty;
    QMap<const QMetaObject *, QMap<int, bool> > idToExpanded;
    QMap<QtProperty *, int> m_propertyToIndex;
    QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;
    QList<QtProperty *>         m_topLevelProperties;

    QtBrowserItem* findchildrens(QtBrowserItem *item, QList<QtBrowserItem *> &list);

    template<template<typename T> class S>
    QtProperty* addPropertyOrReturnExisted(const S<QtProperty*> &sequence, int propType, QString name){
        foreach (QtProperty* prop, sequence) {
            if(prop->propertyName()==name) return prop;
        }
        qDebug() << "New dynprop add";
        return addProperty(propType, name);
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
    void editAddCurObjectCommands();
};


#endif // PROPMANAGER_H
