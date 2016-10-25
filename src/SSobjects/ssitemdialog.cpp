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

SSitemdialog::SSitemdialog(const QPoint &position_, QGraphicsScene *scene_,
        QWidget *parent)
    : QDialog(parent), position(position_), scene(scene_)
{
    QStringList initlistNames;
    initlistNames << "Temp1" << "Temp2" << "Temp3";
    model = new StandardTableModel(this,initlistNames);
    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);

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
    QFont Labels(QApplication::font().family(),11,1);

    //элементы для фильтрации и общих настроек
    filterSelectGroupBox = new QGroupBox(tr("Filter"));

    //индекс состояния
    stateIndexLbl = new QLabel(tr("Index state:"));
    stateIndex = new QSpinBox;
    stateIndexLbl->setBuddy(stateIndex);
    stateIndex->setRange(0,32);
    stateIndex->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    //кол-во состояний
    stateCountLbl = new QLabel(tr("Index state:"));
    stateCount = new QSpinBox;
    stateCountLbl->setBuddy(stateIndex);
    stateCount->setRange(0,32);
    stateCount->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

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
    listwdg->setIconSize(QSize(24, 24));
    //lwg.setSelectionMode(QAbstractItemView::MultiSelection);
    listwdg->setViewMode(QListView::IconMode);

    //таблица свойств
    tableView = new QTableView();
    tableView->setModel(proxyModel);
    tableView->setItemDelegate(new ItemDelegate(this));
    tableView->verticalHeader()->setDefaultAlignment(
                Qt::AlignVCenter|Qt::AlignRight);
    //if (item)
        //textEdit->setHtml(item->toHtml());
}


void SSitemdialog::createLayout()
{
    QGridLayout *keyCells = new QGridLayout;
    keyCells->addWidget(stateCountLbl,0,0,Qt::AlignLeft);
    keyCells->setColumnMinimumWidth(1, 10);
    keyCells->addWidget(stateIndexLbl,0,2,Qt::AlignLeft);
    keyCells->addWidget(stateCount,1,0,Qt::AlignLeft);
    keyCells->addWidget(stateIndex,1,2,Qt::AlignLeft);
    filterSelectGroupBox->setLayout(keyCells);

    QVBoxLayout *left = new QVBoxLayout;
    left->addWidget(filterSelectGroupBox);
    left->addWidget(tableView);
    QHBoxLayout *worklayout = new QHBoxLayout;
    worklayout->addLayout(left);
    worklayout->addWidget(listwdg);
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(worklayout);
    mainlayout->addWidget(buttonBox);
    setLayout(mainlayout);
    listwdg->setFocus();
}


void SSitemdialog::createConnections()
{
    //connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateUi()));
    //connect(model, SIGNAL(itemChanged(QStandardItem*)),
    //            this, SLOT(setDirty()));
    connect(stateIndex, SIGNAL(valueChanged(int)),
                this, SLOT(updateUi()));
    connect(stateCount, SIGNAL(valueChanged(int)),
                this, SLOT(stateCountChanged(int)));
    connect(stateCount, SIGNAL(valueChanged(int)),
                model, SLOT(stateCountChanged(int)));
    connect(listwdg, SIGNAL(itemChanged(QListWidgetItem *)),
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

void SSitemdialog::stateCountChanged(int value)
{
    if(stateIndex->value()>value)
        stateIndex->setValue(value);
    stateIndex->setRange(1,value);
}
void SSitemdialog::restoreFilters()
{
    proxyModel->setName(listwdg->selectedItems().size()==1 ?
                            listwdg->currentItem()->text() : QString());
    proxyModel->setState(stateIndex->value());
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

