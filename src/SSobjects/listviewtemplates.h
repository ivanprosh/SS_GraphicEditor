#ifndef LISTVIEWTEMPLATES_H
#define LISTVIEWTEMPLATES_H

#include <QListView>

class ListViewTemplates : public QListView
{
    Q_OBJECT

public:
    explicit ListViewTemplates(QObject *parent = 0):QListView(parent){}

signals:

public slots:

private:
    QString curLabel;
};

#endif // LISTVIEWTEMPLATES_H
