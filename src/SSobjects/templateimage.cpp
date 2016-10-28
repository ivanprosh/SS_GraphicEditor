#include "templateimage.h"
#include <QDebug>

void TemplateImage::mousePressEvent(QMouseEvent *me)
{
   if(me->buttons() & Qt::LeftButton)
   {
       QPainter painter(this);
       QString path = QFileDialog::getOpenFileName(0,tr("Choose image"),"","*.png *.jpg *.bmp");
       if(path.isEmpty()) return;

       QPixmap img;
       img.load(path);
       img = img.scaled(this->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
       this->setPixmap(img);
       QFileInfo fileInfo(path);
       qDebug() << "File " << fileInfo.fileName();
       emit imageChanged(fileInfo.fileName());
   }
}

