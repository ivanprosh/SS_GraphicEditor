#ifndef SSDYNTEXT_H
#define SSDYNTEXT_H

#include "propmanager.h"
#include "itemtypes.hpp"
#include "ssrectitem.h"
#include <QStyleOptionGraphicsItem>
#include <QFont>
#include <QGraphicsRectItem>
#include <QStaticText>
#include <QPainter>

class QDataStream;
class QSettings;

class SSDynText : public SSRectItem
{
    Q_OBJECT
    Q_CLASSINFO("Name", tr("Dynamic text properties"))

    //свойства
    Q_PROPERTY(QString Help READ Help WRITE setHelp)
    Q_PROPERTY(int StatesCount READ StatesCount WRITE setStatesCount)
    //Q_PROPERTY(int StateDigParCount READ StateDigParCount WRITE setStateDigParCount)
    Q_PROPERTY(QFont Font READ Font WRITE setFont)
    Q_PROPERTY(QColor BackColor READ BackColor WRITE setBackColor)
    Q_PROPERTY(QColor ForeColor READ ForeColor WRITE setForeColor)
    Q_PROPERTY(bool BackTransparency READ BackTransparency WRITE setBackTransparency)
    Q_PROPERTY(bool _3DText READ _3DText WRITE set_3DText)
    Q_PROPERTY(_XAlignment XAlignment READ XAlignment WRITE setXAlignment)
    Q_PROPERTY(_YAlignment YAlignment READ YAlignment WRITE setYAlignment)

    //доп. св-ва
    Q_PROPERTY(int VisibleIndex READ VisibleIndex WRITE setVisibleIndex)

public:
    enum {Type = SSDynTextItemType};
    enum _XAlignment { AlignHCenter=Qt::AlignHCenter, AlignLeft=Qt::AlignLeft, AlignRight=Qt::AlignRight };
    enum _YAlignment { AlignVCenter=Qt::AlignVCenter, AlignTop=Qt::AlignTop, AlignBottom=Qt::AlignBottom };

    Q_ENUM(_XAlignment)
    Q_ENUM(_YAlignment)

    explicit SSDynText(const QRect &rect);
    int type() const { return Type; }

    //свойства
    QString Help() const {return m_Help;}
    QColor BackColor() const { return m_BackColor;}
    QColor ForeColor() const { return m_ForeColor;}
    QFont Font() const { return m_Font;}
    bool BackTransparency() const {return m_BackTransparency;}
    bool _3DText() const {return m_3DText;}
    _XAlignment XAlignment() const {return m_XAlignment;}
    _YAlignment YAlignment() const {return m_YAlignment;}
    int VisibleIndex() const {return m_VisibleIndex;}
    int StatesCount() const {return m_StatesCount;}

    //карта соответствий программного тега и индекса состояния
    QHash<QString,SScommandProperty> states;

signals:
    void dirty();
    void dynamicPropCountChanged(QString,int);

public slots:
    void dynPropertyChanged(QByteArray propName);

    //свойства
    void setHelp(const QString& value){
        if(value != m_Help){m_Help=value;emit dirty();}
    }
    void setBackColor(const QColor& value) {
        if(value != m_BackColor){
            m_BackColor=value;
            QGraphicsRectItem::setBrush(QBrush(BackColor(),Qt::SolidPattern));
            update();
            emit dirty();
        }
    }
    void setForeColor(const QColor& value) {
        if(value != m_ForeColor){
            m_ForeColor=value;
            QGraphicsRectItem::setPen(QPen(ForeColor()));
            update();
            emit dirty();
        }
    }
    void setFont(const QFont& value){
        if(value != m_Font){
            m_Font=value;
            update();
            emit dirty();}
    }
    void set_3DText(const bool& value) {
        if(value != m_3DText){m_3DText=value;emit dirty();}
    }
    void setBackTransparency(const bool& value) {
        if(value != m_BackTransparency){m_BackTransparency=value;emit dirty();}
    }
    void setXAlignment(_XAlignment value) {
        if(value != m_XAlignment){
            m_XAlignment=value;
            update();
            emit dirty();}
    }
    void setYAlignment(_YAlignment value) {
        if(value != m_YAlignment){
            m_YAlignment=value;
            update();
            emit dirty();}
    }
    void setStatesCount(const int& value){
        if(value != m_StatesCount){
            m_StatesCount=value;
            updateDynamicPropView(tr("State"),states,m_StatesCount);
            emit dynamicPropCountChanged(tr("State"),StateStartIndex);
            emit dirty();
        }
    }
    void setVisibleIndex(const int& value){
        if(value != m_VisibleIndex){
            m_VisibleIndex=value;
            update();
            emit dirty();
        }
    }
protected:
    void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool event(QEvent *event);
private:
    void updateTransform();
    void updateDynamicPropView(QString propSingleName, QHash<QString, SScommandProperty> &hash, int count);
    void updateHashDynProperties(QHash<QString, SScommandProperty> &hash, const QString &propName);

    //свойства
    int m_StatesCount;
    int m_VisibleIndex;
    QString m_Help;
    QColor m_BackColor;
    QColor m_ForeColor;
    QFont m_Font;
    bool m_BackTransparency;
    bool m_3DText;
    _XAlignment m_XAlignment;
    _YAlignment m_YAlignment;

};
/*
QDataStream &operator<<(QDataStream &out, const SSDynText &DynTextItem);
QDataStream &operator>>(QDataStream &in, SSDynText &DynTextItem);

QSettings &operator<<(QSettings &out, const SSDynText &DynTextItem);
QSettings &operator>>(QSettings &in, SSDynText &DynTextItem);
*/
#endif // SSDYNTEXT_H
