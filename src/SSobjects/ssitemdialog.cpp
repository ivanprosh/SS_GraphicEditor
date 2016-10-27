#include "ssitemdialog.h"
#include "ssgraphobjinfo.h"
#include "../aqp/alt_key.hpp"
/*
#include "swatch.hpp"
#include "textedit.hpp"
#include "textitem.hpp"
#include "SSitemdialog.hpp"
*/
#include <QApplication>
#include <QDialogButtonBox>
#include <QGraphicsScene>
#include <QPoint>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QStringList>
#include <QTableView>
#include <QHeaderView>

#include "itemdelegate.hpp"
#include "standardtablemodel.hpp"
#include "proxymodel.hpp"
#include <QDebug>

SSitemdialog::SSitemdialog(const QPoint &position_, QGraphicsScene *scene_,
        QWidget *parent)
    : QDialog(parent), position(position_), scene(scene_)
{
    //QStringList initlistNames;
    initlistNames << "Temp1" << "Temp2" << "Temp3";
    model = new StandardTableModel(this,initlistNames);
    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setDynamicSortFilter(true);

    createWidgets();
    createLayout();
    createConnections();

    AQP::accelerateWidget(this);
    setWindowTitle(tr("%1 - %2 Indicator Object")
            .arg(QApplication::applicationName())
            .arg(item ? tr("Edit") : tr("Add")));
    updateUi();
}


void SSitemdialog::createWidgets()
{
    //QFont Labels(QApplication::font().family(),11,1);

    //элементы для фильтрации и общих настроек
    filterSelectGroupBox = new QGroupBox(tr("Filter"));
    filterSelectGroupBox->setMaximumWidth(100);
//    //индекс состояния
//    stateIndexLbl = new QLabel(tr("Index state:"));
//    stateIndex = new QSpinBox;
//    stateIndexLbl->setBuddy(stateIndex);
//    stateIndex->setRange(1,32);
//    //stateIndex->setValue(1);
//    stateIndex->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    //кол-во состояний
    stateCountLbl = new QLabel(tr("Max state count:"));
    stateCount = new QSpinBox;
    stateCountLbl->setBuddy(stateCount);
    stateCount->setRange(1,32);
    stateCount->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    //stateCount->setValue(1);

    //Имя шаблона
    /*
    TemplateLabel = new QLabel(tr("&Template(name)"));
    TemplateLabel->setFont(Labels);
    TemplateName = new QComboBox();
    TemplateName->setEditable(true);
    TemplateName->setFont(Labels);
    TemplateName->setValidator(new QRegExpValidator(QRegExp("[A-Za-zа-яА-я, -]{,50}")));
    TemplateName->setObjectName("Template");
    TemplateLabel->setBuddy(TemplateName);
    */
    //кнопки ок/отмена
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|
                                     QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(
            style()->standardIcon(QStyle::SP_DialogOkButton));
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(
            style()->standardIcon(QStyle::SP_DialogCancelButton));
    //список существующих шаблонов
    listwdg = new QListWidget(this);
    listwdg->setIconSize(QSize(48, 48));
    listwdg->setSelectionMode(QAbstractItemView::SingleSelection);
    listwdg->setViewMode(QListView::IconMode);
    listwdg->setMovement(QListView::Static);
    listwdg->setSpacing(10);
    //listwdg->setWidth(200);
    initListTemplates();
    listwdg->setCurrentRow(0);

    //таблица свойств
    tableView = new QTableView;
    tableView->setModel(proxyModel);
    tableView->setItemDelegate(new ItemDelegate(this));
    tableView->verticalHeader()->setDefaultAlignment(
                Qt::AlignVCenter|Qt::AlignRight);
    tableView->resizeColumnsToContents();
    //tableView->resizeRowsToContents();
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->verticalHeader()->setVisible(false);
    //tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //адаптация размеров
    int width = tableView->verticalHeader()->width()+tableView->columnWidth(0);
    //int height = tableView->horizontalHeader()->height()+10;
    const int count = tableView->model()->columnCount();
    for(int i=0;i<count;i++)
        width += tableView->columnWidth(i);
    //tableView->setFixedSize(width,height);
    tableView->setFixedWidth(width);


    //tableView->viewport()->update();
    //if (item)
    //textEdit->setHtml(item->toHtml());
}

void SSitemdialog::initListTemplates()
{
    QListWidgetItem *Test1 = new QListWidgetItem(listwdg);
    Test1->setIcon(QIcon(":/images/obj_icons/dd.bmp"));
    Test1->setText(initlistNames.at(0));
    Test1->setTextAlignment(Qt::AlignHCenter);
    Test1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QListWidgetItem *Test2 = new QListWidgetItem(listwdg);
    Test2->setIcon(QIcon(":/images/obj_icons/dp.bmp"));
    Test2->setText(initlistNames.at(1));
    Test2->setTextAlignment(Qt::AlignHCenter);
    Test2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QListWidgetItem *Test3 = new QListWidgetItem(listwdg);
    Test3->setIcon(QIcon(":/images/obj_icons/dt.bmp"));
    Test3->setText(initlistNames.at(2));
    Test3->setTextAlignment(Qt::AlignHCenter);
    Test3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


void SSitemdialog::createLayout()
{
    QGridLayout *keyCells = new QGridLayout;
    keyCells->addWidget(stateCountLbl,0,0,Qt::AlignLeft);
    keyCells->setColumnMinimumWidth(1, 10);
    //keyCells->addWidget(stateIndexLbl,0,2,Qt::AlignLeft);
    keyCells->addWidget(stateCount,1,0,Qt::AlignLeft);
    //keyCells->addWidget(stateIndex,1,2,Qt::AlignLeft);
    filterSelectGroupBox->setLayout(keyCells);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(filterSelectGroupBox);
    topLayout->addWidget(listwdg);
    //left->addWidget(tableView);
    QVBoxLayout *worklayout = new QVBoxLayout;
    worklayout->addLayout(topLayout);
    worklayout->addWidget(tableView);
    worklayout->addWidget(buttonBox);
    /*
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(worklayout);
    mainlayout->addWidget(buttonBox);
    */
    setLayout(worklayout);
    listwdg->setFocus();
}


void SSitemdialog::createConnections()
{
    //connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateUi()));
    //connect(model, SIGNAL(itemChanged(QStandardItem*)),
    //            this, SLOT(setDirty()));
//    connect(stateIndex, SIGNAL(valueChanged(int)),
//                this, SLOT(updateUi()));
    connect(stateCount, SIGNAL(valueChanged(int)),
                this, SLOT(updateUi()));
    connect(stateCount, SIGNAL(valueChanged(int)),
                model, SLOT(stateCountChanged(int)));
    connect(listwdg, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                this, SLOT(updateUi()));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


void SSitemdialog::updateUi()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            listwdg->selectedItems().size()==1);
    restoreFilters();
}

//void SSitemdialog::stateCountChanged(int value)
//{
//    if(stateIndex->value()>value)
//        stateIndex->setValue(value);
//    stateIndex->setRange(1,value);
//}
void SSitemdialog::restoreFilters()
{
    proxyModel->setName(listwdg->selectedItems().size()==1 ?
                            listwdg->currentItem()->text() : QString());
    proxyModel->setCountState(stateCount->value());
    //reportFilterEffect();
}

void SSitemdialog::accept()
{
    /*
    if (item && !textEdit->document()->isModified()) {
        QDialog::reject();
        return;
    }
    if (!item)
        item = new TextItem(position, scene);
    item->setHtml(textEdit->toHtml());
    item->update();
    QDialog::accept();
    */
}

