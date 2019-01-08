#include "formdevicewidget.h"
#include "ui_formdevicewidget.h"

#include "formtoptestgx80.h"
#include "dialogrsmii.h"

FormDeviceWidget::FormDeviceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceWidget)
{
    ui->setupUi(this);
    deviceMap["Gx 80"] = new FormTopTestGX80(this);
    deviceMap["RSM II"] = new DialogRSMII(this);
    init();
}

FormDeviceWidget::~FormDeviceWidget()
{
    delete ui;
}

void FormDeviceWidget::insertDevice(const QString &tag, QWidget *deviceWidget)
{
    ui->listWidget_device->addItem(tag);
    deviceMap[tag]=deviceWidget;
    ui->stackedWidget_device->addWidget(deviceWidget);
}

void FormDeviceWidget::init()
{
    foreach (QString tag, deviceMap.keys()) {
        insertDevice(tag,deviceMap[tag]);
    }
//    ui->listWidget_device->addItem("Gx 80");
//    ui->listWidget_device->addItem("RSM II");
}

void FormDeviceWidget::on_listWidget_device_itemDoubleClicked(QListWidgetItem *item)
{
    ui->listWidget_device->setCurrentItem(item);
    ui->stackedWidget_device->setCurrentWidget(deviceMap[item->text()]);
}
