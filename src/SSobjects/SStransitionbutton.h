#ifndef TRANSITIONBUTTON_H
#define TRANSITIONBUTTON_H

#include "itemtypes.hpp"
#include <QFont>
#include <QGraphicsRectItem>

class QDataStream;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QSettings;

class SSTransitionButton : public QObject,public QGraphicsRectItem
{
    Q_OBJECT

    //свойства
    Q_PROPERTY(QString DisplayOptions READ DisplayOptions WRITE setDisplayOptions)
    Q_PROPERTY(QString PictureEnabled READ PictureEnabled WRITE setPictureEnabled)
    Q_PROPERTY(QString PictureDisabled READ PictureDisabled WRITE setPictureDisabled)
    Q_PROPERTY(QFont Text READ Text WRITE setText)
    Q_PROPERTY(QString UnitName READ UnitName WRITE setUnitName)
    Q_PROPERTY(int MnemoNum READ MnemoNum WRITE setMnemoNum)
    Q_PROPERTY(bool IsNewWnd READ IsNewWnd WRITE setNewWnd)
    Q_PROPERTY(QString Help READ Help WRITE setHelp)
    Q_PROPERTY(QString BtnState_Tag READ BtnState_Tag WRITE setBtnState_Tag)
    Q_PROPERTY(QString BtnState_Unit READ BtnState_Unit WRITE setBtnState_Unit)
    Q_PROPERTY(bool BtnStateInvert READ BtnStateInvert WRITE setBtnStateInvert)
    Q_PROPERTY(bool PictureEnabledTransparent READ PictureEnabledTransparent WRITE setPictureEnabledTransparent)
    Q_PROPERTY(bool PictureDisabledTransparent READ PictureDisabledTransparent WRITE setPictureDisabledTransparent)

public:
    enum {Type = SSTransitionButtonType};

    explicit SSTransitionButton(const QRect &rect, QGraphicsScene *scene);
    int type() const { return Type; }

    //свойства
    QString DisplayOptions() const { return m_DisplayOptions; }
    QString PictureEnabled() const { return m_PictureEnabled;}
    QString PictureDisabled() const { return m_PictureDisabled;}
    QFont Text() const { return m_Text;}
    QString UnitName() const { return m_UnitName; }
    int MnemoNum() const { return m_MnemoNum; }
    bool IsNewWnd() const { return m_IsNewWnd; }
    QString Help() const { return m_Help; }
    QString BtnState_Tag() const { return m_BtnState_Tag; }
    QString BtnState_Unit() const { return m_BtnState_Unit; }
    bool BtnStateInvert() const { return m_BtnStateInvert; }
    bool PictureEnabledTransparent() const { return m_PictureEnabledTransparent; }
    bool PictureDisabledTransparent() const { return m_PictureDisabledTransparent; }

signals:
    void dirty();

public slots:
    //свойства
    void setDisplayOptions(const QString& value) {
        if(value != m_DisplayOptions){m_DisplayOptions=value;emit dirty();}
    }
    void setPictureEnabled(const QString& value){
        if(value != m_PictureEnabled){m_PictureEnabled=value;emit dirty();}
    }
    void setPictureDisabled(const QString& value){
        if(value != m_PictureDisabled){m_PictureDisabled=value;emit dirty();}
    }
    void setText(const QFont& value){
        if(value != m_Text){m_Text=value;emit dirty();}
    }
    void setUnitName(const QString& value){
        if(value != m_UnitName){m_UnitName=value;emit dirty();}
    }
    void setMnemoNum(const int& value){
        if(value != m_MnemoNum){m_MnemoNum=value;emit dirty();}
    }
    void setNewWnd(const bool& value){
        if(value != m_IsNewWnd){m_IsNewWnd=value;emit dirty();}
    }
    void setHelp(const QString& value){
        if(value != m_Help){m_Help=value;emit dirty();}
    }
    void setBtnState_Tag(const QString& value){
        if(value != m_BtnState_Tag){m_BtnState_Tag=value;emit dirty();}
    }
    void setBtnState_Unit(const QString& value){
        if(value != m_BtnState_Unit){m_BtnState_Unit=value;emit dirty();}
    }
    void setBtnStateInvert(const bool& value){
        if(value != m_BtnStateInvert){m_BtnStateInvert=value;emit dirty();}
    }
    void setPictureEnabledTransparent(const bool& value){
        if(value != m_PictureEnabledTransparent){m_PictureEnabledTransparent=value;emit dirty();}
    }
    void setPictureDisabledTransparent(const bool& value){
        if(value != m_PictureDisabledTransparent){m_PictureDisabledTransparent=value;emit dirty();}
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
    QString m_DisplayOptions;
    QString m_PictureEnabled;
    QString m_PictureDisabled;
    QFont m_Text;
    QString m_UnitName;
    int m_MnemoNum;
    bool m_IsNewWnd;
    QString m_Help;
    QString m_BtnState_Tag;
    QString m_BtnState_Unit;
    bool m_BtnStateInvert;
    bool m_PictureEnabledTransparent;
    bool m_PictureDisabledTransparent;
};

QDataStream &operator<<(QDataStream &out, const SSTransitionButton &boxItem);
QDataStream &operator>>(QDataStream &in, SSTransitionButton &boxItem);

QSettings &operator<<(QSettings &out, const SSTransitionButton &boxItem);
QSettings &operator>>(QSettings &in, SSTransitionButton &boxItem);

#endif // TRANSITIONBUTTON_H
