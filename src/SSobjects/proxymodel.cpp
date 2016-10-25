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

#include "../global.hpp"
#include "proxymodel.hpp"


ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    ;//m_minimumZipcode = m_maximumZipcode = InvalidZipcode;
}

QModelIndex ProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (sourceModel()) {
       return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
     } else {
       return QModelIndex();
     }
}

QModelIndex ProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return index(sourceIndex.column(), sourceIndex.row());
}

QModelIndex ProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, (void*) 0);
}

int ProxyModel::rowCount(const QModelIndex &parent) const
{
    return sourceModel() ? sourceModel()->columnCount() : 0;
}

int ProxyModel::columnCount(const QModelIndex &parent) const
{
    return sourceModel() ? sourceModel()->rowCount() : 0;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!sourceModel()) { return QVariant(); }
    Qt::Orientation new_orientation = orientation == Qt::Horizontal ?
                Qt::Vertical : Qt::Horizontal;
    return sourceModel()->headerData(section, new_orientation, role);
}


void ProxyModel::clearFilters()
{
    //m_minimumZipcode = m_maximumZipcode = InvalidZipcode;
    //m_county.clear();
    m_state=1;
    invalidateFilter();
}


bool ProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    /*
    if (m_minimumZipcode != InvalidZipcode ||
        m_maximumZipcode != InvalidZipcode) {
        QModelIndex index = sourceModel()->index(sourceRow, Zipcode,
                                                 sourceParent);
        if (m_minimumZipcode != InvalidZipcode &&
            sourceModel()->data(index).toInt() < m_minimumZipcode)
            return false;
        if (m_maximumZipcode != InvalidZipcode &&
            sourceModel()->data(index).toInt() > m_maximumZipcode)
            return false;
    }
    */
    if (!m_name.isEmpty()) {
        QModelIndex index = sourceModel()->index(sourceRow, 0,
                                                 sourceParent);
        if (m_name != sourceModel()->data(index).toString())
            return false;
    }

    if (m_state!=0) {

        QModelIndex index = sourceModel()->index(sourceRow, 1,
                                                 sourceParent);
        if (m_state != sourceModel()->data(index).toInt())
            return false;
    }
    return true;
}

void ProxyModel::setState(const int &state)
{
    if (m_state != state) {
        m_state = state;
        invalidateFilter();
    }
}
/*
void ProxyModel::setMinimumZipcode(int minimumZipcode)
{
    if (m_minimumZipcode != minimumZipcode) {
        m_minimumZipcode = minimumZipcode;
        invalidateFilter();
    }
}


void ProxyModel::setMaximumZipcode(int maximumZipcode)
{
    if (m_maximumZipcode != maximumZipcode) {
        m_maximumZipcode = maximumZipcode;
        invalidateFilter();
    }
}
*/

void ProxyModel::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        invalidateFilter();
    }
}



