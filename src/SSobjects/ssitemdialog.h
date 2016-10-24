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

class SSitemdialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSitemdialog(QMap<QString,SSGraphicData>& GraphicData,
                          //TextItem *item_=0,
            const QPoint &position_=QPoint(),
            QGraphicsScene *scene_=0, QWidget *parent=0);

    SSGraphicData *GraphicDataItem() const { return item; }

public slots:
    void accept();

private slots:
    void updateUi();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

    //TextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    SSGraphicData *item;
    QPoint position;
    QGraphicsScene *scene;
    QListWidget* listwdg;
    QTableWidget* tableStatewdg;
    //QString originalHtml;
    QComboBox* TemplateName;
    QLabel* TemplateLabel;
    //модель таблицы
    StandardTableModel *model;
};
};

#endif // SSITEMDIALOG_H
