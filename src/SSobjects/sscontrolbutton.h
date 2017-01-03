#ifndef SSCONTROLBUTTON_H
#define SSCONTROLBUTTON_H

#include "itemtypes.hpp"
#include "ssrectitem.h"
#include <QFont>
#include <QStaticText>

class QDataStream;
class QGraphicsScene;
class QSettings;

class SSControlButton : public SSRectItem
{
    Q_OBJECT
    Q_CLASSINFO("Name", tr("Control Button properties"))
    //свойства
    Q_PROPERTY(DisplayOption DisplayOptions READ DisplayOptions WRITE setDisplayOptions)
    Q_PROPERTY(QString PictureEnabled READ PictureEnabled WRITE setPictureEnabled)
    Q_PROPERTY(bool PictureEnabledTransparent READ PictureEnabledTransparent WRITE setPictureEnabledTransparent)
    Q_PROPERTY(QString PictureDisabled READ PictureDisabled WRITE setPictureDisabled)
    Q_PROPERTY(bool PictureDisabledTransparent READ PictureDisabledTransparent WRITE setPictureDisabledTransparent)
    Q_PROPERTY(QString Text READ Text WRITE setText)
    Q_PROPERTY(QFont Font READ Font WRITE setFont)
    //Q_PROPERTY(QString UnitName READ UnitName WRITE setUnitName)
    //Q_PROPERTY(int MnemoNum READ MnemoNum WRITE setMnemoNum)
    //Q_PROPERTY(bool IsNewWnd READ IsNewWnd WRITE setNewWnd)
    Q_PROPERTY(QString DigParPermit_Tag READ DigParPermit_Tag WRITE setDigParPermit_Tag)
    Q_PROPERTY(QString DigParPermit_Unit READ DigParPermit_Unit WRITE setDigParPermit_Unit)
    Q_PROPERTY(bool DigParPermitInvert READ DigParPermitInvert WRITE setDigParPermitInvert)
    Q_PROPERTY(QString TooplTip_EnabledBtn READ TooplTip_EnabledBtn WRITE setTooplTip_EnabledBtn)
    Q_PROPERTY(QString TooplTip_DisabledBtn READ TooplTip_DisabledBtn WRITE setTooplTip_DisabledBtn)
    Q_PROPERTY(ConfirmTypes ConfirmType READ ConfirmType WRITE setConfirmType)
    Q_PROPERTY(QString ConfirmText READ ConfirmText WRITE setConfirmText)
    Q_PROPERTY(QString BtnCmd_Tag READ BtnCmd_Tag WRITE setBtnCmd_Tag)
    Q_PROPERTY(QString BtnCmd_Unit READ BtnCmd_Unit WRITE setBtnCmd_Unit)

    Q_PROPERTY(QString Help READ Help WRITE setHelp)
    Q_PROPERTY(QString BtnState_Tag READ BtnState_Tag WRITE setBtnState_Tag)
    Q_PROPERTY(QString BtnState_Unit READ BtnState_Unit WRITE setBtnState_Unit)
    Q_PROPERTY(bool BtnStateInvert READ BtnStateInvert WRITE setBtnStateInvert)


public:
    enum DisplayOption {Text_Only,Picture_Only};
    enum ConfirmTypes {Disabled,Standart,Other};
    enum {Type = SSControlButtonType};

    Q_ENUM(DisplayOption)
    Q_ENUM(ConfirmTypes)

    explicit SSControlButton(const QRect &rect);
    ~SSControlButton();
    int type() const { return Type; }

    //свойства
    DisplayOption DisplayOptions() const { return m_DisplayOptions; }
    QString PictureEnabled() const { return m_PictureEnabled;}
    QString PictureDisabled() const { return m_PictureDisabled;}
    QString Text() const { return m_Text;}
    QFont Font() const { return m_Font;}
    //QString UnitName() const { return m_UnitName; }
    //int MnemoNum() const { return m_MnemoNum; }
    //bool IsNewWnd() const { return m_IsNewWnd; }
    QString DigParPermit_Tag(){return m_DigParPermit_Tag;}
    QString DigParPermit_Unit(){return m_DigParPermit_Unit;}
    bool DigParPermitInvert(){return m_DigParPermitInvert;}
    QString TooplTip_EnabledBtn(){return m_TooplTip_EnabledBtn;}
    QString TooplTip_DisabledBtn(){return m_TooplTip_DisabledBtn;}
    ConfirmTypes ConfirmType(){return m_ConfirmType;}
    QString ConfirmText(){return m_ConfirmText;}
    QString BtnCmd_Tag(){return m_BtnCmd_Tag;}
    QString BtnCmd_Unit(){return m_BtnCmd_Unit;}

    QString Help() const { return m_Help; }
    QString BtnState_Tag() const { return m_BtnState_Tag; }
    QString BtnState_Unit() const { return m_BtnState_Unit; }
    bool BtnStateInvert() const { return m_BtnStateInvert; }
    bool PictureEnabledTransparent() const { return m_PictureEnabledTransparent; }
    bool PictureDisabledTransparent() const { return m_PictureDisabledTransparent; }

    //ввод-вывод в поток
    QDataStream& print(QDataStream& out) const;

signals:
    void dirty();

public slots:
    //свойства
    void setDisplayOptions(DisplayOption value) {
        if(value != m_DisplayOptions){
            m_DisplayOptions=value;
            update();
            emit dirty();}
    }
    void setPictureEnabled(const QString& value){
        if(value != m_PictureEnabled){m_PictureEnabled=value;emit dirty();}
    }
    void setPictureDisabled(const QString& value){
        if(value != m_PictureDisabled){m_PictureDisabled=value;emit dirty();}
    }
    void setText(const QString& value){
        if(value != m_Text){
            m_Text=value;
            update();
            emit dirty();}
    }
    void setFont(const QFont& value){
        if(value != m_Font){
            m_Font=value;
            update();
            emit dirty();}
    }
    /*
    void setUnitName(const QString& value){
        if(value != m_UnitName){m_UnitName=value;emit dirty();}
    }
    void setMnemoNum(const int& value){
        if(value != m_MnemoNum){m_MnemoNum=value;emit dirty();}
    }
    void setNewWnd(const bool& value){
        if(value != m_IsNewWnd){m_IsNewWnd=value;emit dirty();}
    }
    */
    void setDigParPermit_Tag(const QString& value) {
        if(value != m_DigParPermit_Tag){m_DigParPermit_Tag=value;emit dirty();}
    }
    void setDigParPermit_Unit(const QString& value) {
        if(value != m_DigParPermit_Unit){m_DigParPermit_Unit=value;emit dirty();}
    }
    void setDigParPermitInvert(const bool& value) {
        if(value != m_DigParPermitInvert){m_DigParPermitInvert=value;emit dirty();}
    }
    void setTooplTip_EnabledBtn(const QString& value) {
        if(value != m_TooplTip_EnabledBtn){m_TooplTip_EnabledBtn=value;emit dirty();}
    }
    void setTooplTip_DisabledBtn(const QString& value) {
        if(value != m_TooplTip_DisabledBtn){m_TooplTip_DisabledBtn=value;emit dirty();}
    }
    void setConfirmType(ConfirmTypes value) {
        if(value != m_ConfirmType){m_ConfirmType=value;emit dirty();}
    }
    void setConfirmText(const QString& value) {
        if(value != m_ConfirmText){m_ConfirmText=value;emit dirty();}
    }
    void setBtnCmd_Tag(const QString& value) {
        if(value != m_BtnCmd_Tag){m_BtnCmd_Tag=value;emit dirty();}
    }
    void setBtnCmd_Unit(const QString& value) {
        if(value != m_BtnCmd_Unit){m_BtnCmd_Unit=value;emit dirty();}
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



protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintButton(QPainter *painter);

private:
    void updateTransform();
    QPixmap* PrivateTitleImage;
    QStaticText label;

    //свойства
    DisplayOption m_DisplayOptions;
    QString m_PictureEnabled;
    QString m_PictureDisabled;
    QFont m_Font;
    QString m_Text;
    //QString m_UnitName;
    //int m_MnemoNum;
    //bool m_IsNewWnd;
    QString m_DigParPermit_Tag;
    QString m_DigParPermit_Unit;
    bool m_DigParPermitInvert;
    QString m_TooplTip_EnabledBtn;
    QString m_TooplTip_DisabledBtn;
    ConfirmTypes m_ConfirmType;
    QString m_ConfirmText;
    QString m_BtnCmd_Tag;
    QString m_BtnCmd_Unit;
    QString m_Help;
    QString m_BtnState_Tag;
    QString m_BtnState_Unit;
    bool m_BtnStateInvert;
    bool m_PictureEnabledTransparent;
    bool m_PictureDisabledTransparent;
};

#endif // SSCONTROLBUTTON_H
