#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QGraphicsEffect>

class Button : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Button(QString text,int x,int y,int w,int h,int s = 14,int r = 30,int g = 160,int b = 40,int a = 255,int lr = 0,int lg = 0,int lb = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setSize(int s);
    void setHasVal(bool b);

signals:
    void ButtonIsReleased();
    void sendVal(Button *val);

private:
    QString text;
    QGraphicsDropShadowEffect *eff;
    QGraphicsScene *scene;
    int w,h;
    int size;
    int rr,gg,bb,aa,liner,lineg,lineb;
    bool hasval;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BUTTON_H
