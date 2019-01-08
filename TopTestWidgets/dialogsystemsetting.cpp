#include "dialogsystemsetting.h"
#include "ui_dialogsystemsetting.h"

#include "formtoptestgx80.h"
#include "dialogrsmii.h"

#include "toptestcore.h"
#include "toptestenum.h"

#include "toptestusb.h"

DialogSystemSetting::DialogSystemSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogSystemSetting)
{
    ui->setupUi(this);
    init();
}

DialogSystemSetting::~DialogSystemSetting()
{
    delete ui;
}

void DialogSystemSetting::on_toolButton_TopTest_clicked()
{
    FormTopTestGX80 gx80(this);
    gx80.exec();
}

void DialogSystemSetting::on_toolButton_RSMII_clicked()
{
    DialogRSMII rsmii(this);
    rsmii.exec();
}

void DialogSystemSetting::init()
{
    ///init unit
    ui->comboBox_Length->addItem(QString::fromStdWString(L"英寸"),TopTest::inch);
    ui->comboBox_Length->addItem(QString::fromStdWString(L"厘米"),TopTest::cm);
    ui->comboBox_Length->addItem(QString::fromStdWString(L"毫米"),TopTest::mm);

    ui->comboBox_Speed->addItem(QString::fromStdWString(L"英寸/分"),TopTest::inchPerMin);
    ui->comboBox_Speed->addItem(QString::fromStdWString(L"英尺/分"),TopTest::feetPerMin);
    ui->comboBox_Speed->addItem(QString::fromStdWString(L"米/分"),TopTest::mPerMin);
    ui->comboBox_Speed->addItem(QString::fromStdWString(L"厘米/分"),TopTest::cmPerMin);
    ui->comboBox_Speed->addItem(QString::fromStdWString(L"毫米/分"),TopTest::mmPerMin);

    ui->comboBox_Temp->addItem(QString::fromStdWString(L"摄氏°C"),TopTest::C);
    ui->comboBox_Temp->addItem(QString::fromStdWString(L"华氏°F"),TopTest::F);

    ///init module
    ///init language
    ui->comboBox_Lan->addItem(QString::fromStdWString(L"中文(zh-CN)"),"zh-CN");
    ui->comboBox_Lan->addItem(QString::fromStdWString(L"English(en-US)"),"en-US");

    ///init port
    ui->comboBox_Port->setEditable(false);
    QStringList portList = TopTestUsb::instance()->getAviablePortList();
    if(portList.count()==1)
        TopTestUsb::instance()->setActivePortName(portList.first());
    ui->comboBox_Port->addItems(portList);
}

void DialogSystemSetting::on_toolButton_OK_clicked()
{
    ///apply current setting
    //appy unit
    UnitType len = (UnitType) ui->comboBox_Length->currentData().toInt();
    TopTestCore::instance()->setLengthUnitType(len);
    UnitType speed = (UnitType) ui->comboBox_Speed->currentData().toInt();
    TopTestCore::instance()->setSpeedUnitType(speed);
    UnitType temp = (UnitType) ui->comboBox_Temp->currentData().toInt();
    TopTestCore::instance()->setTemputureUnitType(temp);

    //apply language
    QString newLan = ui->comboBox_Lan->currentData().toString();
    if(newLan != lan)
    {
        lan = newLan;
        emit this->languageChanged(lan);
    }

    //apply port
}

void DialogSystemSetting::on_comboBox_Port_currentTextChanged(const QString &arg1)
{
    TopTestUsb::instance()->setActivePortName(arg1);
}
