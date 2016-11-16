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

#include "../../aqp/aqp.hpp"
#include "../../global.hpp"
#include "standardtablemodel.hpp"
#include "templmodelinfo.h"
#include <QDataStream>
#include <QFile>


namespace {
const qint32 MagicNumber = 0x5A697043;
const qint16 FormatNumber = 100;
}


StandardTableModel::StandardTableModel(QObject *parent,const QStringList& initlistNames)
    : QStandardItemModel(parent),listNames(initlistNames),maxStateValue(32)
{
    initialize();
}


void StandardTableModel::initialize()
{
    QStringList ColumnNames;
    ColumnNames             << "Name"
                            << "StIndex"
                            << "StFrisCycled"
                            << "StFrDelay"
                            << "StFr1"
                            << "StFr2"
                            << "StFr3"
                            << "StFr4"
                            << "StFr5"
                            << "StFr6"
                            << "StFr7"
                            << "StFr8"
                            << "StFr9"
                            << "StFr10"
                            << "StFrTr1"
                            << "StFrTr2"
                            << "StFrTr3"
                            << "StFrTr4"
                            << "StFrTr5"
                            << "StFrTr6"
                            << "StFrTr7"
                            << "StFrTr8"
                            << "StFrTr9"
                            << "StFrTr10";

    setHorizontalHeaderLabels(ColumnNames);

    addTemplate(listNames);
}

void StandardTableModel::addTemplate(const QStringList &TemplateNames)
{
    for (int it = 0; it < TemplateNames.size(); ++it) {
        addTemplate(TemplateNames.at(it));
    }
}
void StandardTableModel::copyitems(QString templateName,int start_row,int count)
{  
    for(int row=start_row;row<start_row+count;row++)
    {
        QList<QStandardItem* > newitems;
        newitems << item(start_row,Name)->clone();
        newitems.last()->setData(templateName,Qt::DisplayRole);
        for(int column=1;column<columnCount();column++){
            newitems<<item(row,column)->clone();
        }
        appendRow(newitems);
    }
}

void StandardTableModel::addTemplate(const QString &TemplateName)
{
    QString curName(TemplateName);
    QList<QStandardItem*> CloneItems = findItems(TemplateName,Qt::MatchExactly,Name);
    if(TemplateName.isEmpty())
        curName = "New Template" + QString::number(rowCount());
    else if(!CloneItems.isEmpty()){
        curName = curName + QString::number(rowCount());
        copyitems(curName,CloneItems.first()->row(),maxStateValue);
    } else {
        for(int curState=1;curState<=maxStateValue;curState++){
            QList<QStandardItem*> items;
            QStandardItem *iName = new QStandardItem;
            iName->setData(curName,Qt::DisplayRole);
            if(curState==1){
                iName->setData(QPixmap(":/images/obj_icons/dp.bmp"),Qt::DecorationRole);
                iName->setData(1,StatesCountRole);
            }
            items << iName;
            QStandardItem *iStateInd = new QStandardItem;
            iStateInd->setData(curState,Qt::DisplayRole);
            items << iStateInd;
            for(int j=StFrisCycled;j<columnCount();j++){
                items << new QStandardItem();
                items.last()->setData(curState,Qt::DisplayRole);
                if(j==StFrisCycled || j>StFr10)
                    items.last()->setCheckable(true);
                items.last()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|
                                       Qt::ItemIsEditable|Qt::ItemIsUserCheckable);
            }
            appendRow(items);
        }
    }
}

void StandardTableModel::deleteTemplate(const QString &TemplateName)
{
    if(TemplateName.isEmpty()) return;
    QList<QStandardItem* > items = findItems(TemplateName,Qt::MatchExactly,Name);
    removeRows(items.at(0)->row(),maxStateValue);
}

void StandardTableModel::clear()
{
    QStandardItemModel::clear();
    initialize();
}


void StandardTableModel::load(const QString &filename)
{
    /*
    if (!filename.isEmpty())
        m_filename = filename;
    if (m_filename.isEmpty())
        throw AQP::Error(tr("no filename specified"));
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
        throw AQP::Error(file.errorString());

    QDataStream in(&file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber)
        throw AQP::Error(tr("unrecognized file type"));
    qint16 formatVersionNumber;
    in >> formatVersionNumber;
    if (formatVersionNumber > FormatNumber)
        throw AQP::Error(tr("file format version is too new"));
    in.setVersion(QDataStream::Qt_4_5);
    clear();

    quint16 zipcode;
    QString postOffice;
    QString county;
    QString state;
    QMap<quint16, QList<QStandardItem*> > itemsForZipcode;

    while (!in.atEnd()) {
        in >> zipcode >> postOffice >> county >> state;
        QList<QStandardItem*> items;
        QStandardItem *item = new QStandardItem;
        item->setData(zipcode, Qt::EditRole);
        items << item;
        foreach (const QString &text, QStringList() << postOffice
                                      << county << state)
            items << new QStandardItem(text);
        itemsForZipcode[zipcode] = items;
    }

    QMapIterator<quint16, QList<QStandardItem*> > i(itemsForZipcode);
    while (i.hasNext())
        appendRow(i.next().value());
     */
}


void StandardTableModel::save(const QString &filename)
{
    /*
    if (!filename.isEmpty())
        m_filename = filename;
    if (m_filename.isEmpty())
        throw AQP::Error(tr("no filename specified"));
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly))
        throw AQP::Error(file.errorString());

    QDataStream out(&file);
    out << MagicNumber << FormatNumber;
    out.setVersion(QDataStream::Qt_4_5);
    for (int row = 0; row < rowCount(); ++row) {
        out << static_cast<quint16>(
                    item(row, Zipcode)->data(Qt::EditRole).toUInt())
            << item(row, PostOffice)->text()
            << item(row, County)->text() << item(row, State)->text();
    }
    */
}

bool StandardTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //if (!index.isValid() || role != (Qt::EditRole | Qt::CheckStateRole))
    if (!index.isValid())
        return false;
    //ZipcodeItem &item = zipcodes[index.row()];
    if (index.column()==Name) {
        QList<QStandardItem *> childs = findItems(value.toString(), Qt::MatchExactly, index.column());
        if(!childs.isEmpty()) return false;
        else if(role==Qt::EditRole){
            QString oldName = index.data().toString();
            QModelIndex it = index;
            while(it.data()==oldName){
                QStandardItemModel::setData(it,value,role);
                //QStandardItemModel::setData(it,oldName,OldNameRole);
                it=QStandardItemModel::index(it.row()+1,index.column());
            }
            emit TemplateNameChanged(oldName,value.toString());
            return true;
        } else if(role==Qt::DecorationRole){
            emit ImageChanged(index.data().toString(),value.value<QPixmap>());
        }
    }
    //emit dataChanged(index, index);
    //return true;
    return QStandardItemModel::setData(index,value,role);
}

void StandardTableModel::stateCountChanged(int value)
{

}
