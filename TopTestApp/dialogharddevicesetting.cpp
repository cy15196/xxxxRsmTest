#pragma execution_character_set("utf-8")

#include "dialogharddevicesetting.h"
#include "ui_dialogharddevicesetting.h"

#include <QDebug>

#include "rsmusb.h"
#include "serialporttransfer.h"

#include "formlinksetting.h"

DialogBase* DialogHardDeviceSetting::linkWidgetTemp = 0;

inline void setTableWidgetText(QTableWidget* table,int row,int column,const QString& string)
{
    QTableWidgetItem* item = new QTableWidgetItem(string);
    item->setTextAlignment(Qt::AlignCenter);
    table->setItem(row,column,item);
}

DialogHardDeviceSetting::DialogHardDeviceSetting(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogHardDeviceSetting)
{
    //    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);

    linkWidgetTemp = FormLinkSetting::dialogInstance()->getCentralDialog();
    qDebug()<<"linkWidgetTemp"<<linkWidgetTemp;
    ui->linkLayout->addWidget(linkWidgetTemp);
    FormLinkSetting::instance()->setVisible(true);

    connect(FormLinkSetting::instance(),SIGNAL(deviceConnected()),
            this,SLOT(refreshWhenPortChanged()));

    connect(FormLinkSetting::instance(),SIGNAL(deviceDisconnected()),
            this,SLOT(refreshWhenPortChanged()));

    init();

    autoRefreshTimer.setInterval(1000);
//    deviceDetectTimer.setInterval(200);
//    deviceDetectTimer.start();
    connect(&autoRefreshTimer,SIGNAL(timeout()),
            this,SLOT(refreshDataInfo()));
//    connect(&deviceDetectTimer,SIGNAL(timeout()),
//            SLOT(refreshDeviceList()));

}

DialogHardDeviceSetting::~DialogHardDeviceSetting()
{
    autoRefreshTimer.stop();

    ui->linkLayout->removeWidget(linkWidgetTemp);
    linkWidgetTemp->setParent(FormLinkSetting::dialogInstance());
    FormLinkSetting::dialogInstance()->setCentralDialog(linkWidgetTemp);
    qDebug()<<"linkWidgetTemp"<<linkWidgetTemp;
    linkWidgetTemp = 0 ;
    FormLinkSetting::instance()->setVisible(false);

    delete ui;
}

void DialogHardDeviceSetting::refreshWhenPortChanged()
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        ui->groupBox_State->setEnabled(false);
        ui->groupBox_PortValue->setEnabled(false);
        ui->groupBox_settings->setEnabled(false);
    }
    else
    {
        ui->groupBox_State->setEnabled(true);
        ui->groupBox_PortValue->setEnabled(true);
        ui->groupBox_settings->setEnabled(true);
        refreshDataInfo();
    }
}

void DialogHardDeviceSetting::refreshDataInfo()
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
        return;

    RSMUsb::instance()->waitForMSecs(50);
    RSMUsb::instance()->refreshUserData();
    RSMUsb::instance()->waitForMSecs(50);
    RSMUsb::instance()->refreshConfigureData();
}

void DialogHardDeviceSetting::onApply()
{

    RSMUsb* rsmUsb = RSMUsb::instance();

    if(rsmUsb->getActivePortName().isEmpty())
        return;

    disconnect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataChanged(int,QVariant)));

    if(ui->label_startMode->font().bold())
    {
        int mode = -1;
        if(ui->radioButton_buttonStart->isChecked())
            mode = 0;
        else if(ui->radioButton_delayStart->isChecked())
            mode = 1;
        else if(ui->radioButton_tempStart->isChecked())
            mode = 2;

        if(mode != -1 )
            rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_Start_Mode,mode);
    }

    if(ui->label_stopMode->font().bold())
    {
        int mode = -1;
        if(ui->radioButton_buttonStop->isChecked())
            mode = 0;
        else if(ui->radioButton_tempStop->isChecked())
            mode = 1;

        if(mode != -1 )
            rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_StopMode,mode);
    }

    if(ui->label_interalDateTime->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_Device_DateTime,
                                     ui->dateTimeEdit->dateTime());
    }

    if(ui->label_sampleTime->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_Sample_Time_Interal,
                                     ui->doubleSpinBox_sampleTime->value());
    }

    if(ui->label_startDelayTime->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_PressStart_DelayTime,
                                     ui->doubleSpinBox_buttonPressDelayTime->value());
    }

    if(ui->label_startTemp->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_FirstTempStart_Start_Temp,
                                     ui->doubleSpinBox_stratTemp->value());
    }

    if(ui->label_stopTemp->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_FirstTempStart_Stop_Temp,
                                     ui->doubleSpinBox_stopTemp->value());
    }

    if(ui->label_delayStop->font().bold())
    {
        rsmUsb->writeDeviceValueItem(TopTest::RsmCMD_FirstTempStart_Stop_DelayTime,
                                     ui->doubleSpinBox_tempStopDelayTime->value());
    }

    connect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataChanged(int,QVariant)));



    refreshDataInfo();

    //
    foreach (QLabel* label, editToLabelMap.values()) {
        label->setFont(QFont());
    }
}

void DialogHardDeviceSetting::showEvent(QShowEvent *event)
{
    this->setFocus();
}

void DialogHardDeviceSetting::hideEvent(QHideEvent *event)
{

}

void DialogHardDeviceSetting::onButtonClicked(int role)
{
    switch(role)
    {
    case QDialogButtonBox::Ok:
    case QDialogButtonBox::Apply:
        this->onApply();
        break;
    default:
        break;
    }
}

int DialogHardDeviceSetting::standerButtons()
{
    return DialogBase::standerButtons() | QDialogButtonBox::Apply;
}

void DialogHardDeviceSetting::init()
{

//    ui->doubleSpinBox_sampleTime->setMaximum(0.05);
    ui->doubleSpinBox_sampleTime->setMaximum(200);
//    ui->doubleSpinBox_sampleTime->setDecimals(0);

    ui->doubleSpinBox_stratTemp->setMaximum(25);
    ui->doubleSpinBox_stratTemp->setMaximum(100);
    ui->doubleSpinBox_stratTemp->setDecimals(0);

    ui->doubleSpinBox_tempStopDelayTime->setMaximum(0);
    ui->doubleSpinBox_tempStopDelayTime->setMaximum(3600);
    ui->doubleSpinBox_tempStopDelayTime->setDecimals(0);

    ui->doubleSpinBox_buttonPressDelayTime->setMaximum(0);
    ui->doubleSpinBox_buttonPressDelayTime->setMaximum(3600);
    ui->doubleSpinBox_buttonPressDelayTime->setDecimals(0);

    ui->doubleSpinBox_stopTemp->setMaximum(25);
    ui->doubleSpinBox_stopTemp->setMaximum(100);
    ui->doubleSpinBox_stopTemp->setDecimals(0);

    ui->tableWidget_Info->setColumnWidth(0,72);
    ui->tableWidget_Info->setColumnWidth(1,128);
//    ui->tableWidget_Info->setColumnWidth(2,72);
    ui->tableWidget_Info->setColumnWidth(3,128);

    enumToRadioButtonMap[TopTest::Direct] = ui->radioButton_buttonStart;
    enumToRadioButtonMap[TopTest::DelayByTime] = ui->radioButton_delayStart;
    enumToRadioButtonMap[TopTest::DelayByTemputure] = ui->radioButton_tempStart;

    enumToRadioButtonMap[0x10+TopTest::DirectStop] = ui->radioButton_buttonStop;
    enumToRadioButtonMap[0x10+TopTest::DelayByTimeAndTemputure] = ui->radioButton_tempStop;

//    enumToRadioButtonMap[0x20+TopTest::DirectStop] = ui->radioButton_buttonStop;
//    enumToRadioButtonMap[0x20+TopTest::DelayByTimeAndTemputure] = ui->radioButton_tempStop;

    editToLabelMap[ui->dateTimeEdit] = ui->label_interalDateTime;
    editToLabelMap[ui->doubleSpinBox_buttonPressDelayTime] = ui->label_startDelayTime;
    editToLabelMap[ui->doubleSpinBox_sampleTime] = ui->label_sampleTime;
    editToLabelMap[ui->doubleSpinBox_stratTemp] = ui->label_startTemp;
    editToLabelMap[ui->doubleSpinBox_stopTemp] = ui->label_stopTemp;
    editToLabelMap[ui->doubleSpinBox_tempStopDelayTime] = ui->label_delayStop;

    editToLabelMap[ui->radioButton_buttonStart] = ui->label_startMode;
    editToLabelMap[ui->radioButton_tempStart] = ui->label_startMode;
    editToLabelMap[ui->radioButton_delayStart] = ui->label_startMode;

    editToLabelMap[ui->radioButton_buttonStop] = ui->label_stopMode;
    editToLabelMap[ui->radioButton_tempStop] = ui->label_stopMode;
//    editToLabelMap[ui->radioButton_delayStop] = ui->label_stopMode;

    ui->radioButton_buttonStart->setChecked(true);
    ui->widget_tempStart->setVisible(false);
    ui->widget_delayStart->setVisible(false);

    ui->radioButton_buttonStop->setChecked(true);
    ui->widget_tempStop->setVisible(false);
    ui->widget_delayStop->setVisible(false);

    connect(ui->radioButton_buttonStart,SIGNAL(toggled(bool)),
            SLOT(onRadioButtonChecked(bool)));
    connect(ui->radioButton_tempStart,SIGNAL(toggled(bool)),
            SLOT(onRadioButtonChecked(bool)));
    connect(ui->radioButton_delayStart,SIGNAL(toggled(bool)),
            SLOT(onRadioButtonChecked(bool)));
    connect(ui->radioButton_buttonStop,SIGNAL(toggled(bool)),
            SLOT(onRadioButtonChecked(bool)));
    connect(ui->radioButton_tempStop,SIGNAL(toggled(bool)),
            SLOT(onRadioButtonChecked(bool)));
//    connect(ui->radioButton_delayStop,SIGNAL(toggled(bool)),
//            SLOT(onRadioButtonChecked(bool)));

    connect(ui->dateTimeEdit,SIGNAL(dateTimeChanged(QDateTime)),
            this,SLOT(onEditValueChanged()));
    foreach (QDoubleSpinBox* dsBox, this->findChildren<QDoubleSpinBox*>()) {
        connect(dsBox,SIGNAL(valueChanged(double)),
                SLOT(onEditValueChanged()));
    }

    foreach (QRadioButton* rBox, this->findChildren<QRadioButton*>()) {
        connect(rBox,SIGNAL(clicked(bool)),
                SLOT(onEditValueChanged()));
    }

    connect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataChanged(int,QVariant)));

    refreshWhenPortChanged();
}


void DialogHardDeviceSetting::on_pushButton_clicked()
{
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void DialogHardDeviceSetting::onEditValueChanged()
{
    qDebug()<<"DialogHardDeviceSetting::onEditValueChanged()"<<sender();
    QLabel* label = editToLabelMap[sender()];
    if(label)
    {
        QFont font;
        font.setBold(true);
        label->setFont(font);
    }
}

void DialogHardDeviceSetting::onRadioButtonChecked(bool checked)
{
    if(!checked)
        return;
    QRadioButton* button = dynamic_cast<QRadioButton*> (sender());
    if(button == ui->radioButton_buttonStart)
    {
        ui->widget_tempStart->setVisible(false);
        ui->widget_delayStart->setVisible(false);
    }
    else if(button == ui->radioButton_tempStart)
    {
        ui->widget_tempStart->setVisible(true);
        ui->widget_delayStart->setVisible(false);
    }
    else if(button == ui->radioButton_delayStart)
    {
        ui->widget_tempStart->setVisible(false);
        ui->widget_delayStart->setVisible(true);
    }


    else if(button == ui->radioButton_buttonStop)
    {
        ui->widget_tempStop->setVisible(false);
        ui->widget_delayStop->setVisible(false);
    }

    else if(button == ui->radioButton_tempStop)
    {
        ui->widget_tempStop->setVisible(true);
        ui->widget_delayStop->setVisible(true);
    }
}

//void DialogHardDeviceSetting::on_comboBox_linkType_currentIndexChanged(const QString &arg1)
//{
//    if(RSMUsb::instance()->getPortHandle())
//    {
//        RSMUsb::instance()->getPortHandle()->setBaudRate(ui->comboBox_linkType->currentData().toInt());
//        if(!deviceDetectTimer.isActive())
//            deviceDetectTimer.start();
//    }
//}

//void DialogHardDeviceSetting::refreshDeviceList()
//{
//    QStringList portList = RSMUsb::instance()->getAviablePortList();
//    qDebug() << portList;


//    deviceDetectTimer.stop();

//    ui->comboBox_deviceChoose->blockSignals(true);
//    ui->comboBox_deviceChoose->clear();
//    ui->comboBox_deviceChoose->addItems(portList);

//    if(portList.isEmpty())
//        ui->comboBox_deviceChoose->setCurrentText("");
//    else if(!portList.contains(RSMUsb::instance()->getActivePortName()))
//        ui->comboBox_deviceChoose->setCurrentText(portList.first());
//    else
//        ui->comboBox_deviceChoose->setCurrentText(RSMUsb::instance()->getActivePortName());

//    this->on_comboBox_deviceChoose_currentTextChanged(ui->comboBox_deviceChoose->currentText());
//    ui->comboBox_deviceChoose->blockSignals(false);

//    if(RSMUsb::instance()->getActivePortName().isEmpty())
//    {
//        deviceDetectTimer.start();
//    }
////    else
////    {
//////        ui->comboBox_BaudRate->setCurrentText(
//////                    QString::number(RSMUsb::instance()->getPortHandle()->getBaudRate()));

////    }
//}

//void DialogHardDeviceSetting::on_comboBox_deviceChoose_currentTextChanged(const QString &arg1)
//{
//    RSMUsb::instance()->setActivePortName(arg1);
//    refreshWhenPortChanged();
//}

void DialogHardDeviceSetting::on_checkBox_autoRefresh_clicked(bool checked)
{
    ui->pushButton->setEnabled(!checked);
    if(checked)
    {
        autoRefreshTimer.start();
    }
    else
    {
        autoRefreshTimer.stop();
    }
}

void DialogHardDeviceSetting::on_pushButton_2_clicked()
{
    refreshDataInfo();
}

void DialogHardDeviceSetting::onDeviceDataChanged(int dataAddress, const QVariant &data)
{
    this->blockSignals(true);

    switch (dataAddress) {

    ///state
    case TopTest::RsmCMD_Device_DateTime:
    {
        QDateTime deviceDataTime = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_Device_DateTime).toDateTime();
        setTableWidgetText(
                    ui->tableWidget_Info,0,1,
                    deviceDataTime.toString("yyyy/MM/dd hh:mm:ss"));
        ui->dateTimeEdit->setDateTime(deviceDataTime);
        break;
    }

    case TopTest::RsmCMD_Sample_Time_Interal:
    {
        double SampleInteralTime = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_Sample_Time_Interal).toDouble();
        setTableWidgetText(ui->tableWidget_Info,1,1,QString(" %1 s").arg(
                               QString::number(SampleInteralTime,'f',2)));
        ui->doubleSpinBox_sampleTime->setValue(SampleInteralTime);
        break;
    }
    ///TODO:启动方式显示成文字
    case TopTest::RsmCMD_Start_Mode:
    {
        int startMode = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_Start_Mode).toInt();
        setTableWidgetText(ui->tableWidget_Info,2,1,
                           TopTest::getTopTestStartModeTranslate(startMode));
        QRadioButton* radio = enumToRadioButtonMap[startMode];
        if(radio)
        {
            radio->setChecked(true);
        }
        break;
    }

    case TopTest::RsmCMD_BatteryVolage:
    {

        double bv = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_BatteryVolage).toDouble();
        setTableWidgetText(ui->tableWidget_Info,5,1,
                           QString(" %1 V").arg(bv));
        break;
    }

    case TopTest::RsmCMD_PressStart_DelayTime:
    {
        double buttonDelayTime = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_PressStart_DelayTime).toDouble();
        setTableWidgetText(ui->tableWidget_Info,3,3,
                           QString(" %1 s").arg(buttonDelayTime));
        ui->doubleSpinBox_buttonPressDelayTime->setValue(buttonDelayTime);
        break;
    }

    case TopTest::RsmCMD_FirstTempStart_Start_Temp:
    {
        double startTemp = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_FirstTempStart_Start_Temp).toDouble();
        setTableWidgetText(ui->tableWidget_Info,3,1,
                           QString(" %1 %2")
                           .arg(startTemp)
                           .arg(TopTestCore::instance()->getCurrentTemputureUnitStr()));
        ui->doubleSpinBox_stratTemp->setValue(startTemp);
        break;
    }

    case TopTest::RsmCMD_FirstTempStart_Stop_Temp:
    {
        double stopTemp = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_FirstTempStart_Stop_Temp).toDouble();
        setTableWidgetText(ui->tableWidget_Info,4,1,
                           QString(" %1 %2")
                           .arg(stopTemp)
                           .arg(TopTestCore::instance()->getCurrentTemputureUnitStr()));
        ui->doubleSpinBox_stopTemp->setValue(stopTemp);
        break;
    }

    case TopTest::RsmCMD_FirstTempStart_Stop_DelayTime:
    {
        double tempStopDelayTime = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_FirstTempStart_Stop_DelayTime).toDouble();
        setTableWidgetText(ui->tableWidget_Info,4,3,
                           QString(" %1 s").arg(tempStopDelayTime));

        ui->doubleSpinBox_tempStopDelayTime->setValue(tempStopDelayTime);
        break;
    }

    case TopTest::RsmCMD_StopMode:
    {
        int stopMode = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_StopMode).toInt();

        qDebug()<<"update stopmode"<<stopMode;

        QRadioButton* radio = enumToRadioButtonMap[0x10 + stopMode];
        if(radio)
        {
            radio->setChecked(true);
        }
        break;
    }

    case TopTest::RsmConfig_User_Data_DeviceModel:
    {
        ///TODO:仪器型号，出厂编号，出厂日期等数据未设置时显示为未设置
        //deviceModel
        QString devceModelStr = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmConfig_User_Data_DeviceModel).toString();
        if(devceModelStr.isNull())
            devceModelStr = tr("未設置");
        else
            devceModelStr = QString("RSM-%1").arg(devceModelStr);
        setTableWidgetText(ui->tableWidget_Info,0,3,devceModelStr);
        break;
    }

    case TopTest::RsmConfig_User_Data_ProduceDateTimeBegin:
    {

        QDateTime produceDateTime = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmConfig_User_Data_ProduceDateTimeBegin).toDateTime();
        QString textProductDateTime;
        if(!produceDateTime.isValid())
            textProductDateTime = tr("未設置");
        else
            textProductDateTime = produceDateTime.toString("yyyy/MM/dd hh:mm:ss");

        setTableWidgetText(ui->tableWidget_Info,1,3,textProductDateTime);
        break;
    }

    case TopTest::RsmConfig_User_Data_ProductNo:
    {

        int32_t productId = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmConfig_User_Data_ProductNo).toInt();

        QString textProductDateTime;
        if(productId == 0)
            textProductDateTime = tr("未設置");
        else
            textProductDateTime = QString("%1").arg(productId);

        setTableWidgetText(ui->tableWidget_Info,2,3,
                           textProductDateTime);
        break;
    }


    case TopTest::RsmCMD_InnerTemp:
    {
        double internalTemp = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_InnerTemp).toDouble();
        setTableWidgetText(ui->tableWidget_Info,5,3,
                           QString("%1 %2")
                           .arg(QString::number(internalTemp,'f',1))
                           .arg(TopTestCore::instance()->getCurrentTemputureUnitStr())
                           );
        break;
    }

    case TopTest::RsmCMD_Port_Number:
    {
        ///port value
        uchar portNum = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_Port_Number).toInt();
        qDebug()<<"Port no is"<<portNum;
        foreach (QLCDNumber* lcdNumber, this->findChildren<QLCDNumber*>()) {
            if(QString(lcdNumber->objectName().split("_").last()).toInt()>portNum)
                lcdNumber->setVisible(false);
            else
                lcdNumber->setVisible(true);
        }

        QList<double> tempValueList = RSMUsb::instance()->getPortTempValues();
        qDebug()<<"origin temp value list"<<tempValueList;

        for(int i = 0; (i< tempValueList.count()) && (i < portNum) ; i++ )
        {
            QLCDNumber* lcdNumber = this->findChild<QLCDNumber*>(QString("lcdNumber_%1").arg(i+1));
            if(lcdNumber)
            {
    //            qDebug()<<"go to display value of port"<<i+1;
                lcdNumber->display(tempValueList[i]);
            }
        }
        break;
    }

    case TopTest::RsmCMD_FirstTempData:
    {
        uchar portNum = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_Port_Number).toInt();
        QList<double> tempValueList = RSMUsb::instance()->getPortTempValues();
        qDebug()<<"origin temp value list"<<tempValueList;

        for(int i = 0; (i< tempValueList.count()) && (i < portNum) ; i++ )
        {
            QLCDNumber* lcdNumber = this->findChild<QLCDNumber*>(QString("lcdNumber_%1").arg(i+1));
            if(lcdNumber)
            {
    //            qDebug()<<"go to display value of port"<<i+1;
                lcdNumber->display(tempValueList[i]);
            }
        }
        break;
    }

    default:
        break;
    }

    foreach (QLabel* label, editToLabelMap.values()) {
        label->setFont(QFont());
    }

    this->blockSignals(false);
}

void DialogHardDeviceSetting::onUnitTypeChanged(int unitType)
{

}
