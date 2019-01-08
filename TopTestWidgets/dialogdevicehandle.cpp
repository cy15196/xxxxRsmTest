#include "dialogdevicehandle.h"
#include "ui_dialogdevicehandle.h"

DialogDeviceHandle::DialogDeviceHandle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDeviceHandle)
{
    ui->setupUi(this);
}

DialogDeviceHandle::~DialogDeviceHandle()
{
    delete ui;
}
