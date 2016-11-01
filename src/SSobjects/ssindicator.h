#ifndef SSINDICATOR_H
#define SSINDICATOR_H

#include <QGraphicsPixmapItem>
#include "../itemtypes.hpp"

class StandardTableModel;

class SSindicator: public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString TemplateName READ TemplateName WRITE setTemplateName)
public:
    enum {Type = SSIndItemType};
    int type() const { return Type; }

    explicit SSindicator(const QPoint &position, QGraphicsScene *scene, const StandardTableModel& sourceModel, const QString &n_TemplateName, QGraphicsItem *parent=nullptr);
    QString TemplateName() const { return m_TemplateName; }

    void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
private:
    QPixmap* image;
    QString m_TemplateName;
    const StandardTableModel& model;
    void paintSelectionOutline(QPainter *painter);
public slots:
    void setTemplateName(const QString& newName);
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
signals:
    void dirty();
};

#endif // SSINDICATOR_H
