#include "editmnm.h"
#include "ui_editmnm.h"
#include "mainwindow.h"


#include <QGraphicsRectItem>


EditMNM::EditMNM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditMNM)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this->rect());
    ui->graphicsView->setScene(scene);

}

EditMNM::~EditMNM()
{
    delete ui;
}
