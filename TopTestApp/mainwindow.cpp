#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QCompleter>


inline void initToolButton(QToolButton* toolButton,
                           const QString & iconPath,
                           double scaledRatio = 1.00 )
{
    //设置背景图片的来源
    toolButton->setIcon(QIcon(iconPath));
    //设置填充按键这么大的
    toolButton->setIconSize(QSize(toolButton->width(),toolButton->height())*scaledRatio );
}

inline void initPixmapLabel(QLabel* label,
                           const QString & iconPath,
                           double scaledRatio = 1.00 )
{
    //设置背景图片的来源
    label->setPixmap(QPixmap(iconPath).scaledToHeight(label->height()* scaledRatio));
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

        this->setStyleSheet(this->styleSheet()+
                            "QToolButton"
                            "{"
                            "border-style: transparent;"
                            "border-width: 5px;"
                            "border-radius: 5px;"
                            "border-color: #ffffff;"
                            "}"
                            "QToolButton:hover:enabled"
                            "{"
                            "border: 0px transparent;"
                            ""
                            "}"
                            "QMainWindow#MainWindow"
                            "{"
                            "border-image: url(:/images/bg_beijing.png);"
                            "}"
                            "QWidget#widget_way"
                            "{"
                            "border-image: url(:/images/bg_beijingkuang.png);"
                            "}"
                            "QToolButton#toolButton_login"
                            "{"
                            "border-radius: 15px;"
                            "}"
                            "QLabel#label_login"
                            "{"
                            "color:blue;"
                            "font-weight:bold;"
                            "}"
                            "QLabel#label_login_error"
                            "{"
                            "color:red;"
                            "}"
                            "QLabel#label_login_success"
                            "{"
                            "color:red;"
                            "font-size:20px;"
                            "font-weight:bold;"
                            "}"
                            "QToolButton#toolButton_login"
                            "{"
                            "color:white;"
                            "font-weight:bold;"
                            "}"
                            );

    ///init tool button
    initToolButton(ui->toolButton_way1,":/images/ic_quxianfenxi.png");
    initToolButton(ui->toolButton_way2,":/images/ic_zaixianfenxi.png");
    initToolButton(ui->toolButton_way3,":/images/ic_wuxianjiance.png");
    initToolButton(ui->toolButton_way4,":/images/ic_xitongshezhi.png");
    initToolButton(ui->toolButton_way5,":/images/ic_dangqianlianjieliebiao.png");

    initPixmapLabel(ui->label_logo,":/images/ic_logo.png");

    ui->stackedWidget_login->setCurrentIndex(0);

    ui->label_login_error->hide();
    ui->widget_way->setEnabled(false);

    QCompleter * com = new QCompleter({"User1","User2"},this);
    com->setMaxVisibleItems(8);
    com->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_name->setCompleter(com);

    ui->lineEdit_passwd->setAttribute(Qt::WA_InputMethodEnabled, false);

//    initToolButton(ui->toolButton_login,":/images/btn_anniu.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_login_clicked(bool checked)
{
    qDebug()<<"on_toolButton_login_clicked";
    if(checkPasseord())
    {
        ui->widget_way->setEnabled(true);
        ui->stackedWidget_login->setCurrentIndex(1);
    }
    else
    {
        ui->label_login_error->show();
    }
}

bool MainWindow::checkPasseord()
{
    if(ui->lineEdit_name->text().isEmpty() ||
            ui->lineEdit_passwd->text().isEmpty())
    {
        return false;
    }
    else
        return true;
}
