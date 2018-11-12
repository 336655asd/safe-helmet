#include "eventlabel.h"
#include <QLabel>
EventLabel::EventLabel()
{

}
void EventLabel:: mouseMoveEvent(QMouseEvent *event)
{
    //QLabel支持html代码，QString 中的arg函数可以替换占位符
    this->setText(QString("<center><h1>Move: (%1,%2)</h1></center>").arg(QString::number(event->x()),QString::number(event->y())));
}
void EventLabel::mousePressEvent(QMouseEvent *event)
{
    this->setText(QString("<center><h2>Press: (%1,%2)</h2></center>").arg(QString::number(event->x()),QString::number(event->y())));
}

void EventLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QString msg;
    msg.sprintf("<center><h3>Release :(%d,%d)</h3></center>",event->x(),event->y());
    this->setText(msg);
}
