#include "formdialogheader.h"
#include "ui_formdialogheader.h"

FormDialogHeader::FormDialogHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDialogHeader)
{
    ui->setupUi(this);
    connect(this,SIGNAL(windowTitleChanged(QString)),
           ui->label_title,SLOT(setText(QString)));

    ui->pushButton_close->setIconSize(QSize(25,25));
    ui->pushButton_close->setIcon(QIcon(":/images/ic_guanbi.png"));
    this->setStyleSheet(this->styleSheet()+
                        "QWidget#widget{"
                        "border-style: solid;"
                        "border-width: 0px 0px 1px 0px;"
                        "border-color: #cfd7e3;"
                        "background-color: #567cdd;"
                        "}"
                        "QLabel#label_title{"
                        "color:white;"
                        "}"
                        "QPushButton"
                        "{"
                        "border: 2px transparent;"
                        "background-color:#567cdd;"
                        "}"
                        );
}

FormDialogHeader::~FormDialogHeader()
{
    delete ui;
}

void FormDialogHeader::setTitle(const QString &str)
{
    ui->label_title->setText(str);
}

void FormDialogHeader::on_pushButton_close_clicked()
{
    emit this->needToCloseDialog();
}
