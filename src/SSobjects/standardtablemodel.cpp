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

#include "../aqp/aqp.hpp"
#include "../global.hpp"
#include "standardtablemodel.hpp"
#include <QDataStream>
#include <QFile>


namespace {
const qint32 MagicNumber = 0x5A697043;
const qint16 FormatNumber = 100;
}


StandardTableModel::StandardTableModel(QObject *parent,int init_size)
    : QStandardItemModel(parent),size(init_size)
{
    initialize();
}


void StandardTableModel::initialize()
{
    setHorizontalHeaderLabels(QStringList() << tr("Value"));
    QStringList VerticalLabels;
    for(i=1;i<=init_size;i++){
        VerticalLabels << "StateFrameisCycled_" + QString::number(i)
                       << "StateFrameDelay_" + QString::number(i)
                       << "StateFrame1_" + QString::number(i)
                       << "StateFrame2_" + QString::number(i)
                       << "StateFrame3_" + QString::number(i)
                       << "StateFrame4_" + QString::number(i)
                       << "StateFrame5_" + QString::number(i)
                       << "StateFrame6_" + QString::number(i)
                       << "StateFrame7_" + QString::number(i)
                       << "StateFrame8_" + QString::number(i)
                       << "StateFrame9_" + QString::number(i)
                       << "StateFrame10_" + QString::number(i)
                       << "StateFrameTransparent1_" + QString::number(i)
                       << "StateFrameTransparent2_" + QString::number(i)
                       << "StateFrameTransparent3_" + QString::number(i)
                       << "StateFrameTransparent4_" + QString::number(i)
                       << "StateFrameTransparent5_" + QString::number(i)
                       << "StateFrameTransparent6_" + QString::number(i)
                       << "StateFrameTransparent7_" + QString::number(i)
                       << "StateFrameTransparent8_" + QString::number(i)
                       << "StateFrameTransparent9_" + QString::number(i)
                       << "StateFrameTransparent10_" + QString::number(i);
    }
    setVerticalHeaderHeaderLabels(VerticalLabels);
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
