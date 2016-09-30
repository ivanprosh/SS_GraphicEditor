#ifndef PROP2MODEL
#define PROP2MODEL


#include <QAbstractTableModel>
#include "pobjectproperty.h"
#include "PObject.h"
#include "QList"



class Prop2Model: public QAbstractTableModel// QAbstractTableModel QStandardItemModel
{
   Q_OBJECT

private:

    PObject *_Object;

   // QList<QString> *pHeader;

    // QAbstractItemModel interface
public:

    explicit Prop2Model(QObject *parent = 0);


    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

     setObject(PObject *Object)
            {  _Object = Object;
                emit endResetModel();
            }

    static Prop2Model *listModel;

};




#endif // PROP2MODEL

