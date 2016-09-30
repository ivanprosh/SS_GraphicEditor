#include "Prop2Model.h"
#include <QDebug>

\
Prop2Model* Prop2Model::listModel = NULL;

Prop2Model::Prop2Model(QObject *parent)
    :QAbstractTableModel(parent)
{
    _Object = NULL;
}

int Prop2Model::rowCount(const QModelIndex &parent) const
{
    if (_Object==NULL)  return 0;

    return  _Object->Prop2.count();
}

int Prop2Model::columnCount(const QModelIndex &parent) const
{
    if (_Object==NULL)  return 0;

    return _Object->countColProp2;
}

QVariant Prop2Model::data(const QModelIndex &index, int role) const
{

     if(!index.isValid() || _Object == NULL)
                return QVariant();

     //Qt::ItemDataRole
        switch (role) {

        case Qt::EditRole:      // значение для редактирования
        case Qt::DisplayRole:   // // значение для отображения
                {
                    return _Object->Prop2.at(index.row()).item( index.column() );
                }
            break;
        case Qt::BackgroundRole:        // цвет фона ячейки
            return QVariant(QColor( 170 ,  210  + 40* ( index.row() % 2 ) ,  140  + 20 * ( index.row() % 2 )   ));

        default:
            return QVariant();
            break;
        }

}

bool Prop2Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
            return 0;

    switch (role) {
    case Qt::EditRole:
    {
          //PObjectPropertys list = (_Object->Prop2.at(index.row()));    // ТАК НЕ ДОЛЖНО БЫТЬ ЭТО НЕНОРМАЛЬНО
          //   list.setItem(index.column(), value);
          //  _Object->Prop2.replace(index.row(),list);
           _Object->Prop2[index.row()].setItem(index.column(), value);

        }
        break;
    default:
        //QVariant();
        break;
    }
    return 1;
}

QVariant Prop2Model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && _Object->Prop2.count() != 0 && role == Qt::DisplayRole)
          return  _Object->Prop2.at(0).nameItem(section);

      return QAbstractTableModel::headerData(section, orientation, role);
}



bool Prop2Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}



Qt::ItemFlags Prop2Model::flags(const QModelIndex &index) const
{

    if(!index.isValid() ||  _Object == NULL)
            return /*Qt::NoItemFlags | */Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    //return Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ;
}
