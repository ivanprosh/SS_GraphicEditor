#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

#include "SSobjects/ssindicator.h"
#include "aqp/alt_key.hpp"
#include "aqp/aqp.hpp"
#include "mainwindow.hpp"
#include "textitemdialog.hpp"
#include "textitem.hpp"
#include "graphicscene.h"
#include "graphicsview.hpp"
#include "propmanager.h"
#include "boxitem.hpp"
#include "commands.h"
//#include "SSobjects/ssgraphobjinfo.h"
#include "SSobjects/DialogIndSource/ssitemdialog.h"
#include "SSobjects/DialogIndSource/standardtablemodel.hpp"
//#include "brushwidget.hpp"
//#include "penwidget.hpp"
#include "smileyitem.hpp"
//#include "transformwidget.hpp"
#include <QtWidgets>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QImageWriter>
#include <QMenu>
#include <QMenuBar>
#include <QMimeData>
#ifdef ANIMATE_ALIGNMENT
#include <QPropertyAnimation>
#endif
#ifdef ANIMATE_IN_PARALLEL
#include <QParallelAnimationGroup>
#endif
#include <QPrintDialog>
#include <QSettings>
#include <QStatusBar>
#include <QSvgGenerator>
#include <QTimer>
#include <QToolBar>
#ifdef USE_STL
#include <algorithm>
#endif
#include <cmath>
#include <limits>
#include <QMetaProperty>
#include <QDebug>

namespace {

const int StatusTimeout = AQP::MSecPerSecond * 30;
const int OffsetIncrement = 5;
const qint32 MagicNumber = 0x5A93DE5;
const qint16 VersionNumber = 1;
const QString ShowGrid("ShowGrid");
const QString MostRecentFile("MostRecentFile");
const QString MimeType = "application/vnd.qtrac.pagedesigner";


#ifndef USE_STL
template<template<typename T> class S, typename T>
T min(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T minimum = sequence.first();
    foreach (const T &item, sequence)
        if (item < minimum)
            minimum = item;
    return minimum;
}

template<template<typename T> class S, typename T>
T max(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T maximum = sequence.first();
    foreach (const T &item, sequence)
        if (item > maximum)
            maximum = item;
    return maximum;
}
#endif

#ifdef NO_DYNAMIC_CAST
QObject *qObjectFrom(QGraphicsItem *item)
{
    if (!item)
        return 0;
    // Types not inheriting QGraphicsObject must be handled explicitly
    if (item->type() == BoxItemType)
        return qobject_cast<QObject*>(static_cast<BoxItem*>(item));
    // Types inheriting QGraphicsObject can be handled generically
    return item->toGraphicsObject();
}
#endif

} // anonymous namespace


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gridGroup(0), addOffset(OffsetIncrement),
      pasteOffset(OffsetIncrement), currentItem(0),dialog(nullptr)
{
    printer = new QPrinter(QPrinter::HighResolution);
    model = new StandardTableModel(this,QStringList()<<"T1"<<"T2"<<"T3");

    undoStack = new QUndoStack(this);

    createSceneAndView();
    createUndoView();
    createActions();
    createMenusAndToolBars();
    createDockWidgets();
    createConnections();

    QSettings settings;
    viewShowGridAction->setChecked(
            settings.value(ShowGrid, true).toBool());
    QString filename = settings.value(MostRecentFile).toString();
    if (filename.isEmpty() || filename == tr("Unnamed"))
        QTimer::singleShot(0, this, SLOT(fileNew()));
    else {
        setWindowFilePath(filename);
        QTimer::singleShot(0, this, SLOT(loadFile()));
    }
}
void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}
/*
QSize MainWindow::sizeHint() const
{
    QSize size = printer->paperSize(QPrinter::Point).toSize() * 1.2;
    size.rwidth() += brushWidget->sizeHint().width();
    return size.boundedTo(
            QApplication::desktop()->availableGeometry().size());
}
*/
//
void MainWindow::createSceneAndView()
{
    view = new GraphicsView;
    scene = new GraphicScene(this);
    QSize pageSize = printer->paperSize(QPrinter::Point).toSize();
    scene->setSceneRect(0, 0, pageSize.width(), pageSize.height());
    view->setScene(scene);
    setCentralWidget(view);
}

//
void MainWindow::createActions()
{
    fileNewAction = new QAction(QIcon(":images/filenew.png"),
            tr("New..."), this);
    fileNewAction->setShortcuts(QKeySequence::New);
    fileOpenAction = new QAction(QIcon(":images/fileopen.png"),
            tr("Open..."), this);
    fileOpenAction->setShortcuts(QKeySequence::Open);
    fileSaveAction = new QAction(QIcon(":images/filesave.png"),
            tr("Save"), this);
    fileSaveAction->setShortcuts(QKeySequence::Save);
    fileSaveAsAction = new QAction(QIcon(":images/filesave.png"),
            tr("Save As..."), this);

    fileSaveAsAction->setShortcuts(QKeySequence::SaveAs);

    fileExportAction = new QAction(QIcon(":images/fileexport.png"),
            tr("Export..."), this);
    filePrintAction = new QAction(QIcon(":images/fileprint.png"),
            tr("Print..."), this);
    fileQuitAction = new QAction(QIcon(":images/filequit.png"),
            tr("Quit"), this);

    fileQuitAction->setShortcuts(QKeySequence::Quit);

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    editSelectedItemAction = new QAction(
            QIcon(":images/editselecteditem.png"),
            tr("Edit Selected Item..."), this);
    editAddTextAction = new QAction(QIcon(":images/editaddtext.png"),
            tr("Add Text..."), this);
    editAddTextAction->setData(TextItemType);
    editAddBoxAction = new QAction(QIcon(":images/editaddbox.png"),
            tr("Add Box"), this);
    editAddBoxAction->setData(BoxItemType);
    //Граф.объекты SS
    editAddSSAction = new QAction(QIcon(":images/ind_default.svg"),
            tr("SS objects"), this);
    editAddSSAction->setData(SSIndItemType);
    editAddSSIndAction = new QAction(QIcon(":images/ind_default.svg"),
           tr("Indicator"), this);
    editAddSSIndAction->setData(SSIndItemType);
    editAddSSAnparAction = new QAction(QIcon(":images/anpar.svg"),
           tr("Anpar"), this);
    editAddSSAnparAction->setData(SSAnparItemType);
    editAddSSControlButtonAction = new QAction(QIcon(":images/controlbutton.svg"),
           tr("ControlButton"), this);
    editAddSSControlButtonAction->setData(SSControlButtonType);
    editAddSSTransitionButtonAction = new QAction(QIcon(":images/transitionbutton.svg"),
           tr("TransitionButton"), this);
    editAddSSTransitionButtonAction->setData(SSTransitionButtonType);
    editAddSSDynTextAction = new QAction(QIcon(":images/dynamicText.svg"),
           tr("DynamicText"), this);
    editAddSSDynTextAction->setData(SSDynTextItemType);
    //
    editCopyAction = new QAction(QIcon(":images/editcopy.png"), tr("&Copy"),
                                 this);
    editCopyAction->setShortcuts(QKeySequence::Copy);
    editCutAction = new QAction(QIcon(":images/editcut.png"), tr("Cu&t"),
                                this);
    editCutAction->setShortcuts(QList<QKeySequence>()
            << QKeySequence::Cut << Qt::Key_Delete);
    editPasteAction = new QAction(QIcon(":images/editpaste.png"),
                                  tr("&Paste"), this);
    editPasteAction->setShortcuts(QKeySequence::Paste);

    editAlignmentAction = new QAction(QIcon(":images/align-left.png"),
                                      tr("Alignment"), this);
    editAlignmentAction->setData(Qt::AlignLeft);
    editAlignLeftAction = new QAction(QIcon(":images/align-left.png"),
                                      tr("Left"), this);
    editAlignLeftAction->setData(Qt::AlignLeft);
    editAlignRightAction = new QAction(QIcon(":images/align-right.png"),
                                       tr("Right"), this);
    editAlignRightAction->setData(Qt::AlignRight);
    editAlignTopAction = new QAction(QIcon(":images/align-top.png"),
                                     tr("Top"), this);
    editAlignTopAction->setData(Qt::AlignTop);
    editAlignBottomAction = new QAction(QIcon(":images/align-bottom.png"),
                                        tr("Bottom"), this);
    editAlignBottomAction->setData(Qt::AlignBottom);
    editClearTransformsAction = new
            QAction(QIcon(":images/editcleartransforms.png"),
                    tr("Clear Transformations"), this);

    viewZoomInAction = new QAction(QIcon(":images/zoom-in.png"),
                                   tr("Zoom In"), this);
    viewZoomInAction->setShortcut(tr("+"));
    viewZoomOutAction = new QAction(QIcon(":images/zoom-out.png"),
                                    tr("Zoom Out"), this);
    viewZoomOutAction->setShortcut(tr("-"));
    viewShowGridAction = new QAction(QIcon(":images/showgrid.png"),
            tr("Show Grid"), this);
    viewShowGridAction->setCheckable(true);
    viewShowGridAction->setChecked(true);
}

//
void MainWindow::createMenusAndToolBars()
{
    QAction *separator = 0;
    setUnifiedTitleAndToolBarOnMac(true);

    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    populateMenuAndToolBar(fileMenu, fileToolBar, QList<QAction*>()
            << fileNewAction << fileOpenAction << fileSaveAction
            << fileExportAction << separator << filePrintAction);
    fileMenu->insertAction(fileExportAction, fileSaveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileQuitAction);

    QMenu *alignmentMenu = new QMenu(tr("Align"), this);
    foreach (QAction *action, QList<QAction*>()
            << editAlignLeftAction << editAlignRightAction
            << editAlignTopAction << editAlignBottomAction)
        alignmentMenu->addAction(action);
    editAlignmentAction->setMenu(alignmentMenu);

    QMenu *SSMenu = new QMenu(tr("SS object"), this);
    foreach (QAction *action, QList<QAction*>()
            << editAddSSIndAction << editAddSSAnparAction << editAddSSControlButtonAction << editAddSSDynTextAction << editAddSSTransitionButtonAction)
        SSMenu->addAction(action);
    editAddSSAction->setMenu(SSMenu);

    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    populateMenuAndToolBar(editMenu, editToolBar, QList<QAction*>()
            << editSelectedItemAction << separator
            << editAddTextAction << editAddBoxAction
            << editAddSSAction << separator << editCopyAction
            << editCutAction << editPasteAction << separator
            << editAlignmentAction << editClearTransformsAction);

    QMenu *viewMenu = menuBar()->addMenu(tr("View"));
    QToolBar *viewToolBar = addToolBar(tr("View"));
    populateMenuAndToolBar(viewMenu, viewToolBar, QList<QAction*>()
            << viewZoomInAction << viewZoomOutAction << separator
            << viewShowGridAction);

    AQP::accelerateMenu(menuBar());
}

//
void MainWindow::populateMenuAndToolBar(QMenu *menu,
        QToolBar *toolBar, QList<QAction*> actions)
{
#ifdef Q_WS_MAC
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
#endif
    foreach (QAction *action, actions) {
        if (!action) {
            menu->addSeparator();
            toolBar->addSeparator();
        }
        else {
            menu->addAction(action);
            toolBar->addAction(action);
        }
    }
}

//
void MainWindow::createDockWidgets()
{
    /*
    setDockOptions(QMainWindow::AnimatedDocks);
    QDockWidget::DockWidgetFeatures features =
            QDockWidget::DockWidgetMovable|
            QDockWidget::DockWidgetFloatable;

    transformWidget = new TransformWidget;
    QDockWidget *transformDockWidget = new QDockWidget(
            tr("Transform"), this);
    transformDockWidget->setFeatures(features);
    transformDockWidget->setWidget(transformWidget);
    addDockWidget(Qt::RightDockWidgetArea, transformDockWidget);

    brushWidget = new BrushWidget;
    QDockWidget *brushDockWidget = new QDockWidget(tr("Brush (Fill)"),
                                                   this);
    brushDockWidget->setFeatures(features);
    brushDockWidget->setWidget(brushWidget);
    addDockWidget(Qt::RightDockWidgetArea, brushDockWidget);

    penWidget = new PenWidget;
    QDockWidget *penDockWidget = new QDockWidget(tr("Pen (Outline)"),
                                                 this);
    penDockWidget->setFeatures(features);
    penDockWidget->setWidget(penWidget);
    addDockWidget(Qt::RightDockWidgetArea, penDockWidget);
    */

    QDockWidget *dock = new QDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    propertyEditor = new QtTreePropertyBrowser(dock);
    variantManager = new TPropManager(this,propertyEditor);
    //m_readOnlyManager = new TPropManager(this,);

    connect(variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
                this, SLOT(valueChanged(QtProperty *, const QVariant &)));

    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);

    //propertyEditor = new QtAbstractPropertyBrowser(dock);
    propertyEditor->setFactoryForManager((QtVariantPropertyManager *)variantManager, variantFactory);
    dock->setWidget(propertyEditor);
}


void MainWindow::createConnections()
{
    connect(fileNewAction, SIGNAL(triggered()),
            this, SLOT(fileNew()));
    connect(fileOpenAction, SIGNAL(triggered()),
            this, SLOT(fileOpen()));
    connect(fileSaveAction, SIGNAL(triggered()),
            this, SLOT(fileSave()));
    connect(fileSaveAsAction, SIGNAL(triggered()),
            this, SLOT(fileSaveAs()));
    connect(fileExportAction, SIGNAL(triggered()),
            this, SLOT(fileExport()));
    connect(filePrintAction, SIGNAL(triggered()),
            this, SLOT(filePrint()));
    connect(fileQuitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(editSelectedItemAction, SIGNAL(triggered()),
            this, SLOT(editSelectedItem()));
    connect(editAddTextAction, SIGNAL(triggered()),
            this, SLOT(editAddItem()));
    connect(editAddBoxAction, SIGNAL(triggered()),
            this, SLOT(editAddItem()));
    //SS    
    foreach (QAction *action, QList<QAction*>()
            << editAddSSAction << editAddSSIndAction << editAddSSAnparAction << editAddSSControlButtonAction << editAddSSDynTextAction
            << editAddSSTransitionButtonAction)
        connect(action, SIGNAL(triggered()), this, SLOT(editAddItem()));

    //connect(editAddSSAction, SIGNAL(triggered()),
    //        this, SLOT(editAddItem()));
    connect(editCopyAction, SIGNAL(triggered()),
            this, SLOT(editCopy()));
    connect(editCutAction, SIGNAL(triggered()),
            this, SLOT(editCut()));
    connect(editPasteAction, SIGNAL(triggered()),
            this, SLOT(editPaste()));
    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(updateUi()));
    foreach (QAction *action, QList<QAction*>()
            << editAlignmentAction << editAlignLeftAction
            << editAlignRightAction << editAlignTopAction
            << editAlignBottomAction)
        connect(action, SIGNAL(triggered()), this, SLOT(editAlign()));

    connect(editClearTransformsAction, SIGNAL(triggered()),
            this, SLOT(editClearTransforms()));
    connect(scene, SIGNAL(selectionChanged()),
            this, SLOT(selectionChanged()));
    connect(viewShowGridAction, SIGNAL(toggled(bool)),
            this, SLOT(viewShowGrid(bool)));
    connect(viewZoomInAction, SIGNAL(triggered()),
            view, SLOT(zoomIn()));
    connect(viewZoomOutAction, SIGNAL(triggered()),
            view, SLOT(zoomOut()));
    connect(model, SIGNAL(itemChanged(QStandardItem*)),
                this, SLOT(setDirty()));
/*
    connect(view, SIGNAL(itemClicked(QGraphicsItem *)),
            this, SLOT(itemClicked(QGraphicsItem *)));
*/
    connect(scene, SIGNAL(itemMoved(QHash<QGraphicsItem *,QPointF>)),
            this, SLOT(itemMoved(QHash<QGraphicsItem *,QPointF>)));

    connect(this, SIGNAL(itemChanged(QObject*)),
            variantManager, SLOT(itemChanged(QObject*)));
}


void MainWindow::setDirty(bool on)
{
    setWindowModified(on);
    updateUi();
}

//
void MainWindow::updateUi()
{
    fileSaveAction->setEnabled(isWindowModified());
    bool hasItems = sceneHasItems();
    fileSaveAsAction->setEnabled(hasItems);
    fileExportAction->setEnabled(hasItems);
    filePrintAction->setEnabled(hasItems);
    int selected = scene->selectedItems().count();
    editSelectedItemAction->setEnabled(selected == 1);
    editCopyAction->setEnabled(selected >= 1);
    editCutAction->setEnabled(selected >= 1);
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    editPasteAction->setEnabled(mimeData &&
            (mimeData->hasFormat(MimeType) || mimeData->hasHtml() ||
             mimeData->hasText()));
    editAlignmentAction->setEnabled(selected >= 2);
    editClearTransformsAction->setEnabled(selected >= 1);
}


bool MainWindow::sceneHasItems() const
{
    foreach (QGraphicsItem *item, scene->items())
        if (item != gridGroup && item->group() != gridGroup)
            return true;
    return false;
}

/*
void MainWindow::getSelectionProperties(bool *hasBrushProperty,
                                        bool *hasPenProperty) const
{
    Q_ASSERT(hasBrushProperty && hasPenProperty);
    *hasBrushProperty = false;
    *hasPenProperty = false;
    foreach (QGraphicsItem *item, scene->selectedItems()) {
#ifdef NO_DYNAMIC_CAST
        if (QObject *object = qObjectFrom(item)) {
#else
        if (QObject *object = dynamic_cast<QObject*>(item)) {
#endif
            const QMetaObject *metaObject = object->metaObject();
            if (metaObject->indexOfProperty("brush") > -1)
                *hasBrushProperty = true;
            if (metaObject->indexOfProperty("pen") > -1)
                *hasPenProperty = true;
            if (*hasBrushProperty && *hasPenProperty)
                break;
        }
    }
}
*/
//
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToClearData()) {
#ifdef SCREENSHOTS
        for (int i = 0; i < screenshots.count(); ++i)
            screenshots.at(i).save(QString("pagedesigner%1.png")
                                   .arg(i + 1, 2, 10, QChar('0')));
#endif
        QSettings settings;
        settings.setValue(ShowGrid, viewShowGridAction->isChecked());
        settings.setValue(MostRecentFile, windowFilePath());
        event->accept();
    }
    else
        event->ignore();
}


void MainWindow::fileNew()
{
    if (!okToClearData())
        return;
    clear();
    setWindowFilePath(tr("Unnamed"));
    setDirty(false);
}

//
bool MainWindow::okToClearData()
{
    if (isWindowModified())
        return AQP::okToClearData(&MainWindow::fileSave, this,
                tr("Unsaved changes"), tr("Save unsaved changes?"));
    return true;
}

//
void MainWindow::fileOpen()
{
    if (!okToClearData())
        return;
    const QString &filename = QFileDialog::getOpenFileName(this,
            tr("%1 - Open").arg(QApplication::applicationName()),
            ".", tr("SS Graphic Editor (*.sge)"));
    if (filename.isEmpty())
        return;
    setWindowFilePath(filename);
    loadFile();
}


void MainWindow::loadFile()
{
    QFile file(windowFilePath());
    QDataStream in;
    if (!openPageDesignerFile(&file, in))
        return;
    in.setVersion(QDataStream::Qt_5_3);
    clear();
    readItems(in);
    statusBar()->showMessage(tr("Loaded %1").arg(windowFilePath()),
                             StatusTimeout);
    setDirty(false);
}


bool MainWindow::openPageDesignerFile(QFile *file, QDataStream &in)
{
    if (!file->open(QIODevice::ReadOnly)) {
        AQP::warning(this, tr("Error"), tr("Failed to open file: %1")
                                        .arg(file->errorString()));
        return false;
    }
    in.setDevice(file);
    qint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != MagicNumber) {
        AQP::warning(this, tr("Error"),
                tr("%1 is not a %2 file").arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    qint16 versionNumber;
    in >> versionNumber;
    if (versionNumber > VersionNumber) {
        AQP::warning(this, tr("Error"),
                tr("%1 needs a more recent version of %2")
                .arg(file->fileName())
                .arg(QApplication::applicationName()));
        return false;
    }
    return true;
}


void MainWindow::clear()
{
    scene->clear();
    gridGroup = 0;
    viewShowGrid(viewShowGridAction->isChecked());
}


void MainWindow::readItems(QDataStream &in, int offset, bool select)
{
    QSet<QGraphicsItem*> items;
    qint32 itemType;
    QGraphicsItem *item = 0;
    while (!in.atEnd()) {
        in >> itemType;
        switch (itemType) {
            case BoxItemType: {
                BoxItem *boxItem = new BoxItem(QRect(), scene);
                in >> *boxItem;
                connectItem(boxItem);
                item = boxItem;
                break;
            }
            case SSIndItemType: {
                SSindicator *indicator = new SSindicator(QPoint(),scene);
                in >> *indicator;
                connectItem(indicator);
                item = indicator;
                break;
            }
            case TextItemType: {
                TextItem *textItem = new TextItem(QPoint(), scene);
                in >> *textItem;
                connectItem(textItem);
                item = textItem;
                break;
            }
        }
        if (item) {
            item->moveBy(offset, offset);
            if (select)
                items << item;
            QUndoCommand *addCommand = new AddCommand(dynamic_cast<QGraphicsItem*>(item), scene);
            undoStack->push(addCommand);
            item = 0;
        }
    }
    if (select)
        selectItems(items);
    else
        selectionChanged();
}


void MainWindow::selectItems(const QSet<QGraphicsItem*> &items)
{
    scene->clearSelection();
    foreach (QGraphicsItem *item, items)
        item->setSelected(true);
    selectionChanged();
}


bool MainWindow::fileSave()
{
    const QString filename = windowFilePath();
    if (filename.isEmpty() || filename == tr("Unnamed"))
        return fileSaveAs();
    QSettings file(filename,QSettings::IniFormat);
    //QFile file(filename);
    //if (!file.open(QIODevice::WriteOnly))
    //    return false;
    //QDataStream out(&file);
    //out << MagicNumber << VersionNumber;
    //out.setVersion(QDataStream::Qt_5_3);
    writeItems(file, scene->items());
    //file.close();
    setDirty(false);
    return true;

}

bool MainWindow::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,
            tr("%1 - Save As").arg(QApplication::applicationName()),
            ".", tr("Page Designer (*.pd)"));
    if (filename.isEmpty())
        return false;
    if (!filename.toLower().endsWith(".pd"))
        filename += ".pd";
    setWindowFilePath(filename);
    return fileSave();
}


void MainWindow::fileExport()
{
    QString suffixes = AQP::filenameFilter(tr("Bitmap image"),
            QImageWriter::supportedImageFormats());
    suffixes += tr(";;Vector image (*.svg)");
    const QString filename = QFileDialog::getSaveFileName(this,
            tr("%1 - Export").arg(QApplication::applicationName()),
            ".", suffixes);
    if (filename.isEmpty())
        return;
    if (filename.toLower().endsWith(".svg"))
        exportSvg(filename);
    else
        exportImage(filename);
}


void MainWindow::exportSvg(const QString &filename)
{
    QSvgGenerator svg;
    svg.setFileName(filename);
    svg.setSize(printer->paperSize(QPrinter::Point).toSize());
    {
        QPainter painter(&svg);
        paintScene(&painter);
    }
    statusBar()->showMessage(tr("Exported %1").arg(filename),
                             StatusTimeout);
}


void MainWindow::exportImage(const QString &filename)
{
    QImage image(printer->paperSize(QPrinter::Point).toSize(),
                 QImage::Format_ARGB32);
    {
        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing|
                               QPainter::TextAntialiasing);
        paintScene(&painter);
    }
    if (image.save(filename))
        statusBar()->showMessage(tr("Exported %1").arg(filename),
                                 StatusTimeout);
    else
        AQP::warning(this, tr("Error"), tr("Failed to export: %1")
                                        .arg(filename));
}

//
void MainWindow::paintScene(QPainter *painter)
{
    bool showGrid = viewShowGridAction->isChecked();
    if (showGrid)
        viewShowGrid(false);
    QList<QGraphicsItem*> items = scene->selectedItems();
    scene->clearSelection();

    scene->render(painter);

    if (showGrid)
        viewShowGrid(true);
    foreach (QGraphicsItem *item, items)
        item->setSelected(true);
    selectionChanged();
}

//
void MainWindow::filePrint()
{
    QPrintDialog dialog(printer);
    if (dialog.exec()) {
        {
            QPainter painter(printer);
            paintScene(&painter);
        }
        statusBar()->showMessage(tr("Printed %1")
                .arg(windowFilePath()), StatusTimeout);
    }
}

//
QPoint MainWindow::position()
{
    QPoint point = mapFromGlobal(QCursor::pos());
    if (!view->geometry().contains(point)) {
        point = previousPoint.isNull()
                ? view->pos() + QPoint(10, 10) : previousPoint;
    }
    if (!previousPoint.isNull() && point == previousPoint) {
        point += QPoint(addOffset, addOffset);
        addOffset += OffsetIncrement;
    }
    else {
        addOffset = OffsetIncrement;
        previousPoint = point;
    }
    return view->mapToScene(point - view->pos()).toPoint();
}


void MainWindow::editSelectedItem()
{
    QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.count() != 1)
        return;
#ifdef NO_DYNAMIC_CAST
    if (QObject *item = qObjectFrom(items.at(0))) {
#else
    if (QObject *item = dynamic_cast<QObject*>(items.at(0))) {
#endif
        const QMetaObject *metaObject = item->metaObject();
        metaObject->invokeMethod(item, "edit", Qt::DirectConnection);
    }
}

//edit
void MainWindow::editAddItem()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;
    if (action != editAddSSAction) {
        editAddSSAction->setData(action->data());
        editAddSSAction->setIcon(action->icon());
    }
    QObject *item = 0;
    int type = action->data().toInt();

    switch (type) {
    case BoxItemType:
        item = new BoxItem(QRect(position(), QSize(90, 30)), scene);
        break;
    case TextItemType:
        TextItemDialog dialog(0, position(), scene, this);
        if (dialog.exec())
            item = dialog.textItem();
        break;
    //элементы SS
    case SSIndItemType:
        if(!dialog){
            dialog = new SSitemdialog(model,position(), scene, this);
        } else {
            dialog->show();
        }
        if (dialog->exec())
            item = dialog->GraphicDataItem();
        break;
    case SSAnparItemType:
        ;
        break;
    case SSControlButtonType:
        ;
        break;
    case SSTransitionButtonType:
        ;
        break;
    case SSDynTextItemType:
        ;
        break;
    default:
        break;
    }
    /*
    if (type == BoxItemType)
        item = new BoxItem(QRect(position(), QSize(90, 30)), scene);
    else if (type == SSIndItemType){
        if(!dialog){
            dialog = new SSitemdialog(model,position(), scene, this);
        } else {
            dialog->show();
        }
        if (dialog->exec())
            item = dialog->GraphicDataItem();
    }
    else if (type == TextItemType) {
        TextItemDialog dialog(0, position(), scene, this);
        if (dialog.exec())
            item = dialog.textItem();
    }
    */
    if (item) {
        QUndoCommand *addCommand = new AddCommand(dynamic_cast<QGraphicsItem*>(item), scene);
        undoStack->push(addCommand);
        connectItem(item);
        setDirty(true);
    }
}


void MainWindow::connectItem(QObject *item)
{
    connect(item, SIGNAL(dirty()), this, SLOT(setDirty()));

    if(dynamic_cast<SSindicator*>(item)){
        connect(model,
                SIGNAL(ImageChanged(QString,QPixmap)),
                item, SLOT(ImageChanged(QString,QPixmap)));
        connect(model, SIGNAL(TemplateNameChanged(QString,QString)),
                item, SLOT(TemplateNameChanged(QString,QString)));
        connect(dialog, SIGNAL(updateStatesCount(QString,int)),
                item, SLOT(StatesCountChanged(QString,int)));
        connect(item,SIGNAL(dynamicPropCountChanged(QString,int)),
                variantManager,SLOT(editAddDynamicProperties(QString,int)));
        //connect(item,SIGNAL(statesCountChanged(int)),
        //        variantManager,SLOT(editAddDynamicProperties(QString,int)));
    }
    /*
    const QMetaObject *metaObject = item->metaObject();
    if (metaObject->indexOfProperty("brush") > -1)
        connect(brushWidget, SIGNAL(brushChanged(const QBrush&)),
                item, SLOT(setBrush(const QBrush&)));
    if (metaObject->indexOfProperty("pen") > -1)
        connect(penWidget, SIGNAL(penChanged(const QPen&)),
                item, SLOT(setPen(const QPen&)));
    if (metaObject->indexOfProperty("angle") > -1) {
        connect(transformWidget, SIGNAL(angleChanged(double)),
                item, SLOT(setAngle(double)));
        connect(transformWidget, SIGNAL(shearChanged(double, double)),
                item, SLOT(setShear(double, double)));
    }
    */
}


void MainWindow::viewShowGrid(bool on)
{
    if (!gridGroup) {
        const int GridSize = 40;
        QPen pen(QColor(175, 175, 175, 127));
        gridGroup = new QGraphicsItemGroup;
        const int MaxX = static_cast<int>(std::ceil(scene->width())
                / GridSize) * GridSize;
        const int MaxY = static_cast<int>(std::ceil(scene->height())
                / GridSize) * GridSize;
        for (int x = 0; x <= MaxX; x += GridSize) {
            QGraphicsLineItem *item = new QGraphicsLineItem(x, 0, x,
                                                            MaxY);
            item->setPen(pen);
            item->setZValue(std::numeric_limits<int>::min());
            gridGroup->addToGroup(item);
        }
        for (int y = 0; y <= MaxY; y += GridSize) {
            QGraphicsLineItem *item = new QGraphicsLineItem(0, y,
                                                            MaxX, y);
            item->setPen(pen);
            item->setZValue(-1000);
            gridGroup->addToGroup(item);
        }
        scene->addItem(gridGroup);
    }
    gridGroup->setVisible(on);
}


void MainWindow::editCopy()
{
    QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.isEmpty())
        return;
    pasteOffset = OffsetIncrement;
    copyItems(items);
    updateUi();
}


void MainWindow::copyItems(const QList<QGraphicsItem*> &items)
{
    QByteArray copiedItems;
    QDataStream out(&copiedItems, QIODevice::WriteOnly);
    writeItems<QDataStream>(out, items);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeType, copiedItems);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}


void MainWindow::editCut()
{
    QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.isEmpty())
        return;
    copyItems(items);

    QUndoCommand *deleteCommand = new DeleteCommand(scene);
    undoStack->push(deleteCommand);
    /*
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext()) {
        QScopedPointer<QGraphicsItem> item(i.next());
        scene->removeItem(item.data());
    }
    */
    setDirty(true);
}


void MainWindow::editPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (!mimeData)
        return;

    if (mimeData->hasFormat(MimeType)) {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems, QIODevice::ReadOnly);
        readItems(in, pasteOffset, true);
        pasteOffset += OffsetIncrement;
    }
    else if (mimeData->hasHtml() || mimeData->hasText()) {
        TextItem *textItem = new TextItem(position(), scene);
        connectItem(textItem);
        if (mimeData->hasHtml())
            textItem->setHtml(mimeData->html());
        else
            textItem->setPlainText(mimeData->text());
    }
    else
        return;
    setDirty(true);
}

void MainWindow::editSSobj()
{
    /*
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;
    QObject *item = 0;
    int type = action->data().toInt();
    if (type == SSIndItemType){
        if(!dialog){
            dialog = new SSitemdialog(model,position(), scene, this);
        } else {
            dialog->show();
        }
        if (dialog->exec())
            item = dialog->GraphicDataItem();
    }
    if (item) {
        connectItem(item);
        setDirty(true);
    }
    */
    /*
    Qt::Alignment alignment = static_cast<Qt::Alignment>(
            action->data().toInt());
    if (action != editAlignmentAction) {
        editAlignmentAction->setData(action->data());
        editAlignmentAction->setIcon(action->icon());
    }

    QList<QGraphicsItem*> items = scene->selectedItems();
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;
    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop)
#ifdef USE_STL
        offset = *std::min_element(coordinates.constBegin(),
                                   coordinates.constEnd());
    else
        offset = *std::max_element(coordinates.constBegin(),
                                   coordinates.constEnd());
#else
        offset = min(coordinates);
    else
        offset = max(coordinates);
#endif
#ifdef SCREENSHOTS
    takeScreenshot();
#endif

    QList<QPointF> positions;
    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i)
            positions << items.at(i)->pos() +
                         QPointF(offset - coordinates.at(i), 0);
    }
    else {
        for (int i = 0; i < items.count(); ++i)
            positions << items.at(i)->pos() +
                         QPointF(0, offset - coordinates.at(i));
    }

    animateAlignment(items, positions);

    setDirty(true);
*/
}

#ifndef ANIMATE_ALIGNMENT
void MainWindow::editAlign()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(
            action->data().toInt());
    if (action != editAlignmentAction) {
        editAlignmentAction->setData(action->data());
        editAlignmentAction->setIcon(action->icon());
    }

    QList<QGraphicsItem*> items = scene->selectedItems();
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;
    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop)
#ifdef USE_STL
        offset = *std::min_element(coordinates.constBegin(),
                                   coordinates.constEnd());
    else
        offset = *std::max_element(coordinates.constBegin(),
                                   coordinates.constEnd());
#else
        offset = min(coordinates);
    else
        offset = max(coordinates);
#endif

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i)
            items.at(i)->moveBy(offset - coordinates.at(i), 0);
    }
    else {
        for (int i = 0; i < items.count(); ++i)
            items.at(i)->moveBy(0, offset - coordinates.at(i));
    }
    setDirty(true);
}

#else

void MainWindow::editAlign()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(
            action->data().toInt());
    if (action != editAlignmentAction) {
        editAlignmentAction->setData(action->data());
        editAlignmentAction->setIcon(action->icon());
    }

    QList<QGraphicsItem*> items = scene->selectedItems();
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;
    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop)
#ifdef USE_STL
        offset = *std::min_element(coordinates.constBegin(),
                                   coordinates.constEnd());
    else
        offset = *std::max_element(coordinates.constBegin(),
                                   coordinates.constEnd());
#else
        offset = min(coordinates);
    else
        offset = max(coordinates);
#endif
#ifdef SCREENSHOTS
    takeScreenshot();
#endif

    QList<QPointF> positions;
    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i)
            positions << items.at(i)->pos() +
                         QPointF(offset - coordinates.at(i), 0);
    }
    else {
        for (int i = 0; i < items.count(); ++i)
            positions << items.at(i)->pos() +
                         QPointF(0, offset - coordinates.at(i));
    }

    animateAlignment(items, positions);
    setDirty(true);
}


void MainWindow::animateAlignment(const QList<QGraphicsItem*> &items,
                                  const QList<QPointF> &positions)
{
    int duration = ((qApp->keyboardModifiers() & Qt::ShiftModifier)
                    != Qt::ShiftModifier) ? 1000 : 5000;

#ifdef ANIMATE_IN_PARALLEL
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
#endif
    for (int i = 0; i < items.count(); ++i) {
#ifdef NO_DYNAMIC_CAST
        QObject *object = qObjectFrom(items.at(i));
#else
        QObject *object = dynamic_cast<QObject*>(items.at(i));
#endif
        if (!object)
            continue;
        QPropertyAnimation *animation = new QPropertyAnimation(
                object, "pos", this);
        animation->setDuration(duration);
        animation->setEasingCurve(QEasingCurve::InOutBack);
#ifdef ANIMATE_IN_PARALLEL
        animation->setStartValue(items.at(i)->pos());
        animation->setEndValue(positions.at(i));
        group->addAnimation(animation);
#else
        animation->setKeyValueAt(0.0, items.at(i)->pos());
        animation->setKeyValueAt(1.0, positions.at(i));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
#endif
    }
#ifdef ANIMATE_IN_PARALLEL
    group->start(QAbstractAnimation::DeleteWhenStopped);
#endif
}

#endif

#ifdef SCREENSHOTS
void MainWindow::takeScreenshot()
{
    screenshots << QPixmap::grabWidget(view);
    if (screenshots.count() < 49)
        QTimer::singleShot(50, this, SLOT(takeScreenshot()));
}
#endif


void MainWindow::populateCoordinates(const Qt::Alignment &alignment,
        QVector<double> *coordinates,
        const QList<QGraphicsItem*> &items)
{
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext()) {
        QRectF rect = i.next()->sceneBoundingRect();
        switch (alignment) {
            case Qt::AlignLeft:
                coordinates->append(rect.x()); break;
            case Qt::AlignRight:
                coordinates->append(rect.x() + rect.width()); break;
            case Qt::AlignTop:
                coordinates->append(rect.y()); break;
            case Qt::AlignBottom:
                coordinates->append(rect.y() + rect.height()); break;
        }
    }
}


void MainWindow::editClearTransforms()
{
    /*
    QList<QGraphicsItem*> items = scene->selectedItems();
    Q_ASSERT(!items.isEmpty());
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext()) {
#ifdef NO_DYNAMIC_CAST
        if (QObject *item = qObjectFrom(i.next())) {
#else
        if (QObject *item = dynamic_cast<QObject*>(i.next())) {
#endif
            if (item->property("angle").isValid()) {
                item->setProperty("angle", 0.0);
                item->setProperty("shearHorizontal", 0.0);
                item->setProperty("shearVertical", 0.0);
            }
        }
    }
    transformWidget->setAngle(0.0);
    transformWidget->setShear(0.0, 0.0);
    setDirty(true);
    */
}

void MainWindow::selectionChanged()
{
    QList<QGraphicsItem*> items = scene->selectedItems();
    if (items.count() == 1) {

       //QObject *item = dynamic_cast<QObject*>(items.first());
       currentItem = items.first();
       //variantManager->addClassProperties(item,item->metaObject());
       emit itemChanged(dynamic_cast<QObject*>(items.first()));



        //общие свойства
        /*
        property = variantManager->addProperty(QVariant::Double, tr("Position X"));
        property->setAttribute(QLatin1String("minimum"), 0);
        property->setAttribute(QLatin1String("maximum"), scene->width());
        property->setValue(currentItem->x());
        addProperty(property, QLatin1String("xpos"));

        property = variantManager->addProperty(QVariant::Double, tr("Position Y"));
        property->setAttribute(QLatin1String("minimum"), 0);
        property->setAttribute(QLatin1String("maximum"), scene->height());
        property->setValue(currentItem->y());
        addProperty(property, QLatin1String("ypos"));

        property = variantManager->addProperty(QVariant::Double, tr("Position Z"));
        property->setAttribute(QLatin1String("minimum"), 0);
        property->setAttribute(QLatin1String("maximum"), 256);
        property->setValue(currentItem->zValue());
        addProperty(property, QLatin1String("zpos"));
        */
        //специализированные свойства

        /*
#ifdef NO_DYNAMIC_CAST
        if (QObject *item = qObjectFrom(items.at(0))) {
#else
        if (QObject *item = dynamic_cast<QObject*>(items.at(0))) {
#endif
            if (item->property("brush").isValid())
                property = variantManager->addProperty(QVariant::Color, tr("Brush Color"));
                property->setValue(item->property("brush").value<QBrush>().color());
                addProperty(property, QLatin1String("brush"));

            if (item->property("pen").isValid())
                property = variantManager->addProperty(QVariant::Color, tr("Pen Color"));
                property->setValue(item->property("pen").value<QPen>().color());
                addProperty(property, QLatin1String("pen"));

            if (item->property("angle").isValid()) {

            }

        }
        */
    } else {
        emit itemChanged(0);
    }
    updateUi();
}

void MainWindow::valueChanged(QtProperty *property, const QVariant &value)
{
    //if (!propertyToId.contains(property))
    //    return;

    if (!currentItem)
        return;
    /*
    if (!propertyEditor->m_propertyToIndex.contains(property))
        return;

    if(QObject *item = dynamic_cast<QObject*>(currentItem)){
        int idx = propertyEditor->m_propertyToIndex.value(property);

        const QMetaObject *metaObject = item->metaObject();
        QMetaProperty metaProperty = item->property(idx);
        if (metaProperty.isEnumType()) {
            if (metaProperty.isFlagType())
                metaProperty.write(item, intToFlag(metaProperty.enumerator(), value.toInt()));
            else
                metaProperty.write(item, intToEnum(metaProperty.enumerator(), value.toInt()));
        } else {
            metaProperty.write(item, value);
        }

        propertyEditor->updateClassProperties(item,metaObject, true);
    }
    */
    /*
    QString id = propertyToId[property];
    if (id == QLatin1String("xpos")) {
        currentItem->setX(value.toDouble());
    } else if (id == QLatin1String("ypos")) {
        currentItem->setY(value.toDouble());
    } else if (id == QLatin1String("zpos")) {
        currentItem->setZValue(value.toDouble());
    } else if (id == QLatin1String("text")) {
    } else if (id == QLatin1String("color")) {        
    //свойства, зависящие от типа
    } else if(QObject *item = dynamic_cast<QObject*>(currentItem)) {

            if (id == QLatin1String("brush")) {
                QBrush brush = item->property("brush").value<QBrush>();
                brush.setColor(value.value<QColor>());
                item->setProperty("brush",brush);
                //metaObject->invokeMethod(item, "setBrush", Qt::DirectConnection, Q_ARG(QBrush,brush));

            } else if (id == QLatin1String("pen")) {
                QPen pen = item->property("pen").value<QPen>();
                pen.setColor(value.value<QColor>());
                item->setProperty("pen",pen);
            }
    }
    */
    updateUi();
}
/*
void MainWindow::itemClicked(QGraphicsItem *item)
{
    updateExpandState();

    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();

    currentItem = item;
    if (!currentItem) {
        //deleteAction->setEnabled(false);
        return;
    }

    //deleteAction->setEnabled(true);

    QtVariantProperty *property;

    property = variantManager->addProperty(QVariant::Double, tr("Position X"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), canvas->width());
    property->setValue(item->x());
    addProperty(property, QLatin1String("xpos"));

    property = variantManager->addProperty(QVariant::Double, tr("Position Y"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), canvas->height());
    property->setValue(item->y());
    addProperty(property, QLatin1String("ypos"));

    property = variantManager->addProperty(QVariant::Double, tr("Position Z"));
    property->setAttribute(QLatin1String("minimum"), 0);
    property->setAttribute(QLatin1String("maximum"), 256);
    property->setValue(item->z());
    addProperty(property, QLatin1String("zpos"));

    if (item->rtti() == QtCanvasItem::Rtti_Rectangle) {
        QtCanvasRectangle *i = (QtCanvasRectangle *)item;

        property = variantManager->addProperty(QVariant::Color, tr("Brush Color"));
        property->setValue(i->brush().color());
        addProperty(property, QLatin1String("brush"));

        property = variantManager->addProperty(QVariant::Color, tr("Pen Color"));
        property->setValue(i->pen().color());
        addProperty(property, QLatin1String("pen"));

        property = variantManager->addProperty(QVariant::Size, tr("Size"));
        property->setValue(i->size());
        addProperty(property, QLatin1String("size"));
    } else if (item->rtti() == QtCanvasItem::Rtti_Line) {
        QtCanvasLine *i = (QtCanvasLine *)item;

        property = variantManager->addProperty(QVariant::Color, tr("Pen Color"));
        property->setValue(i->pen().color());
        addProperty(property, QLatin1String("pen"));

        property = variantManager->addProperty(QVariant::Point, tr("Vector"));
        property->setValue(i->endPoint());
        addProperty(property, QLatin1String("endpoint"));
    } else if (item->rtti() == QtCanvasItem::Rtti_Ellipse) {
        QtCanvasEllipse *i = (QtCanvasEllipse *)item;

        property = variantManager->addProperty(QVariant::Color, tr("Brush Color"));
        property->setValue(i->brush().color());
        addProperty(property, QLatin1String("brush"));

        property = variantManager->addProperty(QVariant::Size, tr("Size"));
        property->setValue(QSize(i->width(), i->height()));
        addProperty(property, QLatin1String("size"));
    } else if (item->rtti() == QtCanvasItem::Rtti_Text) {
        QtCanvasText *i = (QtCanvasText *)item;

        property = variantManager->addProperty(QVariant::Color, tr("Color"));
        property->setValue(i->color());
        addProperty(property, QLatin1String("color"));

        property = variantManager->addProperty(QVariant::String, tr("Text"));
        property->setValue(i->text());
        addProperty(property, QLatin1String("text"));

        property = variantManager->addProperty(QVariant::Font, tr("Font"));
        property->setValue(i->font());
        addProperty(property, QLatin1String("font"));
    }
}
*/
void MainWindow::itemMoved(QHash<QGraphicsItem *,QPointF> sceneBefore)
{
    if (sceneBefore.empty())
        return;

    undoStack->push(new MoveCommand(sceneBefore));
}

QSettings &operator<<(QSettings &out, const int &type){return out;}
QSettings &operator>>(QSettings &in, int &type){return in;}
