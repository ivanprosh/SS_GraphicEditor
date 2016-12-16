#ifndef SSANPAR_H
#define SSANPAR_H

#include "itemtypes.hpp"
#include <QFont>
#include <QGraphicsRectItem>

class QDataStream;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QSettings;

class SSAnpar :public QObject,public QGraphicsRectItem
{
    Q_OBJECT

    //свойства
    Q_PROPERTY(QColor BackColor READ BackColor WRITE setBackColor)
    Q_PROPERTY(QColor ForeColor READ ForeColor WRITE setForeColor)
    Q_PROPERTY(QFont Font READ Font WRITE setFont)
    Q_PROPERTY(AnparTypes AnparType READ AnparType WRITE setAnparType)
    Q_PROPERTY(QString AnParTag READ AnParTag WRITE setAnParTag)
    Q_PROPERTY(QString AnParUnit READ AnParUnit WRITE setAnParUnit)
    Q_PROPERTY(QString DigParTag READ DigParTag WRITE setDigParTag)
    Q_PROPERTY(QString DigParUnit READ DigParUnit WRITE setDigParUnit)

public:
    enum {Type = SSAnparItemType};
    enum AnparTypes { Default, Horizontal, Vertical };

    Q_ENUM(AnparTypes)

    explicit SSAnpar(const QRect &rect, QGraphicsScene *scene);
    int type() const { return Type; }

    //свойства
    QColor BackColor() const { return m_BackColor;}
    QColor ForeColor() const { return m_ForeColor;}
    QFont Font() const { return m_Font;}
    AnparTypes AnparType() const { return m_AnparType;}
    QString AnParTag(){return m_AnParTag;}
    QString AnParUnit(){return m_AnParUnit;}
    QString DigParTag(){return m_DigParTag;}
    QString DigParUnit(){return m_DigParUnit;}

signals:
    void dirty();

public slots:
    //свойства
    void setBackColor(const QColor& value) {
        if(value != m_BackColor){m_BackColor=value;emit dirty();}
    }
    void setForeColor(const QColor& value) {
        if(value != m_ForeColor){m_ForeColor=value;emit dirty();}
    }
    void setFont(const QFont& value){
        if(value != m_Font){m_Font=value;emit dirty();}
    }
    void setAnparType(AnparTypes value){
        if(value != m_AnparType){m_AnparType=value;emit dirty();}
    }

    void setAnParTag(const QString& value) {
        if(value != m_AnParTag){m_AnParTag=value;emit dirty();}
    }
    void setAnParUnit(const QString& value) {
        if(value != m_AnParUnit){m_AnParUnit=value;emit dirty();}
    }
    void setDigParTag(const QString& value) {
        if(value != m_DigParTag){m_DigParTag=value;emit dirty();}
    }
    void setDigParUnit(const QString& value) {
        if(value != m_DigParUnit){m_DigParUnit=value;emit dirty();}
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
    void paintSelectionOutline(QPainter *painter);

    //свойства
    QColor m_BackColor;
    QColor m_ForeColor;
    QFont m_Font;
    AnparTypes m_AnparType;
    QString m_AnParTag;
    QString m_AnParUnit;
    QString m_DigParTag;
    QString m_DigParUnit;
};

QDataStream &operator<<(QDataStream &out, const SSAnpar &AnparItem);
QDataStream &operator>>(QDataStream &in, SSAnpar &AnparItem);

QSettings &operator<<(QSettings &out, const SSAnpar &AnparItem);
QSettings &operator>>(QSettings &in, SSAnpar &AnparItem);

#endif // SSANPAR_H
