#include "wall.h"
#include "constants.h"

#include <QPainter>

Wall::Wall(int x,int y)
{
    setPos(x, y);
    setData(GD_Type,GO_Wall);
}

QRectF Wall::boundingRect() const
{
    return QRectF(-WALL_SIZE / 2, -WALL_SIZE / 2, WALL_SIZE, WALL_SIZE);
}
QPainterPath Wall::shape() const
{
    QPainterPath path;
    path.addRoundedRect(0,0,WALL_SIZE,WALL_SIZE,4,4);
    return path;
}
void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(),Qt::gray);
    painter->restore();
}
