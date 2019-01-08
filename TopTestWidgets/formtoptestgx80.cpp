#include "formtoptestgx80.h"
#include "ui_formtoptestgx80.h"

#include "toptestusb.h"

#include <QDateTime>
#include <QDebug>

FormTopTestGX80::FormTopTestGX80(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTopTestGX80),
    usbHandle(TopTestUsb::instance())
{
    ui->setupUi(this);


    connect(ui->comboBox_StartMode,SIGNAL(currentIndexChanged(int)),
            this,SLOT(onStateStartModeChanged(int)));

    connect(ui->comboBox_StartMode_Set,SIGNAL(currentIndexChanged(int)),
            this,SLOT(onSetStartModeChanged(int)));

    ui->comboBox_StartMode->setEnabled(false);

    ui->comboBox_StartMode->addItems({"PressStart","DelayStart","TempStart"});
    ui->comboBox_StartMode_Set->addItems({"PressStart","DelayStart","TempStart"});

    {
        ui->lineEdit_DeviceType->setReadOnly(true);
        ui->lineEdit_SampleTime->setReadOnly(true);
        ui->lineEdit_InternalTemp->setReadOnly(true);

        ui->comboBox_StartMode->setEditable(false);
        ui->lineEdit_StartTemp->setReadOnly(true);
        ui->lineEdit_StartDelayTime->setReadOnly(true);

        ui->dateTimeEdit_DeviceDateTime->setReadOnly(true);

        ui->dateTimeEdit_CurrentTime->setDateTime(QDateTime::currentDateTime());

        ui->doubleSpinBox_SampleTime->setMinimum(0.05);
        ui->doubleSpinBox_SampleTime->setMaximum(999.95);

        ui->doubleSpinBox_StartDelayTime->setMinimum(1);
        ui->doubleSpinBox_StartDelayTime->setDecimals(0);
        ui->doubleSpinBox_StartDelayTime->setMaximum(200);

        ui->doubleSpinBox_StartTemp->setMinimum(25);
        ui->doubleSpinBox_StartTemp->setDecimals(0);
        ui->doubleSpinBox_StartTemp->setMaximum(100);

   }

//    refreshState();
}

FormTopTestGX80::~FormTopTestGX80()
{
    delete ui;
}

void FormTopTestGX80::refreshState()
{
    qDebug()<<"start refresh device state";

    ui->lineEdit_DeviceType->setText(usbHandle->ReadDeviceID());
    ui->lineEdit_SampleTime->setText(QString::number(usbHandle->ReadSampleTime()));
    ui->lineEdit_InternalTemp->setText(QString::number(usbHandle->ReadDeviceInternalValue()));

    ui->comboBox_StartMode->setCurrentIndex(usbHandle->ReadDeviceStartMode());
    ui->lineEdit_StartTemp->setText(QString::number(usbHandle->ReadDeviceDelayStartTemputure()));
    ui->lineEdit_StartDelayTime->setText(QString::number(usbHandle->ReadDeviceDelayStartTime()));

    ui->dateTimeEdit_DeviceDateTime->setDate(usbHandle->ReadDeviceDate());
    ui->dateTimeEdit_DeviceDateTime->setTime(usbHandle->ReadDeviceTime());

    ui->dateTimeEdit_CurrentTime->setDateTime(QDateTime::currentDateTime());


    ///refresh LCD display
    QList<double> portValueList = usbHandle->ReadPortCurrentValue();
    qDebug()<< portValueList;

    foreach (QLCDNumber* lcd, this->findChildren<QLCDNumber*>()) {
        lcd->display(portValueList.at( QString(lcd->objectName().remove("lcdNumber_")).toInt()-1) );
    }
}

void FormTopTestGX80::pushSettingToDevice()
{
    qDebug()<<"start push setting to device";
    if(ui->lineEdit_SampleTime->text().toDouble()
            != ui->doubleSpinBox_SampleTime->value())
        usbHandle->SetSampleTime(ui->doubleSpinBox_SampleTime->value());
    if(ui->comboBox_StartMode->currentIndex()
            != ui->comboBox_StartMode_Set->currentIndex())
        usbHandle->SetDeviceStartMode((TopTest::TopTestStartMode)ui->comboBox_StartMode_Set->currentIndex());
//    if(ui->lineEdit_StartDelayTime->text().toDouble()
//            != ui->doubleSpinBox_StartDelayTime->value())
//    usbHandle->SetDeviceDelayStartTime(ui->doubleSpinBox_StartDelayTime->value());
//    if(ui->lineEdit_StartTemp->text().toDouble()
//            != ui->doubleSpinBox_StartTemp->value())
//    usbHandle->SetDeviceDelayStartTemputure(ui->doubleSpinBox_StartTemp->value());
    if(ui->dateTimeEdit_DeviceDateTime->date()
            != ui->dateTimeEdit_CurrentTime->date())
    usbHandle->SetDeviceDate(ui->dateTimeEdit_CurrentTime->date());
    if(ui->dateTimeEdit_DeviceDateTime->time()
            != ui->dateTimeEdit_CurrentTime->time())
    usbHandle->SetDeviceTime(ui->dateTimeEdit_CurrentTime->time());

    refreshState();
}

void FormTopTestGX80::on_pushButton_Clear_clicked()
{
    usbHandle->ClearDeviceData();
}

void FormTopTestGX80::on_pushButton_Write_clicked()
{
    pushSettingToDevice();
}

void FormTopTestGX80::onStateStartModeChanged(int mode)
{
    ui->lineEdit_StartDelayTime->setVisible(false);
    ui->lineEdit_StartTemp->setVisible(false);

    switch(mode)
    {
    case TopTest::Direct:
        break;
    case TopTest::DelayByTemputure:
        ui->lineEdit_StartTemp->setVisible(true);
        break;
    case TopTest::DelayByTime:
        ui->lineEdit_StartDelayTime->setVisible(true);
        break;
    default:
        break;
    }
}

void FormTopTestGX80::onSetStartModeChanged(int mode)
{
    ui->doubleSpinBox_StartDelayTime->setVisible(false);
    ui->doubleSpinBox_StartTemp->setVisible(false);

    switch(mode)
    {
    case TopTest::Direct:
        break;
    case TopTest::DelayByTemputure:
        ui->doubleSpinBox_StartTemp->setVisible(true);
        break;
    case TopTest::DelayByTime:
        ui->doubleSpinBox_StartDelayTime->setVisible(true);
        break;
    default:
        break;
    }
}
