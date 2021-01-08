#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsItem>
#include <QColor>

class Food : public QGraphicsItem
{
public:
    Food(qreal x, qreal y, QColor c);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    QPainterPath shape() const;
private:
    QColor *color;
};

#endif // FOOD_H
