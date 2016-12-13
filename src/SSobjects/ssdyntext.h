#ifndef SSDYNTEXT_H
#define SSDYNTEXT_H

#include "itemtypes.hpp"
#include <QFont>
#include <QGraphicsRectItem>

class QDataStream;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QSettings;

class SSDynText:   public QObject,public QGraphicsRectItem
{
    Q_OBJECT

    Q_PROPERTY(QString Help READ Help WRITE setHelp)
    //Q_PROPERTY(int statesCount READ statesCount)
    Q_PROPERTY(int StateDigParCount READ StateDigParCount WRITE setStateDigParCount)
    Q_PROPERTY(QFont Font READ Font WRITE setFont)
    Q_PROPERTY(QColor BackColor READ BackColor WRITE setBackColor)
    Q_PROPERTY(QColor ForeColor READ ForeColor WRITE setForeColor)
    Q_PROPERTY(bool BackTransparency READ BackTransparency WRITE setBackTransparency)
    Q_PROPERTY(bool _3DText READ _3DText WRITE set_3DText)
    Q_PROPERTY(Aligment XAlignment READ XAlignment WRITE setXAlignment)
    Q_PROPERTY(Aligment YAlignment READ YAlignment WRITE setYAlignment)


public:
    enum {Type = SSDynTextItemType};
    enum Aligment { Center, Left, Right };

    Q_ENUM(Aligment)

    explicit SSDynText(const QRect &rect, QGraphicsScene *scene);
    int type() const { return Type; }

    //свойства
    QString Help() const {return m_Help;}
    QColor BackColor() const { return m_BackColor;}
    QColor ForeColor() const { return m_ForeColor;}
    QFont Font() const { return m_Font;}
    bool BackTransparency() const {return m_BackTransparency;}
    bool _3DText() const {return m_3DText;}
    Aligment XAlignment() const {return m_XAlignment;}
    Aligment YAlignment() const {return m_YAlignment;}

signals:
    void dirty();

public slots:
    //свойства
    void setHelp(const QString& value){
        if(value != m_Help){m_Help=value;emit dirty();}
    }
    void setBackColor(const QColor& value) {
        if(value != m_BackColor){m_BackColor=value;emit dirty();}
    }
    void setForeColor(const QColor& value) {
        if(value != m_ForeColor){m_ForeColor=value;emit dirty();}
    }
    void setFont(const QFont& value){
        if(value != m_Font){m_Font=value;emit dirty();}
    }
    void set_3DText(const bool& value) {
        if(value != m_3DText){m_3DText=value;emit dirty();}
    }
    void setBackTransparency(const bool& value) {
        if(value != m_BackTransparency){m_BackTransparency=value;emit dirty();}
    }
    void setXAlignment(Aligment value) {
        if(value != m_XAlignment){m_XAlignment=value;emit dirty();}
    }
    void setYAlignment(Aligment value) {
        if(value != m_YAlignment){m_YAlignment=value;emit dirty();}
    }

/*
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
*/
private:
    void updateTransform();

    //свойства
    QString m_Help;
    QColor m_BackColor;
    QColor m_ForeColor;
    QFont m_Font;
    bool m_BackTransparency;
    bool m_3DText;
    Aligment m_XAlignment;
    Aligment m_YAlignment;
};

QDataStream &operator<<(QDataStream &out, const SSDynText &DynTextItem);
QDataStream &operator>>(QDataStream &in, SSDynText &DynTextItem);

QSettings &operator<<(QSettings &out, const SSDynText &DynTextItem);
QSettings &operator>>(QSettings &in, SSDynText &DynTextItem);

#endif // SSDYNTEXT_H
