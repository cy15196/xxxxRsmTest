#include "dialogrsmii.h"
#include "ui_dialogrsmii.h"

DialogRSMII::DialogRSMII(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRSMII)
{
    ui->setupUi(this);
}

DialogRSMII::~DialogRSMII()
{
    delete ui;
}
