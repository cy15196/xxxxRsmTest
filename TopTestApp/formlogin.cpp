#include "formlogin.h"
#include "ui_formlogin.h"


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


FormLogin::FormLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLogin)
{
    ui->setupUi(this);

    ui->lineEdit_name->setPlaceholderText(tr("Please input username"));
    ui->lineEdit_password->setPlaceholderText(tr("Please input password"));

    this->setStyleSheet(this->styleSheet()+
                        "QWidget#verticalWidget{"
                        "background-color:#ffffff;"
                        "}"
                        "QWidget#verticalWidget_2{"
                        "background-color:#ffffff;"
                        "}"
                        "QLabel#label_login{"
                        "font-size:20px;"
                        "}"
                        "QLabel#label_error{"
                        "font-size:20px;"
                        "}"
                        "QLineEdit{"
                        "font-size:20px;"
                        "}"
                        "QToolButton#toolButton_login{"
                        "font-size:20px;"
                        "background-color:#5a81e8;"
                        "opacity:0.7;"
                        "color:white;"
                        "}"
                        );

    this->setWindowFlags(Qt::FramelessWindowHint);

    initToolButton(ui->toolButton_hide,":/images/ic_yincang.png");
    initToolButton(ui->toolButton_clear,":/images/ic_guanbi.png");
    initToolButton(ui->toolButton_close,":/images/ic_guanbi.png");

    connect(ui->toolButton_close,SIGNAL(clicked(bool)),
            this,SLOT(close()));
}

FormLogin::~FormLogin()
{
    delete ui;
}
