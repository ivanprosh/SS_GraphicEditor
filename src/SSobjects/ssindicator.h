#ifndef SSINDICATOR_H
#define SSINDICATOR_H

#include <QGraphicsPixmapItem>
#include <QModelIndex>
#include "../itemtypes.hpp"

class StandardTableModel;

class SSindicator: public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString TemplateName READ TemplateName WRITE setTemplateName)
public:
    enum {Type = SSIndItemType};
    int type() const { return Type; }

    explicit SSindicator(const QPoint &position, QGraphicsScene *scene, const QModelIndex &Template_Index = QModelIndex(), QGraphicsItem *parent=nullptr);
    QString TemplateName() const { return m_TemplateName; }

    void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;

    friend QDataStream &operator<<(QDataStream &out, const SSindicator &ind);
    friend QDataStream &operator>>(QDataStream &in, SSindicator &ind);
private:
    QPixmap image;
    QString m_TemplateName;
    //const StandardTableModel* model;
    void paintSelectionOutline(QPainter *painter);
public slots:
    void setTemplateName(const QString& newName);
    void TemplateNameChanged(const QString& oldName,const QString& newName);
    void ImageChanged(const QString& ,const QPixmap& pix);
    //void ModelDataChanged(const QModelIndex& indexStart,const QModelIndex& indexFinish);
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
signals:
    void dirty();
};

#endif // SSINDICATOR_H
