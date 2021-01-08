#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QRectF>

class GameController;

class Snake : public QGraphicsPixmapItem
{
public:
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Snake(GameController & controller,int c);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setMoveDirection(Direction direction);
    Direction moveDirection;
    qreal     score;
    int       speed;

protected:
    void advance(int step);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();

    QPointF        head;
    int            growing;
    int            color;
    QList<QPointF> tail;
    int            tickCounter;
    GameController &controller;
};

#endif // SNAKE_H
