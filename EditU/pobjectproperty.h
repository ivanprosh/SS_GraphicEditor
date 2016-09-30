#ifndef POBJECTPROPERTY
#define POBJECTPROPERTY
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QStringList>
#include <QPixmap>


enum type_property{
    TYPE_PROP_ANPAR,
    TYPE_PROP_TEXT,
    TYPE_PROP_TEXT_STATE,
    TYPE_PROP_BUTTON,
    TYPE_PROP_IM,
    TYPE_PROP_IM_STATE,


};


typedef QList<QVariant> QListObjectPropertys;


class PObjectPropertys {

private:
    QListObjectPropertys items; // список свойств

    static QList<QStringList> namesItems;   // наименования параметров

    type_property typeProp;

public:

    static void initNamesItems();   // инициализация наименований параметров

    QString nameItem(int index) const
    {
        return namesItems.at(typeProp).at(index);
    };

    QVariant item(int index) const
    {
        return items.at(index);
    };

    void setItem(int index, QVariant value){
         items.replace(index, value);
    };

    int count(){
        return items.count();
    };

    void push_back(QVariant value){
        items.push_back(value);
    }

    void setType(type_property type){
        typeProp = type;
    };



};

class QPixmapU : public QPixmap
{
 private:
    QString path_;
 public:
    QPixmapU(const QString& path)
        :QPixmap(path)
    {
         path_ = path;
    }

    QString path(); // getter

};


#endif // POBJECTPROPERTY


