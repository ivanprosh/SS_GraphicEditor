#ifndef PROXYMODEL_HPP
#define PROXYMODEL_HPP
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


#include <QSortFilterProxyModel>
#include <QAbstractProxyModel>
//#include <QModelIndex>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ProxyModel(QObject *parent=0);
    /*
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex index(int row, int column, const QModelIndex &) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    */
    //int minimumZipcode() const { return m_minimumZipcode; }
    //int maximumZipcode() const { return m_maximumZipcode; }
    QString name() const { return m_name; }
    int CountState() const { return m_CountState; }

public slots:
    void clearFilters();
    //void setMinimumZipcode(int minimumZipcode);
    //void setMaximumZipcode(int maximumZipcode);
    void setName(const QString &name);
    void setCountState(const int &CountState);

protected:
    //bool filterAcceptsColumn(int sourceRow,
    //                      const QModelIndex &sourceParent) const;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
private:
    //int m_minimumZipcode;
    //int m_maximumZipcode;
    QString m_name;
    int m_CountState;
};

#endif // PROXYMODEL_HPP
