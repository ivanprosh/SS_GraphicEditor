#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "sscontrolbutton.h"

SSControlButton::SSControlButton(const QRect &_rect) : SSRectItem(_rect),label("C")
{
  setDisplayOptions(Text_Only);
  PrivateTitleImage = new QPixmap();
  //������-����� ���� ������
  QGraphicsRectItem::setBrush(QBrush(QColor(212,212,212),Qt::SolidPattern));
  QGraphicsRectItem::setPen(QPen(Qt::MiterJoin));
  //��������� �������� � ����
  QGraphicsItem::setCacheMode(DeviceCoordinateCache);
  /*
     ���������� �� ����� ������������ ����� ������� QUndoCommand ���
     ���������� ��������� �������-������ ��������
  */

  setSelected(true);

}
SSControlButton::~SSControlButton(){
    if(PrivateTitleImage) delete PrivateTitleImage;
}

void SSControlButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //������ ������ � ����
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    painter->setPen(QPen(Qt::black,4,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->drawRect(option->rect.x() + 2,option->rect.y() + 3,rect().width(),rect().height());

    if (DisplayOptions()==Text_Only){
        painter->setPen(QGraphicsRectItem::pen());
        painter->setBrush(QGraphicsRectItem::brush());
        painter->drawRect(rect());

        if (!Text().isEmpty()){
            painter->setFont(Font());
            painter->drawText(option->rect,Qt::AlignCenter,Text());
        }
    } else {
         if(PrivateTitleImage->load("resource/"+PictureEnabled()))
            painter->drawPixmap(option->rect,*PrivateTitleImage);
         else
            painter->drawPixmap(option->rect,QPixmap(":/images/obj_icons/default_obj.png"));
    }
    if (option->state & QStyle::State_Selected)
        paintSelectionOutline(painter);
    //������ ������������� ������
    painter->setPen(QPen(Qt::red,4));
    painter->drawStaticText(option->rect.x()+3,option->rect.y(),label);
}

QDataStream &SSControlButton::print(QDataStream &out) const
{
    //qDebug() << "In derived class";
    return SSRectItem::print(out);
}
