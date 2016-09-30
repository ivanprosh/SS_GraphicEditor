#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMdiSubWindow>
#include <QGraphicsRectItem>
#include <QClipboard>
#include <QMimeData>
#include "PObject.h"
#include "qlistmodel.h"
#include <QItemSelectionModel>
#include <QStandardItem>
#include "itemdelegate.h"
#include "prop2model.h"


//MainWindow::

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PObjectPropertys::initNamesItems();

    ComboBoxItemDelegate *pDelegate = new ComboBoxItemDelegate(this);
    QListModel *listModel = new QListModel(this);
    Prop2Model *prop2model = new Prop2Model(this);
    ui->tableProperty->setModel(listModel);
    ui->tableProperty2->setModel(prop2model);
    ui->tableProperty->setItemDelegate(pDelegate);
    ui->tableProperty2->setItemDelegate(pDelegate);
    QListModel::listModel = listModel;
    Prop2Model::listModel = prop2model;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ACreate_triggered()
{
   QMdiSubWindow *wnd = ui->mdiArea->addSubWindow(new EditMNM(this));
   wnd->show();

}

// Добавление объекта на сцену
void MainWindow::on_AAddAnPar_triggered(bool checked)
{
    if (ui->mdiArea->activeSubWindow()==NULL) return;
    EditMNM *wnd =
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());

    if (wnd){

        QGraphicsItem *pItem = new PObjectAnPar(20,20, 70,20);
        wnd->scene->addItem(pItem);
           pItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
}


// Копирование объета в буфер обмена
void MainWindow::on_ACopy_triggered()
{
    EditMNM *wnd =  // указатель на активное окно
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());
    if (wnd){
    clipboard = wnd->scene->selectedItems() ;
    //ui->label->setText(QString::number(clipboard.size()));
    }
}

// Втавка элементов
void MainWindow::on_APaste_triggered()
{
    EditMNM *wnd =  // указатель на активное окно
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());
    if (wnd){
       // ui->label->setText(QString::number(clipboard.size()));
        wnd->scene->clearSelection();

        foreach(QGraphicsItem *item, clipboard){
            //QGraphicsItem *pItem = new PObject(item->pos().x(),item->pos().y(), 50,50,PObj::TYPE_OBJECT_ANPAR);
            //wnd->scene->addItem(pItem);
            PObject *pItem = ((PObject*)(item))->ObjectCopy();
            wnd->scene->addItem(pItem);    // создание копии
               pItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
            pItem->setSelected(true);

         }
    }
}

// удаление элементов
void MainWindow::on_ADelete_triggered()
{
    EditMNM *wnd =  // указатель на активное окно
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());
    if (wnd){
        foreach(QGraphicsItem *item, wnd->scene->selectedItems()){
            wnd->scene->removeItem(item);
        }
    }
}

// вырезание элентов (копирование в буффер, удаление)
void MainWindow::on_ACut_triggered()
{
    EditMNM *wnd =  // указатель на активное окно
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());
    if (wnd){

        clipboard = wnd->scene->selectedItems() ;


        foreach(QGraphicsItem *item, wnd->scene->selectedItems()){
            wnd->scene->removeItem(item);
        }
    }
}

void MainWindow::on_actionTEST_triggered()
{
     ui->tableProperty->setModel(QListModel::listModel);
     //ui->tableProperty->

    // ui->tableProperty->update();
}

void MainWindow::on_AAddText_triggered()
{
    if (ui->mdiArea->activeSubWindow()==NULL) return;
    EditMNM *wnd =
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());

    if (wnd){

        QGraphicsItem *pItem = new PObjectTaxt(40,40, 60,20);
        wnd->scene->addItem(pItem);
           pItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
}

void MainWindow::on_btnAddProp_clicked()
{
    PObject::curObject->AddProp2();
    Prop2Model::listModel->setObject(PObject::curObject);
}

void MainWindow::on_btnDelProp_clicked()
{
    QModelIndex  cell = ui->tableProperty2->currentIndex();
    PObject::curObject->DelProp2( cell.row());
    Prop2Model::listModel->setObject(PObject::curObject);
}

void MainWindow::on_AAddButton_triggered()
{
    if (ui->mdiArea->activeSubWindow()==NULL) return;
    EditMNM *wnd =
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());

    if (wnd){

        QGraphicsItem *pItem = new PObjectButton(40,40, 60,24);
        wnd->scene->addItem(pItem);
           pItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
}

void MainWindow::on_AAddIm_triggered()
{
    if (ui->mdiArea->activeSubWindow()==NULL) return;
    EditMNM *wnd =
        qobject_cast<EditMNM*>(ui->mdiArea->activeSubWindow()->widget());

    if (wnd){

        QGraphicsItem *pItem = new PObjectIM(50,50, 50, 50);
        wnd->scene->addItem(pItem);
           pItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    }
}
