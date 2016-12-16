#include "ssdyntext.h"

SSDynText::SSDynText()
{

}

void SSDynText::updateDynamicPropView(QString propSingleName, QHash<QString,SScommandProperty>& hash,int count){

    int index;
    for(index=0;index<count;index++){
       QString propName(propSingleName+QString::number(index+1));
       if(!hash.contains(propName)){
           hash.insert(propName,qMakePair(QString(),QString()));
       }
       setProperty(propName.toUtf8(),QVariant::fromValue(hash.value(propName)));
    }
    QRegExp rgxPattern(propSingleName+"([0-9])");
    QListIterator<QByteArray> iter(dynamicPropertyNames());
    while(iter.hasNext()){
        if(rgxPattern.exactMatch(iter.next()) && rgxPattern.cap(1).toInt() > count)
        {
            setProperty(QString(propSingleName+rgxPattern.cap(1)).toUtf8(),QVariant());
        }
    }

    qDebug() << dynamicPropertyNames();

}
