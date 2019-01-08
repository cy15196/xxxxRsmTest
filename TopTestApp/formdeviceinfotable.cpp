#include "formdeviceinfotable.h"
#include "ui_formdeviceinfotable.h"
#include <QSettings>
#include <QDebug>

FormDeviceInfoTable::FormDeviceInfoTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceInfoTable)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,30);
//    test();
}

FormDeviceInfoTable::~FormDeviceInfoTable()
{
    delete ui;
}

void FormDeviceInfoTable::setPortInfo(const QMap<int, QPair<QString, QColor> > &portInfo)
{

}

void FormDeviceInfoTable::applyDevice(const QString &deviceName)
{
    qDebug()<<"go to apply device"<<deviceName;

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup("Device");

    if(appSetting.allKeys().isEmpty())
    {
        return;
    }

    QString firstKeyName = appSetting.allKeys().first();
    qDebug()<<"firstDeviceName"<<firstKeyName;

    QString realProcessName = deviceName;
    qDebug()<<"realDeviceName"<<realProcessName;

    QVariant progressSetting  = appSetting.value(realProcessName);
    QMap<QString,QVariant> typeDeviceMap = progressSetting.toMap().value("tableWidget_device").toMap();
    qDebug()<<"process map is"<<progressSetting;
    bool isSingleSection = progressSetting.toMap().value("radioButton_3").toBool();

    appSetting.endGroup();
    appSetting.endGroup();

    ui->tableWidget->loadDataToTable(typeDeviceMap, isSingleSection);
}

void FormDeviceInfoTable::applyDevice(const QMap<QString, QVariant> &deviceMap)
{
    qDebug()<<"process map is"<<deviceMap;
    QMap<QString,QVariant> typeDeviceMap = deviceMap.value("tableWidget_device").toMap();
    bool isSingleSection = deviceMap.value("radioButton_3").toBool();
    ui->tableWidget->loadDataToTable(typeDeviceMap, isSingleSection);
}


void FormDeviceInfoTable::test()
{
    for(int i=0;i<8;i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(ui->tableWidget->rowCount()));
        noItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,noItem);
    }

}

QTableWidget *FormDeviceInfoTable::getTableWidget()
{
    return ui->tableWidget;
}
