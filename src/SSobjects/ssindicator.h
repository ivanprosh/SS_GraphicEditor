#ifndef SSINDICATOR_H
#define SSINDICATOR_H

#include <QGraphicsPixmapItem>
#include <QModelIndex>
#include "../itemtypes.hpp"
#include "aqp.hpp"
#include "graphicscene.h"
#include "mainwindow.hpp"
#include <QtWidgets>

class StandardTableModel;

class SSindicator: public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString TemplateName READ TemplateName WRITE setTemplateName)
    Q_PROPERTY(int statesCount READ statesCount)
    Q_PROPERTY(int commandsCount READ commandsCount WRITE setCommandsCount)
    Q_PROPERTY(int BorderBlinkFreq READ BorderBlinkFreq WRITE setBorderBlinkFreq)

    Q_PROPERTY(bool AutoCloseDUWnd READ isAutoCloseDUWnd WRITE setAutoCloseDUWnd)
    Q_PROPERTY(int StateDigParCount READ StateDigParCount)
    Q_PROPERTY(QString BorderCtrlDigPar_Tag READ BorderCtrlDigPar_Tag WRITE setBorderCtrlDigPar_Tag)
    Q_PROPERTY(QString BorderCtrlDigPar_Unit READ BorderCtrlDigPar_Unit WRITE setBorderCtrlDigPar_Unit)
    Q_PROPERTY(bool BorderCtrlDigParInvert READ isBorderCtrlDigParInvert WRITE setBorderCtrlDigParInvert)
    Q_PROPERTY(QColor BorderColor READ BorderColor WRITE setBorderColor)
    Q_PROPERTY(int BorderWidth READ BorderWidth WRITE setBorderWidth)
    Q_PROPERTY(int BorderBlinkFreq READ BorderBlinkFreq WRITE setBorderBlinkFreq)

    //Q_PROPERTY(SScommandProperty Commands READ Command WRITE setCommand)

public:
    enum {Type = SSIndItemType};
    int type() const { return Type; }

    explicit SSindicator(const QPoint &position, QGraphicsScene *scene, const QModelIndex &Template_Index = QModelIndex(), QGraphicsItem *parent=nullptr);

    void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    friend QDataStream &operator<<(QDataStream &out, const SSindicator &ind);
    friend QDataStream &operator>>(QDataStream &in, SSindicator &ind);

    //свойства
    QString TemplateName() const { return m_TemplateName; }
    bool isAutoCloseDUWnd() const {return AutoCloseDUWnd; }
    QString BorderCtrlDigPar_Tag() const {return m_BorderCtrlDigPar_Tag; }
    QString BorderCtrlDigPar_Unit() const {return m_BorderCtrlDigPar_Unit; }
    bool isBorderCtrlDigParInvert() const {return BorderCtrlDigParInvert; }
    QColor BorderColor() const {return m_BorderColor; }
    int BorderWidth() const {return m_BorderWidth; }
    int BorderBlinkFreq() const {return m_BorderBlinkFreq; }
    int StateDigParCount() const {return m_StateDigParCount; }
    int statesCount() const {return m_statesCount; }
    int commandsCount() const {return m_commandsCount;}
    //SScommandProperty Command1() const {return commands.isEmpty() ? qMakePair(QString(),QString()): commands.first();}

private:
    QPixmap image;
    QString m_TemplateName;
    //QStringList &dynamicPropertyList;
    void paintSelectionOutline(QPainter *painter);

    //свойства
    bool AutoCloseDUWnd;
    QString m_BorderCtrlDigPar_Tag;
    QString m_BorderCtrlDigPar_Unit;
    bool BorderCtrlDigParInvert;
    QColor m_BorderColor;
    int m_BorderWidth;
    int m_BorderBlinkFreq;
    int m_StateDigParCount;
    int m_statesCount;
    int m_commandsCount;

    QHash<QString,SScommandProperty> commands;

    void initializeProperties();
    void createInternalConnections();
public slots:
    void TemplateNameChanged(const QString& oldName,const QString& newName);
    void ImageChanged(const QString& ,const QPixmap& pix);
    void StatesCountChanged(QString TemplateName,int value);
    void dynPropertyChanged(QByteArray propName);
    //void ModelDataChanged(const QModelIndex& indexStart,const QModelIndex& indexFinish);

    //свойства
    void setTemplateName(const QString& newName);
    void setAutoCloseDUWnd(bool value){
        if(value != AutoCloseDUWnd){AutoCloseDUWnd=value;emit dirty();}
    }
    void setBorderCtrlDigPar_Tag(const QString& newName){
        if(newName != m_BorderCtrlDigPar_Tag){m_BorderCtrlDigPar_Tag=newName;emit dirty();}
    }
    void setBorderCtrlDigPar_Unit(const QString& newName){
        if(newName != m_BorderCtrlDigPar_Unit){m_BorderCtrlDigPar_Unit=newName;emit dirty();}
    }
    void setBorderCtrlDigParInvert(bool value){
        if(value != BorderCtrlDigParInvert){BorderCtrlDigParInvert=value;emit dirty();}
    }
    void setBorderColor(const QColor& color){
        if(color!=m_BorderColor){m_BorderColor=color;emit dirty();}
    }
    void setBorderWidth(const int& value){
        if(value != m_BorderWidth){m_BorderWidth=value;emit dirty();}
    }
    void setBorderBlinkFreq(const int& value){
        if(value != m_BorderBlinkFreq){m_BorderBlinkFreq=value;emit dirty();}
    }
    void setStatesCount(const int& value){
        if(value != m_statesCount){m_statesCount=value;emit dirty();}
    }
    void setCommandsCount(const int& value){
        if(value != m_commandsCount){
            m_commandsCount=value;
            updateCommandsView();
            emit commandsCountChanged(value);
            emit dirty();
        }
    }
    void updateCommandsView();

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    bool event(QEvent *event);
signals:
    void dirty();
    void commandsCountChanged(int);
private slots:
    void debinfo();
};

#endif // SSINDICATOR_H
