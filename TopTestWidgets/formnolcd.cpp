#include "formnolcd.h"
#include "ui_formnolcd.h"

FormNoLCD::FormNoLCD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNoLCD)
{
    ui->setupUi(this);
    setNo(0);
}

FormNoLCD::~FormNoLCD()
{
    delete ui;
}

void FormNoLCD::setLCDValue(double value)
{
    ui->lcdNumber->display(value);
}

int FormNoLCD::getNo() const
{
    return no;
}

void FormNoLCD::setLCDVisiable(bool show)
{

}

void FormNoLCD::setNo(int no)
{
    ui->label->setText(QString::number(no));
}
