#ifndef SSABSTRACTGRAPHOBJ_H
#define SSABSTRACTGRAPHOBJ_H

#include <QObject>

class SSAbstractGraphObj : public QObject
{
    Q_OBJECT
public:
    explicit SSAbstractGraphObj(QObject *parent = 0);

signals:

public slots:
};

#endif // SSABSTRACTGRAPHOBJ_H