#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP
/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include <QGraphicsView>
#include <QPainter>
#include <QWheelEvent>
#include <cmath>


class GraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(bool grid READ grid WRITE setGrid)

private:
    bool m_grid;
    int m_gridsize;
    bool grid(){return m_grid;}
public:
    explicit GraphicsView(QWidget *parent=0) : QGraphicsView(parent), m_grid(1),m_gridsize(40)
    {
        setDragMode(RubberBandDrag);
        setRenderHints(QPainter::Antialiasing|
                       QPainter::TextAntialiasing);
    }

public slots:
    void zoomIn() { scaleBy(1.1); }
    void zoomOut() { scaleBy(1.0 / 1.1); }
    void setGrid(bool value) {
        m_grid = value;
        repaint();
    }
    void setGridSize(int value) {
        m_gridsize = value;
        repaint();
    }

protected:
    void wheelEvent(QWheelEvent *event)
    {
        scaleBy(std::pow(4.0 / 3.0, (event->delta() / 240.0)));
        repaint();
    }
    void drawBackground(QPainter *painter, const QRectF &rect)
    {
        if(m_grid){
            //const int GridSize = 40;
            QPen pen(QColor(175, 175, 175, 127));
            painter->setPen(pen);

            const int MaxX = static_cast<int>(std::ceil(rect.width())
                                              / m_gridsize) * m_gridsize;
            const int MaxY = static_cast<int>(std::ceil(rect.height())
                                              / m_gridsize) * m_gridsize;
            for (int x = 0; x <= MaxX; x += m_gridsize) {
                painter->drawLine(x, 0, x,MaxY);
            }
            for (int y = 0; y <= MaxY; y += m_gridsize) {
                painter->drawLine(0, y,MaxX, y);
            }
        }
        QGraphicsView::drawBackground(painter, rect);
    }

private:
    void scaleBy(double factor) { scale(factor, factor); }
};

#endif // GRAPHICSVIEW_HPP
