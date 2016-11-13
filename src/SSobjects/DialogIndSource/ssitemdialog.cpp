#include "ssitemdialog.h"
#include "ssindicator.h"
//#include "ssgraphobjinfo.h"
#include "templateimage.h"
#include "templmodelinfo.h"
#include "checkboxdelegate.h"
#include "../aqp/alt_key.hpp"
#include "../aqp/aqp.hpp"
#include "uniqueproxymodel.hpp"
/*
#include "swatch.hpp"
#include "textedit.hpp"
#include "textitem.hpp"
#include "SSitemdialog.hpp"
*/
#include <QAbstractButton>
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

#include "imagemodeldelegate.hpp"
#include "standardtablemodel.hpp"
#include "proxymodel.hpp"
#include <QDebug>

SSitemdialog::SSitemdialog(StandardTableModel* newmodel,const QPoint &position_, QGraphicsScene *scene_,
        QWidget *parent)
    : QDialog(parent), position(position_), scene(scene_), model(newmodel)
{
    //Testmodel = new StandardTableModel(this,QStringList()<<"Temp1"<<"Temp2"<<"Temp3");
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

    //�������� ��� ���������� � ����� ��������
    filterSelectGroupBox = new QGroupBox(tr("Properties"));
    filterSelectGroupBox->setMaximumWidth(150);
    //�����������
    //QLabel* LabelImage = new QLabel(trUtf8("������(�����������)"));
    image = new TemplateImage(this);
    image->setFixedSize(100,100);
    //image.load(":/images/obj_icons/default_obj.png");
    image->setPixmap(QPixmap(":/images/obj_icons/default_obj.png"));

//    //������ ���������
//    stateIndexLbl = new QLabel(tr("Index state:"));
//    stateIndex = new QSpinBox;
//    stateIndexLbl->setBuddy(stateIndex);
//    stateIndex->setRange(1,32);
//    //stateIndex->setValue(1);
//    stateIndex->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    //���-�� ���������
    stateCountLbl = new QLabel(tr("Max state count:"));
    stateCount = new QSpinBox;
    stateCountLbl->setBuddy(stateCount);
    stateCount->setRange(1,32);
    stateCount->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    //stateCount->setValue(1);

    //��� �������
    /*
    TemplateLabel = new QLabel(tr("&Template(name)"));
    TemplateLabel->setFont(Labels);
    TemplateName = new QComboBox();
    TemplateName->setEditable(true);
    TemplateName->setFont(Labels);
    TemplateName->setValidator(new QRegExpValidator(QRegExp("[A-Za-z�-��-�, -]{,50}")));
    TemplateName->setObjectName("Template");
    TemplateLabel->setBuddy(TemplateName);
    */
    //������ ��/������
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|
                                     QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(
            style()->standardIcon(QStyle::SP_DialogApplyButton));
    buttonBox->button(QDialogButtonBox::Ok)->setText("&Create");
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(
            style()->standardIcon(QStyle::SP_DialogCancelButton));
    QPushButton* AddTemplateBtn = new QPushButton("&New Template",this);
    //AddTemplateBtn->setFont(buttonBox->button(QDialogButtonBox::Ok)->font());
    //AddTemplateBtn->setCheckable(true);
    AddTemplateBtn->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    buttonBox->addButton(AddTemplateBtn,QDialogButtonBox::ActionRole);

    QPushButton* DelTemplateBtn = new QPushButton("&Delete Template",this);
    //DelTemplateBtn->setFont(buttonBox->button(QDialogButtonBox::Ok)->font());
    //DelTemplateBtn->setCheckable(true);
    DelTemplateBtn->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    buttonBox->addButton(DelTemplateBtn,QDialogButtonBox::ActionRole);
    //������ ������������ ��������
    listview = new QListView(this);
    listview->setIconSize(QSize(48, 48));
    listview->setSelectionMode(QAbstractItemView::SingleSelection);
    listview->setViewMode(QListView::IconMode);
    listview->setMovement(QListView::Static);
    listview->setSpacing(10);
    //listwdg->setWidth(200);
    //initListTemplates();
    //listview->setCurrentRow(0);
    createUniqListModel(listview,Name);

    //������� �������
    tableView = new QTableView;

    for(int i=StFrTr1;i<=StFrTr10;i++)
        tableView->setItemDelegateForColumn(i,new CheckBoxDelegate(this));
    for(int i=StFr1;i<=StFr10;i++)
        tableView->setItemDelegateForColumn(i,new ImageModelDelegate(this));
    for(int i=StFrTr1;i<=StFrTr10;i++)
        tableView->setItemDelegateForColumn(i,new CheckBoxDelegate(this));

    tableView->setModel(proxyModel);
    tableView->verticalHeader()->setDefaultAlignment(
                Qt::AlignVCenter|Qt::AlignRight);
    tableView->resizeColumnsToContents();
    //tableView->resizeRowsToContents();
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->verticalHeader()->setVisible(false);
    //tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //��������� ��������
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
void SSitemdialog::createUniqListModel(QListView *curlistview, int column)
{
    delete curlistview->model();
    UniqueProxyModel *uniqueProxyModel = new UniqueProxyModel(column,
                                                              this);
    uniqueProxyModel->setDynamicSortFilter(false);
    uniqueProxyModel->setSourceModel(model);
    uniqueProxyModel->sort(column, Qt::AscendingOrder);
    curlistview->setModel(uniqueProxyModel);
    curlistview->setModelColumn(column);
}
void SSitemdialog::initListTemplates()
{
    /*
    QListWidgetItem *Test1 = new QListWidgetItem(listview);
    Test1->setIcon(QIcon(":/images/obj_icons/dd.bmp"));
    Test1->setText("T1");
    Test1->setTextAlignment(Qt::AlignHCenter);
    Test1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QListWidgetItem *Test2 = new QListWidgetItem(listview);
    Test2->setIcon(QIcon(":/images/obj_icons/dp.bmp"));
    Test2->setText("T2");
    Test2->setTextAlignment(Qt::AlignHCenter);
    Test2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    QListWidgetItem *Test3 = new QListWidgetItem(listview);
    Test3->setIcon(QIcon(":/images/obj_icons/dt.bmp"));
    Test3->setText("T3");
    Test3->setTextAlignment(Qt::AlignHCenter);
    Test3->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    */
}


void SSitemdialog::createLayout()
{
    QGridLayout *keyCells = new QGridLayout;
    keyCells->addWidget(image,0,0,Qt::AlignLeft);
    keyCells->addWidget(stateCountLbl,1,0,Qt::AlignLeft);
    keyCells->setColumnMinimumWidth(1, 10);
    //keyCells->addWidget(stateIndexLbl,0,2,Qt::AlignLeft);
    keyCells->addWidget(stateCount,2,0,Qt::AlignLeft);
    //keyCells->addWidget(stateIndex,1,2,Qt::AlignLeft);
    filterSelectGroupBox->setLayout(keyCells);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(filterSelectGroupBox);
    topLayout->addWidget(listview);
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
    listview->setFocus();
}


void SSitemdialog::createConnections()
{
    //connect(stateCount, SIGNAL(valueChanged(int)),
    //            this, SLOT(updateUi()));
    connect(stateCount, SIGNAL(valueChanged(int)),
                this, SLOT(stateCountChanged(int)));

    connect(model, SIGNAL(TemplateNameChanged(QString,QString)),
                    this, SLOT(updateUi()));
    connect(listview, SIGNAL(pressed(QModelIndex)),
                this, SLOT(templateChanged(QModelIndex)));

    connect(image, SIGNAL(imageChanged(QPixmap)),this, SLOT(imageChanged(QPixmap)));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(buttonClicked(QAbstractButton *)));
}
void SSitemdialog::stateCountChanged(int value){

    if(AQP::question(this, tr("Change Counts in Template"),
                     tr("All childs will update!"),
                     "", tr("&Yes"), tr("&No"))) {

        QString TemplateName = listview->currentIndex().data().toString();
        listview->model()->setData(listview->currentIndex(),value,StatesCountRole);
        emit updateStatesCount(TemplateName,value);
        updateUi();
    } else {
        stateCount->setValue(listview->currentIndex().data(StatesCountRole).toInt());
    }
}

void SSitemdialog::templateChanged(const QModelIndex& index){
   image->setPixmap(index.data(Qt::DecorationRole).value<QPixmap>());
   stateCount->setValue(listview->currentIndex().data(StatesCountRole).toInt());
   updateUi();
}

void SSitemdialog::imageChanged(const QPixmap&pix){
   listview->model()->setData(listview->currentIndex(),pix,Qt::DecorationRole);
}

void SSitemdialog::buttonClicked(QAbstractButton *sender)
{
    if(sender->text().contains("new template",Qt::CaseInsensitive)){
        model->addTemplate(listview->currentIndex().data().toString());
    } else if(sender->text().contains("delete template",Qt::CaseInsensitive)){
        model->deleteTemplate(listview->currentIndex().data().toString());
    }
}

void SSitemdialog::updateUi()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            listview->currentIndex().isValid());
    restoreFilters();
}

void SSitemdialog::restoreFilters()
{
    proxyModel->setName(listview->currentIndex().isValid() ?
                            listview->currentIndex().data(Qt::DisplayRole).toString() : QString());
    proxyModel->setCountState(stateCount->value());

    qDebug() << "Listview counts " << listview->model()->rowCount();
    //reportFilterEffect();
}

void SSitemdialog::accept()
{
    if(!listview->currentIndex().isValid())
    {
        QDialog::reject();
        return;
    }

    item = new SSindicator(position, scene,listview->currentIndex());
    //���������� ������� � ������ �������� ����
    //connectItem(item);
    item->update();
    QDialog::accept();
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
