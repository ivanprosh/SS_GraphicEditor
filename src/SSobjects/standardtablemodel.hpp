#ifndef STANDARDTABLEMODEL_HPP
#define STANDARDTABLEMODEL_HPP
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

#include <QStandardItemModel>


class StandardTableModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit StandardTableModel(QObject *parent=0,const QStringList& initlistNames=QStringList());

    //QString filename() const { return m_filename; }
    void clear();
    void load(const QString &filename=QString());
    void save(const QString &filename=QString());
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role=Qt::EditRole);
    void addTemplate(const QStringList &TemplateNames);
    void addTemplate(const QString &TemplateName=QString());
    //void addTemplate(const StandardTableModel*);
signals:
    void TemplateNameChanged();
public slots:
    void stateCountChanged(int value);

private:
    const int maxStateValue;
    void initialize();
    //int size;
    QStringList listNames;

    void copyitems(QList<QStandardItem *> &collection);
};

#endif // STANDARDTABLEMODEL_HPP
