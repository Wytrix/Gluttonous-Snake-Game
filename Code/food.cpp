#include <QPainter>
#include <QColor>
#include <QTime>

#include "constants.h"
#include "food.h"

static const qreal FOOD_RADIUS = 4;

Food::Food(qreal x, qreal y, QColor c)
{
    setPos(x, y);
    color = new QColor(c);
    setData(GD_Type, GO_Food);
}

QRectF Food::boundingRect() const
{
    return QRectF(-TILE_SIZE,    -TILE_SIZE,
                   TILE_SIZE * 2, TILE_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillPath(shape(), *color);

    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(TILE_SIZE / 2, TILE_SIZE / 2), FOOD_RADIUS, FOOD_RADIUS);
    return p;
}
