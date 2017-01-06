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
#include "imagemodeldelegate.hpp"
#include "proxymodel.hpp"
#include "fileedit.h"
//#include "zipcodespinbox.hpp"
#include <QComboBox>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItemV4>
#include <QFileDialog>
#include <QApplication>

        // adjusted gives a 3 pixel right margin
void ImageModelDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    //QString filename = index.data(Qt::EditRole).toString();
    //QPixmap pix;

    //if(filename.isEmpty() || !pix.load("resource/"+filename)){
    //if(filename.isEmpty()){
    //    pix.load(":/images/obj_icons/default_obj.png");
    //} else if (pix.load("resource/"+filename)){

        /*
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        if(option.state & QStyle::State_Selected)
            painter->fillRect(option.rect,option.palette.highlight());
        float koef = pix.width()/pix.height();
        int x = option.rect.center().x()-option.rect.height()*koef/2;
        painter->drawPixmap(x,option.rect.y(),option.rect.height()*koef,option.rect.height(),pix);
        //
        painter->restore();
        */
    //} else
        QStyledItemDelegate::paint(painter,option,index);
}


QWidget *ImageModelDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{
    FileEdit* editor = new FileEdit(parent);
    editor->setFilter("*.bmp");

    return editor;
    //return QStyledItemDelegate::createEditor(parent, option, index);
}


void ImageModelDelegate::setEditorData(QWidget *editor,
        const QModelIndex &index) const
{
    FileEdit* fileEditor = qobject_cast<FileEdit*>(editor);
    QString fileName = index.model()->data(index).toString();
    fileEditor->setFilePath(fileName);
    /*
    if (index.column() == Zipcode) {
        int value = index.model()->data(index).toInt();
        ZipcodeSpinBox *spinBox =
                qobject_cast<ZipcodeSpinBox*>(editor);
        Q_ASSERT(spinBox);
        spinBox->setValue(value);
    }
    else if (index.column() == State) {
        QString state = index.model()->data(index).toString();
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(comboBox);
        comboBox->setCurrentIndex(comboBox->findText(state));
    }
    else
    */
     //   QStyledItemDelegate::setEditorData(editor, index);
}


void ImageModelDelegate::setModelData(QWidget *editor,
        QAbstractItemModel *model, const QModelIndex &index) const
{
    FileEdit* fileEditor = qobject_cast<FileEdit*>(editor);
    model->setData(index,fileEditor->filePath());
    QPixmap pix;
    if (pix.load("resource/"+fileEditor->filePath()))
        model->setData(index,pix,Qt::DecorationRole);
    //QStyledItemDelegate::setModelData(editor, model, index);
}
