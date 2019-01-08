#include "dialogcurrentlinkdevices.h"
#include "ui_dialogcurrentlinkdevices.h"

#include "rsmusb.h"
#include "clabso_ft_dpi.h"

DialogCurrentLinkDevices::DialogCurrentLinkDevices(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogCurrentLinkDevices)
{
    ui->setupUi(this);
    init();
    this->setMinimumSize(ClabsoFTDpi::convertSize( QSize(600,400) ));

}

DialogCurrentLinkDevices::~DialogCurrentLinkDevices()
{
    delete ui;
}

void DialogCurrentLinkDevices::onButtonClicked(int role)
{

}

int DialogCurrentLinkDevices::standerButtons()
{
    return QDialogButtonBox::Ok;
}

void DialogCurrentLinkDevices::refreshDeviceList()
{
    ui->tableWidget->setRowCount(0);

    QStringList list = RSMUsb::instance()->getAviablePortList();

    list<<"Com1"<<"Com4"<<"Com5";

    ui->tableWidget->setRowCount(list.count());
    for(int i =0 ; i< ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("DeviceName"));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(list.at(i)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem("DeviceModel"));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem("Aviable"));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QTime::currentTime().toString()));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(""));
    }
}

void DialogCurrentLinkDevices::init()
{
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(4,60);

    refreshDeviceList();
}
