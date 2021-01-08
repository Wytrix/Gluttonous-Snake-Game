#include "cloud.h"
#include <QPainter>

Cloud::Cloud(int x,int y,int w,int h,QString &file)
{
    setPos(x,y);
    QPixmap pixmap;
    pixmap.load(file);
    pixmap = pixmap.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(pixmap);
}

QRectF Cloud::boundingRect() const
{
    QRect rect = this->pixmap().rect();
    return QRectF(0,0,rect.width(),rect.height());
}

void Cloud::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap = this->pixmap();
    QRect rect = pixmap.rect();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPixmap(rect,pixmap);
}

QPainterPath Cloud::shape() const
{
    QRectF rect = boundingRect();
    QPainterPath path;
    path.addRoundRect(rect,5,5);
    return path;
}
