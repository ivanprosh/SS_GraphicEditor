/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include "../../global.hpp"
#include "proxymodel.hpp"

ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    ;//m_minimumZipcode = m_maximumZipcode = InvalidZipcode;
}
/*
QModelIndex ProxyModel::parent(const QModelIndex &) const {
  return QModelIndex();
}

QModelIndex ProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (sourceModel()) {
        return sourceModel()->index(proxyIndex.row(),proxyIndex.column());
       //return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
     } else {
       return QModelIndex();
     }
}

QModelIndex ProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    //return index(sourceIndex.column(), sourceIndex.row(), sourceIndex);
    return index(sourceIndex.row(),sourceIndex.column(), sourceIndex);
}

QModelIndex ProxyModel::index(int row, int column, const QModelIndex &) const
{
    //return createIndex(row, column, (void*) 0);
    return createIndex(row,column, (void*) 0);
}

int ProxyModel::rowCount(const QModelIndex &parent) const
{
    //return sourceModel() ? sourceModel()->columnCount() : 0;
    return sourceModel() ? sourceModel()->rowCount() : 0;
}

int ProxyModel::columnCount(const QModelIndex &parent) const
{
    //return sourceModel() ? sourceModel()->rowCount() : 0;
    return sourceModel() ? sourceModel()->columnCount() : 0;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    //if (!sourceModel()) { return QVariant(); }
    //Qt::Orientation new_orientation = orientation == Qt::Horizontal ?
    //            Qt::Vertical : Qt::Horizontal;
    //return sourceModel()->headerData(section, new_orientation, role);

    return sourceModel()->headerData(section, orientation, role);
}
*/

void ProxyModel::clearFilters()
{
    //m_minimumZipcode = m_maximumZipcode = InvalidZipcode;
    //m_county.clear();
    m_CountState=1;
    invalidateFilter();
}

bool ProxyModel::filterAcceptsRow(int source_row,
        const QModelIndex &sourceParent) const
{
    if (!m_name.isEmpty()) {
        QModelIndex index = sourceModel()->index(source_row, 0,
                                                 sourceParent);
        //qDebug() << m_name << " Filter";
        if (m_name != sourceModel()->data(index).toString())
            return false;
    }
    if (m_CountState!=0) {

        //qDebug() << m_CountState << " Filter";
        QModelIndex index = sourceModel()->index(source_row, 1,
                                                 sourceParent);
        if (m_CountState < sourceModel()->data(index).toInt())
            return false;
    }
    //qDebug() << " Filter";
    return true;
}

void ProxyModel::setCountState(const int &CountState)
{
    if (m_CountState != CountState) {
        m_CountState = CountState;
        invalidateFilter();
        //qDebug() << "Invalidate!";
    }
}

void ProxyModel::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        invalidateFilter();
    }
}



