#include "ssitemdialog.h"
#include "../ssindicator.h"
//#include "ssgraphobjinfo.h"
#include "templateimage.h"
#include "templmodelinfo.h"
#include "checkboxdelegate.h"
#include "../../aqp/alt_key.hpp"
#include "../../aqp/aqp.hpp"
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
    initializeModel();

    AQP::accelerateWidget(this);
    setWindowTitle(tr("%1 - %2 Indicator Object")
            .arg(QApplication::applicationName())
            .arg(item ? tr("Edit") : tr("Add")));
    updateUi();
}


void SSitemdialog::createWidgets()
{
    //элементы для фильтрации и общих настроек
    filterSelectGroupBox = new QGroupBox(tr("Properties"));
    filterSelectGroupBox->setMaximumWidth(150);
    //изображение
    //QLabel* LabelImage = new QLabel(trUtf8("Шаблон(изображение)"));
    image = new TemplateImage(this);
    image->setMaximumSize(50,50);
    //image.load(":/images/obj_icons/default_obj.png");
    QPixmap pix = QPixmap(":/images/obj_icons/default_obj.png");
    image->setPixmap(pix.scaled(this->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));

//    //индекс состояния
//    stateIndexLbl = new QLabel(tr("Index state:"));
//    stateIndex = new QSpinBox;
//    stateIndexLbl->setBuddy(stateIndex);
//    stateIndex->setRange(1,32);
//    //stateIndex->setValue(1);
//    stateIndex->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    //кол-во состояний
    stateCountLbl = new QLabel(tr("States\ncount:"));
    stateCount = new QSpinBox;
    stateCountLbl->setBuddy(stateCount);
    stateCount->setRange(1,32);
    stateCount->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    stateCount->installEventFilter(this);
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
    //список существующих шаблонов
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
    if(listview->model()->rowCount() > 0){
        listview->setCurrentIndex(listview->model()->index(0,0));
        templateChanged(listview->model()->index(0,0));
    }

    //таблица свойств
    tableView = new QTableView;

    for(int i=Frame1;i<=Frame10;i++)
        tableView->setItemDelegateForColumn(i,new ImageModelDelegate(this));
    //for(int i=StFrTr1;i<=StFrTr10;i++)
    //    tableView->setItemDelegateForColumn(i,new CheckBoxDelegate(this));

    tableView->setModel(proxyModel);
    tableView->verticalHeader()->setDefaultAlignment(
                Qt::AlignVCenter|Qt::AlignRight);
    tableView->resizeColumnsToContents();
    //tableView->resizeRowsToContents();
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->verticalHeader()->setVisible(false);
    //tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //адаптация внешнего вида
    tableView->hideColumn(Name);
    tableView->hideColumn(FrameisCycled);
    tableView->setColumnWidth(Frame1,100);
    //int width = tableView->verticalHeader()->width()+tableView->columnWidth(0);
    //const int count = tableView->model()->columnCount();
    //for(int i=0;i<count;i++)
    //    width += tableView->columnWidth(i);
    //tableView->setFixedSize(width,height);
    //tableView->setFixedWidth(width);
    //tableView->set

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
void SSitemdialog::initializeModel()
{

}


void SSitemdialog::createLayout()
{
    QVBoxLayout *keyCells = new QVBoxLayout;
    keyCells->addWidget(image,0,Qt::AlignCenter);
    keyCells->addWidget(stateCountLbl);
    //keyCells->setColumnMinimumWidth(1, 10);
    //keyCells->addWidget(stateIndexLbl,0,2,Qt::AlignLeft);
    keyCells->addWidget(stateCount);
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
    connect(stateCount, SIGNAL(editingFinished()),
                this, SLOT(stateCountChanged()));

    connect(model, SIGNAL(TemplateNameChanged(QString,QString)),
                    this, SLOT(updateUi()));
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                    this, SLOT(updateUi()));
    connect(listview, SIGNAL(pressed(QModelIndex)),
                this, SLOT(templateChanged(QModelIndex)));

    connect(image, SIGNAL(imageChanged(QPixmap)),this, SLOT(imageChanged(QPixmap)));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(buttonClicked(QAbstractButton *)));
}
void SSitemdialog::stateCountChanged(){

    QString TemplateName = listview->currentIndex().data().toString();

    int curModelStatesCount = listview->model()->data(listview->currentIndex(),StatesCountRole).toInt();

    if(curModelStatesCount==stateCount->value()) return;

    if(AQP::question(this, tr("Change Counts in Template"),
                         tr("All childs will update!"),
                         "", tr("&Yes"), tr("&No")))
    {
        listview->model()->setData(listview->currentIndex(),stateCount->value(),StatesCountRole);
        emit updateStatesCount(TemplateName,stateCount->value());
        updateUi();
    } else {
        stateCount->setValue(curModelStatesCount);
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
void SSitemdialog::deleteTemplate(QString TemplateName){
    if(AQP::question(this, tr("Delete Template"),
                         tr("Delete Template and all Child Items. Attention! Undo/Redo stack will be clean"),
                         "", tr("&Yes"), tr("&No")))
    {
        QList<QGraphicsItem*> items = scene->items();
        QListIterator<QGraphicsItem*> i(items);
        while (i.hasNext()) {
            if(SSindicator* curItem = dynamic_cast<SSindicator*>(i.next())){
                if(curItem->TemplateName()==TemplateName){
                    QScopedPointer<QGraphicsItem> item(curItem);
                    scene->removeItem(item.data());
                }
            }
        }
        model->deleteTemplate(TemplateName);
        QMetaObject::invokeMethod(this->parent(),"clearUndoStack",Qt::QueuedConnection);
        updateUi();
    }
}

void SSitemdialog::buttonClicked(QAbstractButton *sender)
{
    if(sender->text().contains("new template",Qt::CaseInsensitive)){
        model->addTemplate(listview->currentIndex().data().toString());
    } else if(sender->text().contains("delete template",Qt::CaseInsensitive)){
        deleteTemplate(listview->currentIndex().data().toString());
    }
}

void SSitemdialog::updateUi()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            listview->currentIndex().isValid());
    //tableView->resizeColumnsToContents();
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
    //связывание свойств в классе главного окна
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
/*
 фильтр позволяет игнорировать событие нажатия Enter и последующую передачу
 его в окно-родитель. Используется при подтверждении изменения количества состояний
*/
bool SSitemdialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == stateCount && event->type() == QEvent::KeyPress) {
        //listview->setFocus();
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
            listview->setFocus();
            qDebug() << "!Ok";
            return true;
        }
        return false;
    } else {
        // pass the event on to the parent class
        return QDialog::eventFilter(obj, event);
    }
}

