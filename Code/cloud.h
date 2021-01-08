#ifndef CLOUD_H
#define CLOUD_H

#include <QGraphicsPixmapItem>

class Cloud : public QGraphicsPixmapItem
{
public:
    Cloud(int x,int y,int w,int h,QString &file);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CLOUD_H
