#include <QPainter>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"

static const qreal SNAKE_SIZE = 20;

Snake::Snake(GameController &controller,int c) :
    head(0, 0),
    growing(30),
    speed(2),
    score(0),
    color(c),
    moveDirection(NoMove),
    controller(controller)
{

}

QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach (QPointF p, tail) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x(),  // x
                          tl.y(),  // y
                          br.x() - tl.x() + SNAKE_SIZE,      // width
                          br.y() - tl.y() + SNAKE_SIZE       //height
                          );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addEllipse(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));
    foreach (QPointF p, tail) {
        QPointF itemp = mapFromScene(p);
        path.addEllipse(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QColor Color;
    if(color == 1)
        Color.setRgb(0xFF,0x3E,0x96);
    else if(color == 2)
        Color.setRgb(0xFF,0xFF,0);
    else if(color == 3)
        Color.setRgb(0x87,0xCE,0xFF);
    else if(color == 4)
        Color.setRgb(0x4E,0xEE,0x94);

    painter->fillPath(shape(),Color);

    QPainterPath path1;
    switch (moveDirection) {
    case MoveLeft:{
        path1.addEllipse(QRectF(0, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE));
        path1.addEllipse(0, SNAKE_SIZE * 0.6, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE);
        break;
    }
    case MoveRight:{
        path1.addEllipse(QRectF(SNAKE_SIZE * 0.6, SNAKE_SIZE * 0.6, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE));
        path1.addEllipse(SNAKE_SIZE * 0.6, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE);
        break;
    }
    case MoveUp:{
        path1.addEllipse(QRectF(0, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE));
        path1.addEllipse(SNAKE_SIZE * 0.6, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE);
        break;
    }
    case MoveDown:{
        path1.addEllipse(QRectF(0, SNAKE_SIZE * 0.6, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE));
        path1.addEllipse(SNAKE_SIZE * 0.6, SNAKE_SIZE * 0.6, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE);
        break;
    }
    case NoMove:
    {
        path1.addEllipse(QRectF(0, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE));
        path1.addEllipse(SNAKE_SIZE * 0.6, 0, 0.4 * SNAKE_SIZE, 0.4 * SNAKE_SIZE);
        break;
    }

    }
    painter->fillPath(path1,Qt::black);

    QString string(QString("score : %1").arg(score));
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPointSizeF(SNAKE_SIZE);
    font.setWeight(SNAKE_SIZE / 2);
    painter->setFont(font);

    QPen pen(Qt::darkGreen,SNAKE_SIZE / 2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter->setPen(pen);

    painter->drawText(mapFromScene(200,-340),string);
    painter->restore();
}

void Snake::setMoveDirection(Direction direction)
{
    moveDirection = direction;
}

void Snake::advance(int step)
{
    if (!step) {
        return;
    }
    if (++tickCounter % speed != 0) {
        return;
    }
    if (moveDirection == NoMove) {
        return;
    }

    if (growing > 0) {
		QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    } else {
        tail.takeFirst();
        tail << head;
    }

    switch (moveDirection) {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
    }

    tickCounter = 0;
    setPos(head);
    handleCollisions();
}

void Snake::moveLeft()
{
    head.rx() -= SNAKE_SIZE / 4;
    if (head.rx() < -SCENE_SIZE) {
        head.rx() = SCENE_SIZE - SNAKE_SIZE;
    }
}

void Snake::moveRight()
{
    head.rx() += SNAKE_SIZE / 4;
    if (head.rx() > SCENE_SIZE - SNAKE_SIZE) {
        head.rx() = -SCENE_SIZE;
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE / 4;
    if (head.ry() < -SCENE_SIZE) {
        head.ry() = SCENE_SIZE - SNAKE_SIZE;
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE / 4;
    if (head.ry() > SCENE_SIZE - SNAKE_SIZE) {
        head.ry() = -SCENE_SIZE;
    }
}

void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    // Check collisions with other objects on screen
    foreach (QGraphicsItem *collidingItem, collisions) {
        if (collidingItem->data(GD_Type) == GO_Food) {
            // Let GameController handle the event by putting another apple
            controller.snakeAteFood(this, (Food *)collidingItem);
            score += 1;
            growing += 1;
        }
        else if(collidingItem->data(GD_Type) == GO_Wall){
            controller.snakeHitWall(this,(Wall *)collidingItem);
        }
    }

    // Check snake eating itself
    if (tail.contains(head)) {
        controller.snakeAteItself(this);
    }
}
