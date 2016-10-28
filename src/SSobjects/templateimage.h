#ifndef TEMPLATEIMAGE_H
#define TEMPLATEIMAGE_H

#include <QtWidgets>

class TemplateImage : public QLabel
{
    Q_OBJECT

public:
    explicit TemplateImage(QWidget *parent = 0):QLabel(parent){}

protected:
    virtual void mousePressEvent(QMouseEvent* me);

signals:
    void imageChanged(QString name);
public slots:
    //void setPixmap(const QPixmap&);
};

#endif // RECEPTIMAGE_H
