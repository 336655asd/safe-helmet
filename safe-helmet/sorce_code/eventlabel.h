#ifndef EVENTLABEL_H
#define EVENTLABEL_H
#include <QMouseEvent>
#include <QLabel>
class EventLabel:public QLabel
{
public:
    EventLabel();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // EVENTLABEL_H
