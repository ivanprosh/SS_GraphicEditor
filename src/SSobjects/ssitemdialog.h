#ifndef SSITEMDIALOG_H
#define SSITEMDIALOG_H

#include <QDialog>
class SSGraphicData;
class QGraphicsScene;
class QListWidget;
class QTableWidget;
class QComboBox;
class QLabel;
class QDialogButtonBox;
class StandardTableModel;
class QGroupBox;
class QSpinBox;
class QTableView;
class ProxyModel;

class SSitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSitemdialog(//QMap<QString,SSGraphicData>& GraphicData,
                          //TextItem *item_=0,
            const QPoint &position_=QPoint(),
            QGraphicsScene *scene_=0, QWidget *parent=0);

    SSGraphicData *GraphicDataItem() const { return item; }

public slots:
    void accept();

private slots:
    void updateUi();
    void stateCountChanged(int value);

private:
    void createWidgets();
    void createLayout();
    void createConnections();
    void restoreFilters();
    //TextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    SSGraphicData *item;
    QPoint position;
    QGraphicsScene *scene;

    //QTableWidget* tableStatewdg;
    //QString originalHtml;
    //QComboBox* TemplateName;
    //виджеты
    QLabel* stateIndexLbl;
    QSpinBox* stateIndex;
    QLabel* stateCountLbl;
    QSpinBox* stateCount;
    QListWidget* listwdg;
    QGroupBox* filterSelectGroupBox;
    //модель/представл. таблицы
    StandardTableModel *model;
    ProxyModel *proxyModel;
    QTableView* tableView;
};


#endif // SSITEMDIALOG_H
