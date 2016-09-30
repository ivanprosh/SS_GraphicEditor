#ifndef QLISTMODEL
#define QLISTMODEL

#include <QAbstractTableModel>
#include "pobjectproperty.h"
#include "PObject.h"



class QListModel: public QAbstractTableModel
{
   Q_OBJECT

private:



    PObject *_Object;

    // QAbstractItemModel interface
public:

    explicit QListModel(QObject *parent = 0);


    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    setObject(PObject *Object)
            {  _Object = Object;
                emit endResetModel();
            }

    static QListModel *listModel;
};


#endif // QLISTMODEL

