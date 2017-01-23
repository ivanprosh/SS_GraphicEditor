#ifndef SSITEMDIALOG_H
#define SSITEMDIALOG_H

#include <QDialog>
class SSindicator;
class QGraphicsScene;
class QListView;
class QTableWidget;
class QComboBox;
class QLabel;
class QDialogButtonBox;
class StandardTableModel;
class QGroupBox;
class QSpinBox;
class QTableView;
class ProxyModel;
class TemplateImage;
class QAbstractButton;

class SSitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSitemdialog(StandardTableModel* newmodel,
            const QPoint &position_=QPoint(),
            QGraphicsScene *scene_=0, QWidget *parent=0);

    SSindicator *GraphicDataItem() const { return item; }
    QStringList initlistNames;
    TemplateImage *image;
public slots:
    void accept();

private slots:
    void templateChanged(const QModelIndex& index);
    void updateUi();
    void imageChanged(const QPixmap&);
    void stateCountChanged();
    void buttonClicked(QAbstractButton *);
private:
    void createWidgets();
    void initListTemplates();
    void createLayout();
    void createConnections();
    void restoreFilters();
    //void connectItem(QObject *item);
    //TextEdit *textEdit;
    QDialogButtonBox *buttonBox;
    SSindicator *item;
    QPoint position;
    QGraphicsScene *scene;

    //QTableWidget* tableStatewdg;
    //QString originalHtml;
    //QComboBox* TemplateName;
    //виджеты
    //QLabel* stateIndexLbl;
    //QSpinBox* stateIndex;
    QLabel* stateCountLbl;
    QSpinBox* stateCount;
    QListView* listview;
    QGroupBox* filterSelectGroupBox;
    //модель/представл. таблицы
    StandardTableModel *model;
    ProxyModel *proxyModel;
    QTableView *tableView;

    void createUniqListModel(QListView *curlistview, int column);
    void initializeModel();
    void deleteTemplate(QString TemplateName);
signals:
    void updateStatesCount(QString TemplateName,int value);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif // SSITEMDIALOG_H
