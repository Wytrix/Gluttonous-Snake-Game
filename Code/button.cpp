#include "button.h"
#include <QPainterPath>
#include <QPainter>
#include <QColor>
#include <QGraphicsSceneEvent>
#include <QObject>

Button::Button(QString txt,int x,int y,int ww,int hh,int s,int r,int g, int b,int a,int lr,int lg,int lb)
{
    setPos(x,y);
    text = txt;
    w = ww;
    h = hh;
    size = s;
    rr = r, gg = g, bb = b, aa = a, liner = lr, lineg =lg, lineb = lb;
    eff = new QGraphicsDropShadowEffect;
    eff->setBlurRadius(20);
    eff->setOffset(10);
    eff->setColor(QColor(r,g,b,100));
    this->setGraphicsEffect(eff);
    hasval = false;
}
QRectF Button::boundingRect() const
{
    return QRectF(- w / 2 - 5, - h / 2 -5, w + 10, h + 10);
}
QPainterPath Button::shape() const
{
    QPainterPath path;
    path.addRoundedRect(- w / 2, - h / 2, w, h , 16 , 16);
    return path;
}
void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidthF(2);
    QColor color(rr,gg,bb,aa);
    painter->setPen(pen);
    painter->fillPath(shape(),color);
    painter->drawPath(shape());

    color.setRgb(liner,lineg,lineb);
    pen.setColor(color);
    painter->setPen(pen);

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPointSize(size);
    font.setWeight(10);

    painter->setFont(font);
    painter->drawText(QRectF(- w / 2, - h / 2, w, h),Qt::AlignCenter,text);

    painter->restore();
}

void Button::setSize(int s)
{
    size = s;
}

void Button::setHasVal(bool b)
{
    hasval = b;
}
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(contains(QPointF(event->pos())))
        {
            setScale(0.9);
        }
    }
    //QGraphicsItem::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        setScale(10 / 9);
        if(contains(event->pos()))
        {
            emit ButtonIsReleased();
            if(hasval)emit sendVal(this);
        }
    }
    //QGraphicsItem::mouseReleaseEvent(event);
}
