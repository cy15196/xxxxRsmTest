#include "dialogdeviceandprogresshanding.h"
#include "ui_dialogdeviceandprogresshanding.h"

#include "dialogdevicehandle.h"
#include "dialogprogresshandle.h"

DialogDeviceAndProgressHanding::DialogDeviceAndProgressHanding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDeviceAndProgressHanding)
{
    ui->setupUi(this);
}

DialogDeviceAndProgressHanding::~DialogDeviceAndProgressHanding()
{
    delete ui;
}

void DialogDeviceAndProgressHanding::on_toolButton_DeviceSetting_clicked()
{
    DialogDeviceHandle a(this);
    a.exec();
}

void DialogDeviceAndProgressHanding::on_toolButton_ProgressSetting_clicked()
{
    DialogProgressHandle a(this);
    a.exec();
}
