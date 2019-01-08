#include "ListBuddy.h"
#include <QPainter>
ListBuddy::ListBuddy(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}
//初始化Ui
void ListBuddy::initUi()
{
    //初始化
    head=new QWidget(this);
    name=new QLabel(this);
//    sign=new QLabel(this);
    //设置头像大小
    head->setFixedSize(25,25);
//    //设置个性签名字体为灰色
//    QPalette color;
//    color.setColor(QPalette::Text,Qt::gray);
//    sign->setPalette(color);
    //布局
    head->move(5,9);
    name->move(35,16);
//    sign->move(54,27);
    //装载事件过滤器
    head->installEventFilter(this);
}
//事件过滤器，主要是为了让图片能够全部填充在head里面
bool ListBuddy::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == head)
    {
        if(event->type() == QEvent::Paint)
        {
            QPainter painter(head);
            painter.drawPixmap(head->rect(), QPixmap(headPath));
        }
    }
    return QWidget::eventFilter(obj, event);
}
