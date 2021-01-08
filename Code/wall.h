#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>

class Wall : public QGraphicsItem
{
public:
    Wall(int x,int y);
private:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // WALL_H
