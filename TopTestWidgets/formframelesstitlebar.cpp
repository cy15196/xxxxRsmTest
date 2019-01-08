#include "formframelesstitlebar.h"
#include "ui_formframelesstitlebar.h"

//#include <QSystemTrayIcon>

#include <QMouseEvent>
#include <QDebug>

FormFramelessTitleBar::FormFramelessTitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFramelessTitleBar)
{
    ui->setupUi(this);
    setMainWidget(topLevelWidget());
//    mSysTrayIcon = 0;
}

FormFramelessTitleBar::~FormFramelessTitleBar()
{
    delete ui;
}

void FormFramelessTitleBar::setMainWidget(QWidget *mainWidget)
{
    ui->label->setText(mainWidget->windowTitle());

    connect(ui->toolButton_close,SIGNAL(clicked(bool)),
            mainWidget,SLOT(close()));
    connect(ui->toolButton_min,SIGNAL(clicked(bool)),
            mainWidget,SLOT(showMinimized()));

    ui->toolButton_resize->setVisible(false);
}

void FormFramelessTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"FormFramelessTitleBar::mouseMoveEvent"<<event;
//    if(event->button() == Qt::LeftButton)
//    {
//        QPoint nowPos = event->pos();
//        QPoint movePoint = topLevelWidget()->pos() + nowPos - oldPos;
//        topLevelWidget()->move(movePoint);
////        oldPos = nowPos;
//    }

//    QWidget::mouseMoveEvent(event);
}

void FormFramelessTitleBar::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"FormFramelessTitleBar::mousePressEvent"<<event;
//    if(event->button() == Qt::LeftButton)
//        oldPos = event->pos();
    QWidget::mousePressEvent(event);
}

//void FormFramelessTitleBar::minWindow()
//{
//    QWidget* topWidget = topLevelWidget();
//    topWidget->showMinimized();

////    //新建QSystemTrayIcon对象
////    mSysTrayIcon = new QSystemTrayIcon(this);
////    //将icon设到QSystemTrayIcon对象中
////    mSysTrayIcon->setIcon(topWidget->windowIcon());
////    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
////    mSysTrayIcon->setToolTip(QObject::trUtf8("测试系统托盘图标"));
////    //给QSystemTrayIcon添加槽函数
////    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

////    //建立托盘操作的菜单
//////    createActions();
//////    createMenu();
////    //在系统托盘显示此对象
////    mSysTrayIcon->show();
//}

//void FormFramelessTitleBar::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
//{
//    switch(reason){
//    case QSystemTrayIcon::Trigger:
//        topLevelWidget()->show();
//        break;
//    default:
//        break;
//    }
//}
