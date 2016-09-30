#include "itemdelegate.h"
#include <QComboBox>
#include <QFontComboBox>
#include <QLineEdit>

#include <QtGlobal>
#include <QString>
#include <QDebug>

#include <QApplication>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QDialog>
#include "pobjectproperty.h"


ComboBoxItemDelegate::ComboBoxItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}


ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
}


QWidget* ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
     if(index.data().type() == QMetaType::QColor && index.column() > 0){
        QColorDialog* pDialog = new QColorDialog(parent);//parent, Qt::Dialog | Qt::Window);
        return pDialog;
     }else
     if(index.data().type() == QMetaType::QFont && index.column() > 0){
        QFontDialog* pDialog = new QFontDialog(parent);//parent, Qt::Dialog | Qt::Window);
        return pDialog;
     }else
     if(index.data().type() == QMetaType::QPixmap && index.column() > 0){
         QString dir;
         QFileDialog* pDialog = new QFileDialog(parent, "Open file",dir,
         tr("BMP (*.bmp *.jpeg);;All files (*.*)"));
        return pDialog;
     }
     else
     return QStyledItemDelegate::createEditor(parent, option, index);
}


void ComboBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

    if(QColorDialog* pDialog = qobject_cast<QColorDialog*>(editor)){
        pDialog->setCurrentColor(index.data().value<QColor>());
      }else
    if(QFontDialog* pDialog = qobject_cast<QFontDialog*>(editor)){
        pDialog->setCurrentFont(index.data().value<QFont>());
      }else
    QStyledItemDelegate::setEditorData(editor, index);
}


void ComboBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if(QColorDialog* pDialog = qobject_cast<QColorDialog*>(editor)){
        model->setData(index, pDialog->currentColor());
      }else
    if(QFontDialog* pDialog = qobject_cast<QFontDialog*>(editor)){
        model->setData(index, pDialog->currentFont());
      }else
    if(QFileDialog* pDialog = qobject_cast<QFileDialog*>(editor)){
            QString nameFile = pDialog->selectedFiles().at(0);
            QPixmapU Pixmap(nameFile);
            model->setData(index, Pixmap);
          }
     else
    QStyledItemDelegate::setModelData(editor, model, index);
}


void ComboBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.data().type() == QMetaType::QColor && index.column() > 0){
              painter->setBrush(QBrush(index.data().value<QColor>(), Qt::SolidPattern));
              painter->drawRect(option.rect);
    }else
    if(index.data().type() == QMetaType::QPixmap && index.column() > 0){
                painter->drawPixmap(option.rect, index.data().value<QPixmap>() );
    }
     else
    QStyledItemDelegate::paint(painter, option, index);

}
