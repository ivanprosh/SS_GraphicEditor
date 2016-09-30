#include "PObject.h"
#include <QGraphicsSceneMouseEvent>
#include "itemdelegate.h"
#include <QMetaType>
#include <QColor>
#include <QPixmap>
#include "qlistmodel.h"
#include "prop2model.h"
#include "qdebug.h"


double PObject::startX;
double PObject::startY;


bool PObject::resizeItem = false;
int PObject::invX;
int PObject::invY;
int PObject::ind;

PObject* PObject::curObject = NULL;

struct vector{
    int x;
    int y;
};

vector point_F[]={{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1}, {-1,1}, {-1, 0} };


#define SIZE_REG 4

// �����������
PObject::PObject(float x, float y, float width, float height, PObj::TYPE_OBJECT type, QGraphicsItem *parent)
    :_width(width),_height(height),QGraphicsItem(parent), typeobject(type)
{
    setX(x);
    setY(y);
}

// ��������� �������
void PObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (isSelected()){
        painter->setBrush(QBrush(Qt::gray, Qt::NoBrush));
        painter->setPen(QPen(Qt::red,1,Qt::DashLine));
                painter->drawRect( QRectF( -1, -1, _width + 2 , _height + 2) );
    }
}

int PObject::countRow()
{
     return ObjectPropertys.count();
}

QRectF PObject::boundingRect() const
{
    return QRectF(-3,-3, _width + 3, _height + 3);
}


QVariant PObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}



void PObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
   if ( event->button() == Qt::LeftButton ){
        PObject::resizeItem = false;

        for (int i=0; i< 8; i++){
            int xp = x() + (point_F[i].x+1) * _width/2;
            int yp = y() + (point_F[i].y+1) * _height/2;

            if ( abs(xp - event->scenePos().x())< SIZE_REG &&
                 abs(yp - event->scenePos().y())< SIZE_REG )
            {
                int n = i + 4; if(n>7) n -= 8;
                PObject::resizeItem = true;
                PObject::startX = x() + (point_F[n].x+1) * _width/2;
                PObject::startY = y() + (point_F[n].y+1) * _height/2;

                PObject::ind = i;

            }

        }
   }
       PObject::curObject = this;
       QListModel::listModel->setObject(this);
       if (typeObject() == PObj::TYPE_OBJECT_TEXT || typeObject() == PObj::TYPE_OBJECT_IM)
       {
            //Prop2Model::listModel->setHeader(&PObject::HeaderText);
            Prop2Model::listModel->setObject(this);
       }
       else
          Prop2Model::listModel->setObject(NULL);

   if(!PObject::resizeItem)
   QGraphicsItem::mousePressEvent(event);
}


void PObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(PObject::resizeItem)
    {
        prepareGeometryChange();

        if (point_F[PObject::ind].x!=0)
                _width = event->scenePos().x() - PObject::startX  ;

        if (_width<0) {
            _width =-_width;
            setX( PObject::startX - _width);
        }

        if (point_F[PObject::ind].y!=0)
                _height = event->scenePos().y() - PObject::startY  ;


        if (_height<0) {
            _height =-_height;
            setY( PObject::startY - _height);
        }
    }
    update();

    if(!PObject::resizeItem)
        QGraphicsItem::mouseMoveEvent(event);

}


void PObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    PObject::resizeItem = false;
    QGraphicsItem::mouseReleaseEvent(event);
}


//---------------------------------------------------------------------------------------


// ����������� ������
PObjectAnPar::PObjectAnPar(float x, float y,float width, float height,QGraphicsItem * parent):
    PObject(x,y,width,height,PObj::TYPE_OBJECT_ANPAR, parent)
{
    ObjectPropertys.push_back( QVariant ("Tag_AnPar"));	//���
    ObjectPropertys.push_back( QVariant ("")        );	//<���. ��� ��������>
    ObjectPropertys.push_back( QVariant ("Tag_prm") );	//���������� ��������, ����������� ��������� ��������
    ObjectPropertys.push_back( QVariant ("")        );	//<���. ��� ��������>
    ObjectPropertys.push_back( QVariant (false)     );	//������������ ���������� ��� ��� (�� ��������� 0)
    ObjectPropertys.push_back( QVariant (0)         );	//������ ������� ����������� ������������ (�� ��������� 0)
    ObjectPropertys.push_back( QVariant( QColor(192,192,192) ) );	//���� ���� ����������� ������������ � ����������� ������� ����������� ���������
    ObjectPropertys.push_back( QVariant( QColor(0,0,0) )       );	//���� ������ ������������ � ����������� ������� ����������� ���������
    ObjectPropertys.push_back( QVariant( QFont("Arial") )      );	// ��������� ������
    ObjectPropertys.push_back( QVariant(4)                     );	//��������� ������������ ������������ ���� ��������: (�� ��������� 4)
    ObjectPropertys.push_back( QVariant("��. ��� ")            );	//������������ ����������� ������� (������ ������� � �������) (�� ��������� 4)
    ObjectPropertys.push_back( QVariant( QColor(0,0,0) )       );	//���� ���� �������� � ����������� ������� ����������� ��������� ��� �1, ���� ����������
    ObjectPropertys.push_back( QVariant( QColor(0,255,0) )     );	//���� ������ �������� � ����������� ������� ����������� ���������
    ObjectPropertys.push_back( QVariant( QFont("Arial") )      );	//��������� ������
    ObjectPropertys.push_back( QVariant(0)                     );	//��� ��������� ���������� (�� ��������� 0)

    ObjectPropertys.setType(TYPE_PROP_ANPAR);
}

void PObjectAnPar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // ������� �������������
    painter->setPen(QPen(Qt::gray,1,Qt::SolidLine));
    QColor BackColor = ObjectPropertys.item(11).value<QColor>();
    painter->setBrush( QBrush(BackColor, Qt::SolidPattern ));

    // ���� ������
    painter->drawRect( QRectF(1, 1, _width - 2, _height - 2) );
    QColor ForeColor =  ObjectPropertys.item(12).value<QColor>();
    painter->setPen(QPen(ForeColor));
    // �����
    QFont font = ObjectPropertys.item(13).value<QFont>();
    painter->setFont( font );
    // ����
    painter->drawText(QRectF(0, 0, _width, _height), Qt::AlignCenter , tr("99.99") ); //); op.at(13).value.toString() );

    PObject::paint(painter, option, widget);
};



PObjectTaxt::PObjectTaxt(float x, float y, float width, float height, QGraphicsItem *parent)
 :PObject(x,y,width,height,PObj::TYPE_OBJECT_TEXT,parent)
{
   ObjectPropertys.push_back(QVariant ("Text") );
   ObjectPropertys.setType(TYPE_PROP_TEXT);

   AddProp2();
   AddProp2();
   AddProp2();
}

void PObjectTaxt::AddProp2()
{
    PObjectPropertys list;

        list.push_back( QVariant("Tag"               ));
        list.push_back( QVariant(""                  ));
        list.push_back( QVariant("TEXT"              ));
        list.push_back( QVariant(QFont("Arial")      ));
        list.push_back( QVariant(QColor(255,255,255) ));
        list.push_back( QVariant(QColor(0,0,0)       ));
        list.push_back( QVariant(true                ));
        list.push_back( QVariant(false               ));
        list.push_back( QVariant(0                   ));
        list.push_back( QVariant(0                   ));
        list.setType(TYPE_PROP_TEXT_STATE);

    Prop2.push_back(list);
    countColProp2 = list.count();

}

void PObjectTaxt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::green,1,Qt::SolidLine));
    painter->setBrush( QBrush(QColor(10,100,100), Qt::SolidPattern ));
    painter->drawRect(QRectF(0, 0, _width, _height));
    painter->setPen(QPen(Qt::red,1,Qt::SolidLine));
    painter->drawText(QRectF(0, 0, _width, _height), Qt::AlignCenter , ObjectPropertys.item(0).toString() ); //); op.at(13).value.toString() );

    PObject::paint(painter, option, widget);
}


PObjectButton::PObjectButton(float x, float y, float width, float height, QGraphicsItem *parent)
    :PObject(x,y,width,height,PObj::TYPE_OBJECT_BTN,parent)
{
    ObjectPropertys.push_back(QVariant ("Help"));                   //������� �� ������
    ObjectPropertys.push_back(QVariant (0));                        //��� �������� �� ������: 0 � �����, 1 - �������
    ObjectPropertys.push_back(QVariant ("Enabled.bmp"));            //��� ����� ����������� ��� ��������� ������. ���� ���� � ������������� � ��� ������ ��������� ������� �_T�
    ObjectPropertys.push_back(QVariant ("Disabled.bmp"));           //��� ����� ����������� ��� ����������� ������. ���� ���� � ������������� � ��� ������ ��������� ������� �_T�
    ObjectPropertys.push_back(QVariant ("Text"));                   //�����, ��������� �� ������
    ObjectPropertys.push_back(QVariant (QFont("Arial")));           //��������� ������
    ObjectPropertys.push_back(QVariant ("StateTag"));               //���������� �������� ������� ������
    ObjectPropertys.push_back(QVariant (""));                       //<���. ��� ��������>
    ObjectPropertys.push_back(QVariant (""));                       //���. ��� ��������, �� ������� ������������ �������
    ObjectPropertys.push_back(QVariant (""));                       //���. ��� ����������, �� ������� ������������ �������
    ObjectPropertys.push_back(QVariant (0));                        //�������� �� ��������� ;0 � �������� ���������� �������� ���� ;1 � ������� ����� ����
    //��������� ������ ��� ������ ���� ����������� (��� �������� ��� ����� ���� �����������, ���� �� �����������)
    ObjectPropertys.push_back(QVariant ("PermitTag"));              //����������� ���������� ��������
    ObjectPropertys.push_back(QVariant (""));                       //;<���. ��� ��������>
    ObjectPropertys.push_back(QVariant ("ToolTip_Enabled "));  		//��������� ��� ��������� ������
    ObjectPropertys.push_back(QVariant ("ToolTip_Disabled"));		//��������� ��� ����������� ������
    ObjectPropertys.push_back(QVariant (1));                        //��������� ��� ����������  ;0 � ��� ���������� ;1 - ������� �����
    ObjectPropertys.push_back(QVariant ("CmdTag"));                 //���������� �������� ������� ������
    ObjectPropertys.push_back(QVariant (""));                       //<���. ��� ��������>

    ObjectPropertys.setType(TYPE_PROP_BUTTON);

    countColProp2 = 0;
}

void PObjectButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // ������� �������������
    painter->setPen(QPen(Qt::white,1,Qt::SolidLine));
    painter->setBrush( QBrush(QColor(212,208,200), Qt::SolidPattern ));
    painter->drawRect( QRectF(1, 1, _width - 2, _height - 2) );


    painter->setPen(QPen(Qt::black,1,Qt::SolidLine));
    painter->drawLine(1,_height - 1, _width - 2, _height - 1);
    painter->drawLine(_width - 2, 1, _width - 2, _height - 1);
    // �����
    QFont font = ObjectPropertys.item(5).value<QFont>();
    painter->setFont( font );
    // ����
    painter->drawText(QRectF(1, 1, _width -2, _height-2), Qt::AlignCenter , ObjectPropertys.item(4).toString() ); //); op.at(13).value.toString() );

    PObject::paint(painter, option, widget);

}

PObjectIM::PObjectIM(float x, float y, float width, float height, QGraphicsItem *parent)
    :PObject(x,y,width,height,PObj::TYPE_OBJECT_IM,parent)
{
    ObjectPropertys.push_back(QVariant (0));                // �������� ��������� ������ �� �����������
    ObjectPropertys.push_back(QVariant (0));                // �������� ��������� ������ �� ���������
    ObjectPropertys.push_back(QVariant (1));                // ���������� ��������
    ObjectPropertys.push_back(QVariant (0));                //����� ������
    ObjectPropertys.push_back(QVariant (0)); 		        //����� ��������� �������
    ObjectPropertys.push_back(QVariant (0)); 		        //����� ��������
    ObjectPropertys.push_back(QVariant (true));             //������� ����, ��� ������ �����
    ObjectPropertys.push_back(QVariant ("Help"));           //������� �� �� �������������� �������
    ObjectPropertys.push_back(QVariant ("NameIM"));         //��� �������������� �������
    ObjectPropertys.push_back(QVariant ("NameIM"));         //������� ��� �������������� ������� (�������)
    ObjectPropertys.push_back(QVariant (QColor(0,0,0)));    //���� ������������
    ObjectPropertys.push_back(QVariant (QFont("Arial")));   //�����
    ObjectPropertys.push_back(QVariant (4));                //��������� ������������
    ObjectPropertys.push_back(QVariant ("Tag"));            //���
    ObjectPropertys.push_back(QVariant (""));               //
    ObjectPropertys.push_back(QVariant (QColor(255,255,0)));// ���� �����
    ObjectPropertys.push_back(QVariant (1));                // ������� �����
    ObjectPropertys.push_back(QVariant (1000));             // ������ ������� �����, �� (0 - ��� �������)
    ObjectPropertys.push_back(QVariant ( QPixmapU("IM.bmp") ));      // ������ �������

    ObjectPropertys.setType(TYPE_PROP_IM);

    Prop2.clear();

    countColProp2 = 0;
}

void PObjectIM::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // ������� �������������
    painter->setPen(QPen(Qt::white,1,Qt::SolidLine));
    painter->setBrush( QBrush(QColor(212,208,200), Qt::SolidPattern ));
    painter->drawRect( QRectF(1, 1, _width - 2, _height - 2) );

    //QPixmap pm = ObjectPropertys.item(18).value<QPixmap>();
    painter->drawPixmap( 1, 1, _width - 2, _height - 2 , ObjectPropertys.item(18).value<QPixmap>() );



    PObject::paint(painter, option, widget);
}


void PObjectIM::AddProp2()
{
   PObjectPropertys list;

       list.push_back( QVariant(false               ));
       list.push_back( QVariant(500                 ));
       list.push_back( QVariant ( QPixmapU("IM.bmp")));
       list.push_back( QVariant ( QPixmapU("IM.bmp")));
       list.push_back( QVariant ( QPixmapU("IM.bmp")));
       list.push_back( QVariant ( QPixmapU("IM.bmp")));
       list.setType(TYPE_PROP_IM_STATE);

   Prop2.push_back(list);
   countColProp2 = list.count();

}
