#include "ssitemdialog.h"
#include "ssgraphobjinfo.h"
/*
#include "aqp/alt_key.hpp"
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

SSitemdialog::SSitemdialog(TextItem *item_,
        const QPoint &position_, QGraphicsScene *scene_,
        QWidget *parent)
    : QDialog(parent), item(item_), position(position_), scene(scene_)
{
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
    //Имя шаблона
    TemplateLabel = new QLabel(tr("&Template(name)"));
    TemplateLabel->setFont(Labels);
    TemplateName = new QComboBox();
    TemplateName->setEditable(true);
    TemplateName->setFont(Labels);
    TemplateName->setValidator(new QRegExpValidator(QRegExp("[A-Za-zа-яА-я, -]{,50}")));
    TemplateName->setObjectName("Template");
    TemplateLabel->setBuddy(TemplateName);
    //textEdit = new TextEdit;
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|
                                     QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Ok)->setIcon(
            style()->standardIcon(QStyle::SP_DialogOkButton));
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon(
            style()->standardIcon(QStyle::SP_DialogCancelButton));
    //список существующих шаблонов
    listwdg = new QListWidget(this);
    listwdg.setIconSize(QSize(24, 24));
    //lwg.setSelectionMode(QAbstractItemView::MultiSelection);
    listwdg.setViewMode(QListView::IconMode);

    //таблица свойств
    //if (item)
        //textEdit->setHtml(item->toHtml());
}


void SSitemdialog::createLayout()
{
    QGridLayout *keyCells = new QGridLayout;

    QVBoxLayout *left = new QVBoxLayout;
    left->addLayout(keyCells);
    left->addWidget(tableStatewdg);
    QHBoxLayout *worklayout = new QHBoxLayout;
    worklayout->addLayout(left);
    worklayout->addWidget(listwdg);
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(worklayout);
    mainlayout->addWidget(buttonBox);
    setLayout(mainlayout);
}


void SSitemdialog::createConnections()
{
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(updateUi()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


void SSitemdialog::updateUi()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
            !textEdit->toPlainText().isEmpty());
}


void SSitemdialog::accept()
{
    if (item && !textEdit->document()->isModified()) {
        QDialog::reject();
        return;
    }
    if (!item)
        item = new TextItem(position, scene);
    item->setHtml(textEdit->toHtml());
    item->update();
    QDialog::accept();
}

