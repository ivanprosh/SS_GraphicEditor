#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPainter>

class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CheckBoxDelegate(QObject *parent = 0);
    ~CheckBoxDelegate(){delete cheсkbox;}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    /*
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    */
private:
    QCheckBox* cheсkbox;
    void paintWidget(QPainter *painter, const QRect &rect, const QString &cacheKey, QWidget *widget) const;
};

#endif // CHECKBOXDELEGATE_H
