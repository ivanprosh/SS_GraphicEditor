#ifndef POBJECT
#define POBJECT
#include <QGraphicsItem>
#include <QObject>
#include <QString>
#include <QPainter>
#include <QVariant>
#include <QTableWidget>
#include "pobjectproperty.h"
#include <QtGlobal>



namespace PObj {


enum TYPE_OBJECT{
     TYPE_OBJECT_NON,
     TYPE_OBJECT_BTN,
     TYPE_OBJECT_ANPAR,
     TYPE_OBJECT_IM,
     TYPE_OBJECT_TEXT,
};
}



typedef QList<QVariant> ListProp2;

class PObject : public QObject, public QGraphicsItem
{
Q_OBJECT
protected:

    QString TagName;

    int _width;      // ширина
    int _height;     // высота


   PObj::TYPE_OBJECT typeobject;



   static double startX;
   static double startY;

   static bool resizeItem;
   static int invX;
   static int invY;

   static int ind;




public:

   static PObject* curObject;

   PObjectPropertys ObjectPropertys;     // свойства объекта

   QList< PObjectPropertys > Prop2;
   int countColProp2;                       // количество колонок для дополнительных параметров


   PObject(float x, float y,float width, float height, PObj::TYPE_OBJECT type = PObj::TYPE_OBJECT_NON,QGraphicsItem * parent = 0 );
   virtual void AddProp2(){}
   virtual void DelProp2(int index){ Prop2.removeAt(index); }

   virtual void paint(QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget);

   virtual PObject* ObjectCopy(){
                            PObject *obg = new PObject(x(), y(), _width, _height, typeobject );
                            obg->ObjectPropertys = ObjectPropertys;
                            return obg;
                          };

   int countRow();

   QRectF boundingRect() const ;

   PObj::TYPE_OBJECT typeObject(){return typeobject;}



   // QGraphicsItem interface
protected:
   QVariant itemChange(GraphicsItemChange change, const QVariant &value);

   virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};


//---------------------------------------------------------------------------------------
class PObjectAnPar : public PObject
{
Q_OBJECT
public:

   PObjectAnPar(float x, float y,float width, float height, QGraphicsItem * parent = 0);

   PObject* ObjectCopy(){
                            PObjectAnPar *obg = new PObjectAnPar(x(), y(), _width, _height );
                            obg->ObjectPropertys = ObjectPropertys;
                            Prop2.clear();
                            //qDebug("   PObjectAnPar->ObjectCopy;");
                            return obg;
                          }


   void paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget);

};


class PObjectTaxt : public PObject
{
Q_OBJECT
public:

   PObjectTaxt(float x, float y,float width, float height,QGraphicsItem * parent = 0);

   PObject* ObjectCopy(){
                            PObjectTaxt *obg = new PObjectTaxt(x(), y(), _width, _height );
                            obg->ObjectPropertys = ObjectPropertys;
                            obg->Prop2 = Prop2;
                            return obg;
                          }

   virtual void AddProp2();

   void paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget);


};

class PObjectButton: public PObject
{
Q_OBJECT
public:

   PObjectButton(float x, float y,float width, float height,QGraphicsItem * parent = 0);

   PObject* ObjectCopy(){
                            PObjectButton *obg = new PObjectButton(x(), y(), _width, _height );
                            obg->ObjectPropertys = ObjectPropertys;
                            Prop2.clear();
                            return obg;
                          }

   void paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget);


};

class PObjectIM: public PObject
{
Q_OBJECT
public:

   PObjectIM(float x, float y,float width, float height,QGraphicsItem * parent = 0);

   PObject* ObjectCopy(){
                            PObjectIM *obg = new PObjectIM(x(), y(), _width, _height );
                            obg->ObjectPropertys = ObjectPropertys;
                            obg->countColProp2 = countColProp2;
                            obg->Prop2 = Prop2;
                            return obg;
                          }
   void AddProp2();

   void paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget);


};


#endif // POBJECT

