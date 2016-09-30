#ifndef EDITMNM_H
#define EDITMNM_H

#include <QWidget>
#include <QGraphicsScene>
#include "editmnm.h"
#include "PObject.h"

namespace Ui {
class EditMNM;
}

class EditMNM : public QWidget
{
    Q_OBJECT

public:
    explicit EditMNM(QWidget *parent = 0);
    ~EditMNM();

QGraphicsScene *scene;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

private slots:


private:
    Ui::EditMNM *ui;


};

#endif // EDITMNM_H


