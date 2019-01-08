#ifndef FORMFRAMELESSTITLEBAR_H
#define FORMFRAMELESSTITLEBAR_H

#include <QWidget>
//#include <QSystemTrayIcon>

namespace Ui {
class FormFramelessTitleBar;
}



class FormFramelessTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit FormFramelessTitleBar(QWidget *parent = 0);
    ~FormFramelessTitleBar();

    void setMainWidget(QWidget* mainWidget);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
//private slots:
//    void minWindow();

//    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

private:

//    QSystemTrayIcon* mSysTrayIcon;
    Ui::FormFramelessTitleBar *ui;
    QPoint oldPos;
};

#endif // FORMFRAMELESSTITLEBAR_H
