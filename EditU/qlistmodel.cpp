#include "qlistmodel.h"
#include <QDebug>
#include  <QtCore/qmath.h>

\
QListModel* QListModel::listModel = NULL;

QListModel::QListModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    _Object = NULL;
}

int QListModel::rowCount(const QModelIndex &parent) const
{
    if (_Object==NULL)  return 0;

    return  _Object->countRow();
}

int QListModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant QListModel::data(const QModelIndex &index, int role) const
{

     if(!index.isValid() || _Object == NULL)
                return QVariant();

     //Qt::ItemDataRole
        switch (role) {

        case Qt::EditRole:      // значение для редактирования
        case Qt::DisplayRole:   // // значение для отображения

            if(index.column() == 0 ){
                //PObjectProperty pProp =  _Object->ObjectPropertys.nameItem(index.row());
                return  _Object->ObjectPropertys.nameItem(index.row());

            }
            else
            {

                return _Object->ObjectPropertys.item(index.row());
            }
            break;
        case Qt::BackgroundRole:        // цвет фона ячейки
            return QVariant(QColor( 170 ,  170  + 10* ( index.row() % 2 ) ,  200  + 20 * ( index.row() % 2 )   ));


        default:
            return QVariant();
            break;
        }



}

bool QListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
            return 0;

    switch (role) {
    case Qt::EditRole:
        if(index.column() != 0 ){
            //QVariant pProp = _Object->ObjectPropertys.item(index.row());
            //pProp.value = value;
            if(_Object->typeObject() == PObj::TYPE_OBJECT_IM && index.row() == 5) //Число состояний объекта
            {
                int n = (int)qPow( 2,  value.toReal());

                    while ( n  > _Object->Prop2.count() )
                       _Object->AddProp2();

                   while ( n  < _Object->Prop2.count() )
                      _Object->DelProp2(_Object->Prop2.count() - 1);
            }

            _Object->ObjectPropertys.setItem(index.row(), value );
            //qDebug()<< value.toString();
            return 1;//_ListObjectPropertys->at(index.row()).value;

        }
        break;
    default:
        //QVariant();
        break;
    }

}


Qt::ItemFlags QListModel::flags(const QModelIndex &index) const
{

    if(!index.isValid() || index.column() == 0 || _Object == NULL)
            return /*Qt::NoItemFlags | */Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    //return Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ;
}
