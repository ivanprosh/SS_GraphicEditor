#ifndef PROPMANAGER_H
#define PROPMANAGER_H

#include <QObject>
#include "qtvariantproperty.h"

class QtVariantPropertyManager;
class QtVariantProperty;
class QtProperty;
class QStringList;
class QtAbstractPropertyBrowser;
//class QList;
//class QMap;

class TPropManager : public QtVariantPropertyManager
{
    Q_OBJECT

private:
    QObject *object;
    QStringList ignoreClassNames;
    QtAbstractPropertyBrowser *browser;

    struct brush {
            QVariant value;
            QtVariantProperty *color;
            QtVariantProperty *style;
    };

    QMap<const QtProperty *, brush> propertyToData;
    QMap<const QtProperty *, QtProperty *> ColorToProperty;
    QMap<const QtProperty *, QtProperty *> StyleToProperty;
    QMap<const QMetaObject *, QtProperty *> m_classToProperty;
    QMap<QtProperty *, const QMetaObject *> m_propertyToClass;
    QMap<QtProperty *, QString> propertyToId;
    QMap<QString, QtVariantProperty *> idToProperty;
    QMap<QString, bool> idToExpanded;
    QMap<QtProperty *, int> m_propertyToIndex;
    QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;
    QList<QtProperty *>         m_topLevelProperties;

    void updateExpandState();
private slots:
    void slotValueChanged(QtProperty *property, const QVariant &value);
    void slotPropertyDestroyed(QtProperty *property);
protected:
    virtual void initializeProperty(QtProperty *property);
    virtual void uninitializeProperty(QtProperty *property);
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
};


#endif // PROPMANAGER_H
