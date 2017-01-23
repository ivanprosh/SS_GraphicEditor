#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QMap>
//#include "SSobjects/ssgraphobjinfo.h"

class GraphicsView;
class QAction;
class QFile;
class QGraphicsItem;
class QGraphicsItemGroup;
class QGraphicsScene;
class QtVariantProperty;
class QtProperty;
class TPropManager;
class StandardTableModel;
class SSitemdialog;
class QUndoStack;
class QUndoView;
class GraphicScene;
class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow() { delete printer; }

    //QSize sizeHint() const;

public slots:
    void setDirty(bool on=true);
    void clearUndoStack();

protected:
    void closeEvent(QCloseEvent *event);    
    //void addClassProperties(const QObject *object);
private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void fileExport();
    void filePrint();
    void editSelectedItem();
    void editAddItem();
    void editCopy();
    void editCut();
    void editPaste();
    void editAlign();
    void editClearTransforms();
    void updateUi();
    void viewShowGrid(bool on);
    void selectionChanged();
    void loadFile();
    void editSSobj();
    //
    //void itemClicked(QGraphicsItem *item);
    void itemMoved(QHash<QGraphicsItem *,QPointF> sceneBefore);
    void valueChanged(QtProperty *property, const QVariant &value);
#ifdef SCREENSHOTS
    void takeScreenshot();
#endif

private:
    void createSceneAndView();
    void createActions();
    void createMenusAndToolBars();
    void populateMenuAndToolBar(QMenu *menu, QToolBar *toolBar,
                                QList<QAction*> actions);
    void createDockWidgets();
    void createConnections();
    bool okToClearData();
    QPoint position();
    void clear();
    void connectItem(QObject *item);
    bool openPageDesignerFile(QFile *file, QDataStream &in);
    void readItems(QDataStream &in, int offset=0, bool select=false);

    template<class T>
    void writeItems(T &out,
                    const QList<QGraphicsItem*> &items){

        foreach (QGraphicsItem *item, items) {
            if (item == gridGroup || item->group() == gridGroup)
                continue;
            qint32 type = static_cast<qint32>(item->type());
            out << type;
            switch (type) {
                case BoxItemType:
                        out << *static_cast<BoxItem*>(item); break;
                case SSIndItemType:
                        out << *static_cast<SSindicator*>(item); break;
                case TextItemType:
                        out << *static_cast<TextItem*>(item); break;
                case SSControlButtonType:
                case SSTransitionButtonType:
                case SSAnparItemType:
                case SSDynTextItemType:
                        out << *static_cast<SSRectItem*>(item); break;
                /*
                case SSControlButtonType:
                        out << *static_cast<SSControlButton*>(item); break;
                case SSAnparItemType:
                        out << *static_cast<SSAnpar*>(item); break;
                case SSDynTextItemType:
                        out << *static_cast<SSDynText*>(item); break;
                */
                default: Q_ASSERT(false);
            }
        }
    }

    void copyItems(const QList<QGraphicsItem*> &items);
    void selectItems(const QSet<QGraphicsItem*> &items);
    void alignItems(Qt::Alignment alignment);
    void exportImage(const QString &filename);
    void exportSvg(const QString &filename);
    void paintScene(QPainter *painter);
    void populateCoordinates(const Qt::Alignment &alignment,
            QVector<double> *coordinates,
            const QList<QGraphicsItem*> &items);
#ifdef ANIMATE_ALIGNMENT
    void animateAlignment(const QList<QGraphicsItem*> &items,
                          const QList<QPointF> &positions);
#endif
    bool sceneHasItems() const;
    //void updateExpandState();
    //void getSelectionProperties(bool *hasBrushProperty,
    //                            bool *hasPenProperty) const;
    //void addProperty(QtVariantProperty *property, const QString &id);

    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExportAction;
    QAction *filePrintAction;
    QAction *fileQuitAction;
    QAction *editSelectedItemAction;
    QAction *editAddTextAction;
    QAction *editAddBoxAction;
    //
    QAction *editAddSSAction;
    QAction *editAddSSIndAction;
    QAction *editAddSSAnparAction;
    QAction *editAddSSDynTextAction;
    QAction *editAddSSTransitionButtonAction;
    QAction *editAddSSControlButtonAction;

    QAction *undoAction;
    QAction *redoAction;
    QAction *editCopyAction;
    QAction *editCutAction;
    QAction *editPasteAction;
    QAction *editAlignmentAction;
    QAction *editAlignLeftAction;
    QAction *editAlignRightAction;
    QAction *editAlignTopAction;
    QAction *editAlignBottomAction;
    QAction *editClearTransformsAction;
    QAction *viewZoomInAction;
    QAction *viewZoomOutAction;
    QAction *viewShowGridAction;

    QPrinter *printer;
    GraphicScene *scene;
    GraphicsView *view;
    QGraphicsItemGroup *gridGroup;
    QPoint previousPoint;
    QGraphicsItem *currentItem;
    SSitemdialog *dialog;
    // objects-properties framework
    //class QtVariantPropertyManager *variantManager;
    //class QtVariantPropertyManager *m_readOnlyManager;
    class TPropManager *variantManager;
    //class TPropManager *m_readOnlyManager;
    class QtTreePropertyBrowser *propertyEditor;

    StandardTableModel *model;

    //система повтора-отмены действий
    QUndoStack *undoStack;
    QUndoView *undoView;
    void createUndoView();

    int addOffset;
    int pasteOffset;

signals:
    void itemChanged(QObject* object);
#ifdef SCREENSHOTS
    QList<QPixmap> screenshots;
#endif

};

QSettings &operator<<(QSettings &out, const int &type);
QSettings &operator>>(QSettings &in, int &type);

#endif // MAINWINDOW_H
