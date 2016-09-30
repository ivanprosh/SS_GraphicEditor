#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editmnm.h"
#include <QList>
#include <QGraphicsItem>
#include "PObject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_ACreate_triggered();
    void on_AAddAnPar_triggered(bool checked);


    void on_ACopy_triggered();
    void on_APaste_triggered();
    void on_ADelete_triggered();
    void on_ACut_triggered();
    void on_actionTEST_triggered();
    void on_AAddText_triggered();
    void on_btnAddProp_clicked();
    void on_btnDelProp_clicked();

    void on_AAddButton_triggered();

    void on_AAddIm_triggered();

private:
    Ui::MainWindow *ui;

    QList<QGraphicsItem *> clipboard;

};

#endif // MAINWINDOW_H
