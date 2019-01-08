#pragma execution_character_set("utf-8")

#include "rsmconfigmainwindow.h"
#include "ui_rsmconfigmainwindow.h"

#include <QDebug>
#include <QPushButton>
#include <QObjectUserData>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDateTimeEdit>

#include <Windows.h>
#include <dbt.h>

#include "rsmusb.h"
#include "serialporttransfer.h"
#include <QCursor>

#include <QSettings>

#include "clabso_ft_dpi.h"

inline void setAppSettingValues(const QStringList& groupList, const QMap<QString,QVariant>& keyValueMap)
{
    qDebug()<<"setting values"<<groupList<<keyValueMap;
    QSettings appSettings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    foreach (QString groupName, groupList) {
        appSettings.beginGroup(groupName);
    }

    foreach (QString key, keyValueMap.keys()) {
        appSettings.setValue(key,keyValueMap[key]);
    }

    foreach (QString groupName, groupList) {
        appSettings.endGroup();
    }
}

inline QMap<QString,QVariant> readAppSettingValues(const QStringList& groupList)
{
    QSettings appSettings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    foreach (QString groupName, groupList) {
        appSettings.beginGroup(groupName);
    }

    QMap<QString,QVariant> ret;

    foreach (QString key, appSettings.allKeys()) {
        ret.insert(key,appSettings.value(key));
    }

    foreach (QString groupName, groupList) {
        appSettings.endGroup();
    }

    return ret;
}

RSMConfigMainWindow::RSMConfigMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RSMConfigMainWindow)
{
    this->setWindowTitle("RSM Config 0.0.1");
    this->setWindowIcon(QIcon(":/global.png"));
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    ui->setupUi(this);
    init();
    autoRefreshTimer.setInterval(1000);
    deviceDetectTimer.setInterval(1000);

    connect(&autoRefreshTimer,SIGNAL(timeout()),
            this,SLOT(refreshPortValues()));
    connect(&deviceDetectTimer,SIGNAL(timeout()),
            SLOT(refreshDeviceList()));

    connect(RSMUsb::instance(),SIGNAL(currentDeviceNoRespond(bool)),
            this,SLOT(onCurrentDeviceNoRespond(bool)));

    connect(RSMUsb::instance(),SIGNAL(currentDeviceError()),
            this,SLOT(onCurrentDeviceError()));

    deviceDetectTimer.start();
}

RSMConfigMainWindow::~RSMConfigMainWindow()
{
    autoRefreshTimer.stop();
    deviceDetectTimer.stop();
    RSMUsb::instance()->deleteLater();
    delete ui;
}

void RSMConfigMainWindow::refreshPortValues()
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
        return;

    qDebug()<<"begin to refresh port values";
    this->setCursor(QCursor(Qt::WaitCursor));
    RSMUsb::instance()->refreshUserData();
//    RSMUsb::instance()->waitForMSecs(300);
    RSMUsb::instance()->refreshConfigureData();
//    this->setEnabled(false);
    this->unsetCursor();
//    this->setEnabled(true);
}

void RSMConfigMainWindow::onAviablePortChanged(const QStringList &portList)
{
    qDebug() << portList;


    deviceDetectTimer.stop();

    ui->comboBox_deviceChoose->blockSignals(true);
    ui->comboBox_deviceChoose->clear();
    ui->comboBox_deviceChoose->addItems(portList);

    if(portList.isEmpty())
        ui->comboBox_deviceChoose->setCurrentText("");
    else if(!portList.contains(RSMUsb::instance()->getActivePortName()))
        ui->comboBox_deviceChoose->setCurrentText(portList.first());
    else
        ui->comboBox_deviceChoose->setCurrentText(RSMUsb::instance()->getActivePortName());

    this->on_comboBox_deviceChoose_currentTextChanged(ui->comboBox_deviceChoose->currentText());
    ui->comboBox_deviceChoose->blockSignals(false);

    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        qDebug()<<"active port is empty,retry";

//        RSMUsb::instance()->refreshAviablePortList();

        deviceDetectTimer.start();
    }
    else
    {
        disconnect(RSMUsb::instance(),SIGNAL(aviablePortChanged(QStringList)),
                this,SLOT(onAviablePortChanged(QStringList)));
    }
}

void RSMConfigMainWindow::onCurrentDeviceError()
{
     this->reset();
}

void RSMConfigMainWindow::refreshDeviceList()
{
    qDebug()<<"go to refresh device list";

    RSMUsb::instance()->refreshAviablePortList();
}

void RSMConfigMainWindow::refreshSettingValues(const QList<int> &rows)
{

    if(RSMUsb::instance()->getActivePortName().isEmpty())
        return;

    RSMUsb::instance()->refreshConfigureData();
    RSMUsb::instance()->waitForMSecs(300);
    RSMUsb::instance()->refreshUserData();
//    RSMUsb::instance()->waitForMSecs(300);


}

void RSMConfigMainWindow::refreshAdjustValue(const QList<int> &rows)
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
        return;
    RSMUsb::instance()->refreshAdjustData();
}

void RSMConfigMainWindow::init()
{
    this->setMinimumSize(ClabsoFTDpi::convertSize(QSize(1280,720)));

    ui->spinBox_rtu->setMinimum(1);
    ui->spinBox_rtu->setMaximum(255);
    ui->spinBox_rtu->setValue(255);

    ui->tableWidget_settings->setMinimumHeight(ClabsoFTDpi::convertSize(300));
    ui->tableWidget_settings->setMaximumHeight(ClabsoFTDpi::convertSize(300));

    ui->tableWidget_adjust->setMinimumHeight(ClabsoFTDpi::convertSize(90));
    ui->tableWidget_adjust->setMaximumHeight(ClabsoFTDpi::convertSize(90));

    ui->tableWidget_portValue->setMinimumHeight(ClabsoFTDpi::convertSize(122));
    ui->tableWidget_portValue->setMaximumHeight(ClabsoFTDpi::convertSize(122));


    ui->widget_titleBar->layout()->setMargin(0);
    ui->widget_titleBar->setFixedHeight(80);
    ui->widget_titleBar->setVisible(false);

    for(int i=0;i<16;i++)
    {
        ui->tableWidget_portValue->setItem(0,i,new QTableWidgetItem(QString::number(i+1)));
    }

    for(int i=0;i<ui->tableWidget_settings->rowCount();i++)
    {
        QPushButton * button = new QPushButton(tr("写入"), this);
        ui->tableWidget_settings->setCellWidget(i,2,button);
//        button->setUserData(Qt::UserRole,new QObjectUserData());
        connect(button,SIGNAL(clicked()),
                this,SLOT(onSettingPushButtonClicked()));
    }

    connect(ui->toolButton_writeLock,SIGNAL(toggled(bool)),
            ui->tableWidget_settings,SLOT(setEnabled(bool)));

    ui->tableWidget_settings->setEnabled(ui->toolButton_writeLock->isChecked());

    ui->tableWidget_settings->setColumnWidth(0,150);
    ui->tableWidget_settings->setColumnWidth(1,150);
    ui->tableWidget_settings->setRowHeight(1,0);
    ui->tableWidget_settings->setRowHeight(ui->tableWidget_settings->rowCount() - 1,0);

    int rowNo = 0;
    QSpinBox* rtuSpinBox = new QSpinBox(this);
    rtuSpinBox->setMaximum(254);
    rtuSpinBox->setMinimum(1);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,rtuSpinBox);

    QLineEdit* idEdit = new QLineEdit(this);
    QRegExp regExp("[a-fA-F0-9]{4}");
    idEdit->setValidator(new QRegExpValidator(regExp, this));
    idEdit->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,idEdit);


    QComboBox* batteryType = new QComboBox(this);
    batteryType->addItem(tr("锂电"),0);
    batteryType->addItem(tr("镍氢电池"),1);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,batteryType);

    QSpinBox* portSpinBox = new QSpinBox(this);
    portSpinBox->setMaximum(16);
    portSpinBox->setMinimum(1);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,portSpinBox);

    QDoubleSpinBox* maxTempspinBox = new QDoubleSpinBox(this);
    maxTempspinBox->setDecimals(1);
    maxTempspinBox->setMaximum(2000.0);
    maxTempspinBox->setMinimum(100.0);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,maxTempspinBox);

    QDoubleSpinBox* adjustSpinBox = new QDoubleSpinBox(this);
    adjustSpinBox->setDecimals(1);
    adjustSpinBox->setMaximum(30.0);
    adjustSpinBox->setMinimum(-30.0);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,adjustSpinBox);

    QComboBox* sensorType = new QComboBox(this);
    sensorType->addItem(tr("K"),0);
    sensorType->addItem(tr("J"),1);
    sensorType->addItem(tr("R"),2);
    sensorType->addItem(tr("S"),3);
    sensorType->addItem(tr("B"),4);
    sensorType->addItem(tr("E"),5);
    sensorType->addItem(tr("T"),6);
    sensorType->addItem(tr("N"),7);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,sensorType);

    QLineEdit* deviceModelEdit = new QLineEdit(this);
    QRegExp regDeviceModel("[A-Za-z0-9\\-]{0,10}");
    deviceModelEdit->setValidator(new QRegExpValidator(regDeviceModel, this));
    deviceModelEdit->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,deviceModelEdit);

    QDateEdit* produceDateTime = new QDateEdit(this);
    QDate minDateTime,maxDateTime;
    minDateTime.setDate(2000,01,01);
    maxDateTime.setDate(2099,12,31);
    produceDateTime->setDate(QDate::currentDate());
    produceDateTime->setDateRange(minDateTime,maxDateTime);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,produceDateTime);


    QLineEdit* productNoEdit = new QLineEdit(this);
    QRegExp regExpProduct("[A-Za-z0-9]{0,10}");
    productNoEdit->setValidator(new QRegExpValidator(regExpProduct, this));
    productNoEdit->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->tableWidget_settings->setCellWidget(rowNo++,1,productNoEdit);

    connect(ui->comboBox_deviceChoose,SIGNAL(currentIndexChanged(QString)),
            RSMUsb::instance(),SLOT(setActivePortName(QString)));
//    QStringList list = RSMUsb::instance()->getAviablePortList();
//    if(!list.isEmpty())
//        RSMUsb::instance()->setActivePortName(list.first());
//    ui->comboBox_deviceChoose->addItems(list);


    QPushButton* adjustZero = new QPushButton(tr("校准零点"),this);
    ui->tableWidget_adjust->setCellWidget(0,16,adjustZero);
//    ui->tableWidget_adjust->setSpan(0,16,2,1);
    connect(adjustZero,SIGNAL(clicked(bool)),
            SLOT(onAdjustPushBottonClicked()));

    QPushButton* adjustLow = new QPushButton(tr("校准"),this);
    ui->tableWidget_adjust->setCellWidget(1,16,adjustLow);
    connect(adjustLow,SIGNAL(clicked(bool)),
            SLOT(onAdjustPushBottonClicked()));

    connect(ui->toolButton_writeLock,SIGNAL(toggled(bool)),
            ui->tableWidget_adjust,SLOT(setEnabled(bool)));

    ui->tableWidget_adjust->setEnabled(ui->toolButton_writeLock->isChecked());

    connect(ui->toolButton_writeLock,SIGNAL(toggled(bool)),
            this,SLOT(onWriteLockToggled(bool)));

    ui->toolButtona_activeState->setEnabled(ui->toolButton_writeLock->isChecked());

    ui->comboBox_linkType->addItem(tr("Usb"),500000);
    ui->comboBox_linkType->addItem(tr("Wireless"),115200);
//    ui->comboBox_BaudRate->setEditable(true);
//    foreach (int baudRate, SerialPortTransfer::getAviableBaudRateList()) {
//        ui->comboBox_BaudRate->addItem(QString::number(baudRate),baudRate);
//    }

//    QMap<QString,QVariant> defaultSettings = readAppSettingValues({"DefaultSettings"});
//    QString defaultRate = defaultSettings.value("BaudRate","").toString();
//    qDebug()<<"default Baud Rate is "<<defaultRate;
//    if(!defaultRate.isEmpty())
//    {
//        ui->comboBox_BaudRate->setCurrentText(defaultRate);
//    }

    connect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataChanged(int,QVariant)));

    if(!deviceDetectTimer.isActive())
        deviceDetectTimer.start();
}

void RSMConfigMainWindow::refreshWhenPortChanged()
{

    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        ui->groupBox_adjust->setEnabled(false);
        ui->groupBox_portValue->setEnabled(false);
        ui->groupBox_setting->setEnabled(false);
        ui->toolButton_writeLock->setEnabled(false);
        ui->toolButtona_activeState->setEnabled(false);
        ui->toolButton_wirelessSwitch->setEnabled(false);
    }
    else
    {
        deviceDetectTimer.stop();

        if(!ui->comboBox_linkType->currentIndex())
        {
            ui->groupBox_adjust->setEnabled(true);
            ui->groupBox_setting->setEnabled(true);
            ui->toolButton_writeLock->setEnabled(true);
            ui->toolButtona_activeState->setEnabled(true);
            ui->toolButton_wirelessSwitch->setEnabled(true);
        }

        ui->groupBox_portValue->setEnabled(true);
//        QString baudRate =
//                QString::number(RSMUsb::instance()->getPortHandle()->getBaudRate());

//        ui->comboBox_BaudRate->setCurrentText(baudRate);

//        QMap<QString,QVariant> map;
//        map.insert("BaudRate",baudRate);
//        setAppSettingValues({"DefaultSettings"},map);
        refreshSettingValues({0,1,2,3,4,5,6,7,8,9});
    }
}

void RSMConfigMainWindow::reset()
{
    //clear port value
    for(int j = 0; j<ui->tableWidget_portValue->columnCount(); j++)
    {
        for(int i=1; i<ui->tableWidget_portValue->rowCount(); i++)
        {
            ui->tableWidget_portValue->setItem(i,j,new QTableWidgetItem(""));
        }
    }

    //clear setting value
    for(int j = 0; j<ui->tableWidget_adjust->columnCount() - 1; j++)
    {
        for(int i = 0; i<ui->tableWidget_adjust->rowCount(); i++)
        {
            ui->tableWidget_adjust->setItem(i,j,new QTableWidgetItem(""));
        }
    }


    //clear adjust value
    for(int j = 0; j < 1; j++)
    {
        for(int i = 0; i< ui->tableWidget_settings->rowCount(); i++)
        {
            ui->tableWidget_settings->setItem(i,j,new QTableWidgetItem(""));
        }
    }

    RSMUsb::instance()->clearDeviceData();

    resetLink();

}



void RSMConfigMainWindow::on_toolButton_writeLock_clicked(bool checked)
{

    qDebug()<<"RSMConfigMainWindow::on_toolButton_writeLock_clicked";

    this->setCursor(QCursor(Qt::WaitCursor));
    this->setEnabled(false);

    RSMUsb::instance()->writeDeviceValueItem(
                TopTest::RsmConfig_ConfigPassword,checked ? 0x1234 : 0);

//    RSMUsb::instance()->refreshConfigureData();

    this->unsetCursor();
    this->setEnabled(true);
}

void RSMConfigMainWindow::on_toolButtona_activeState_clicked(bool checked)
{
    this->setCursor(QCursor(Qt::WaitCursor));
    this->setEnabled(false);

    bool ok;
    int deviceId = ui->label_id->text().right(4).toInt(&ok,16);
    qDebug()<<"device Id is"<<deviceId<<"checkId"<<checked;

    ///激活过程耗时较长，需要等待超过半秒
    int mSecs = 600;
    if(checked)
        RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_ActiveDevice,
                                                 ~deviceId,
                                                 mSecs);
    else
        RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_ActiveDevice,
                                                 deviceId,
                                                 mSecs);


//    RSMUsb::instance()->waitForMSecs(600);
//    qDebug()<<"after write active"<<deviceId;
//    RSMUsb::instance()->refreshConfigureData();
//    bool active = RSMUsb::instance()->getDeviceMapValue(TopTest::RsmConfig_ActiveDevice).toBool();
//    ui->toolButtona_activeState->setChecked(active);
//    QString activeStateText;
//    if(active)
//        activeStateText = tr("Activated");
//    else
//        activeStateText = tr("Unactivated");
//    ui->label_activeState->setText(activeStateText);

    this->unsetCursor();
    this->setEnabled(true);
}

void RSMConfigMainWindow::onSettingPushButtonClicked()
{
    int rowIndex = -1;
    for(int i=0;i<ui->tableWidget_settings->rowCount();i++)
    {
        QWidget*cellWidget = ui->tableWidget_settings->cellWidget(i,2);
        if(cellWidget == sender())
        {
            rowIndex = i;
            break;
        }
    }

    qDebug()<<"push button index"<<rowIndex;

    if(rowIndex == -1)
        return;



    this->setCursor(QCursor(Qt::WaitCursor));
    this->setEnabled(false);

    switch (rowIndex) {
    case 0:
    {
        QSpinBox* edit = qobject_cast<QSpinBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmCMD_RTU_Address,edit->value());
    }
        break;
    case 2:
    {
        QComboBox* edit = qobject_cast<QComboBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_BatteryType,edit->currentData().toInt());
    }
        break;
    case 3:
    {
        QSpinBox* edit = qobject_cast<QSpinBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_PortNumber,edit->value());
    }
        break;
    case 4:
    {
        QDoubleSpinBox* edit = qobject_cast<QDoubleSpinBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_MaxValid_Temp,edit->value());
    }
        break;
    case 5:
    {
        QDoubleSpinBox* edit = qobject_cast<QDoubleSpinBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_CoolEnd_Adjust_TempValue,edit->value());
    }
        break;
    case 6:
    {
        QComboBox* edit = qobject_cast<QComboBox*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmCMD_PortModel,edit->currentData().toInt());
    }
        break;
    case 7:
    {
        QLineEdit* edit = qobject_cast<QLineEdit*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_User_Data_DeviceModel,edit->text());
    }
        break;
    case 8:
    {
        QDateEdit* edit = qobject_cast<QDateEdit*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_User_Data_ProduceDateTimeBegin,edit->dateTime());
    }
        break;
    case 9:
    {
        QLineEdit* edit = qobject_cast<QLineEdit*>(ui->tableWidget_settings->cellWidget(rowIndex,1));
        if(edit)
            RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_User_Data_ProductNo,edit->text());
    }
        break;
    default:
        break;
    }

//    refreshSettingValues({rowIndex});

    this->unsetCursor();
    this->setEnabled(true);
}

void RSMConfigMainWindow::onAdjustPushBottonClicked()
{
    int rowIndex = -1;
    for(int i=0;i<ui->tableWidget_adjust->rowCount();i++)
    {
        QWidget*cellWidget = ui->tableWidget_adjust->cellWidget(i,16);
        if(cellWidget == sender())
        {
            rowIndex = i;
            break;
        }
    }

    if(rowIndex == -1)
        return;

    this->setCursor(QCursor(Qt::WaitCursor));
    this->setEnabled(false);

    switch (rowIndex) {
    case 0:
    {
        RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_AdjustCMD,
                                                 0x10);
    }
        break;
    case 1:
    {
        RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmConfig_AdjustCMD,
                                                 0x20);
    }
        break;
//    case 3:
//    {
//        RSMUsb::instance()->writeAdjustCommand(0x30);
//    }
//        break;
    default:
        break;
    }

    refreshAdjustValue({rowIndex});

    this->unsetCursor();
    this->setEnabled(true);
}


bool RSMConfigMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    if ("windows_dispatcher_MSG" == eventType
            || "windows_generic_MSG" == eventType)

        {
            MSG * msg = reinterpret_cast<MSG*>(message);
//            qDebug()<<"windows  message of type "<<eventType<<QString::number(msg->message,16);
            if(msg->message == WM_DEVICECHANGE )
            {

                PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
                switch(msg->wParam)
                {
                case DBT_DEVICEARRIVAL://检测到新设备
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {

                        qDebug() << "DBT_DEVICEARRIVAL";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEQUERYREMOVE://请求移除设备，可能失败  此时刷新不会让移动设备消失
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEQUERYREMOVE";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEQUERYREMOVEFAILED://去除中断
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEQUERYREMOVEFAILED";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVICEREMOVEPENDING://即将删除，仍然有效
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEREMOVEPENDING";
                    }
                    break;

                case DBT_DEVICEREMOVECOMPLETE://设备不见了
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                    {
                        qDebug() << "DBT_DEVICEREMOVECOMPLETE";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_CUSTOMEVENT:
                    if (lpdb->dbch_devicetype == DBT_DEVTYP_HANDLE)
                    {
                        qDebug() << "DBT_CUSTOMEVENT";
//                        refreshDeviceList();
                    }
                    break;

                case DBT_DEVNODES_CHANGED:
                {
                    qDebug() << "DBT_DEVNODES_CHANGED";
                    if(!RSMUsb::instance()->getActivePortName().isEmpty())
                    {
                        if(!deviceDetectTimer.isActive())
                            deviceDetectTimer.start();
                    }
                }
                   break;

                default:
                    qDebug() << msg->wParam;
                }
//                outputDrives();
          }
    }
    return QMainWindow::nativeEvent(eventType,message,result);
}

void RSMConfigMainWindow::on_pushButton_refresh_clicked()
{
    refreshPortValues();
}

void RSMConfigMainWindow::on_checkBox_autoRefresh_clicked(bool checked)
{
    if(checked)
        autoRefreshTimer.start();
    else
        autoRefreshTimer.stop();

    ui->pushButton_refresh->setEnabled(!checked);
}

void RSMConfigMainWindow::on_comboBox_deviceChoose_currentTextChanged(const QString &arg1)
{
    RSMUsb::instance()->setActivePortName(arg1);
    refreshWhenPortChanged();
}

//void RSMConfigMainWindow::on_comboBox_BaudRate_currentTextChanged(const QString &arg1)
//{
//    if(RSMUsb::instance()->getPortHandle())
//    {
//        RSMUsb::instance()->getPortHandle()->setBaudRate(arg1.toInt());
//    }
//}

void RSMConfigMainWindow::on_toolButton_wirelessSwitch_clicked(bool checked)
{
    this->setCursor(QCursor(Qt::WaitCursor));
    this->setEnabled(false);

    RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmCMD_WirelessSwitch,checked);
//    RSMUsb::instance()->waitForMSecs(300);
    this->unsetCursor();
    this->setEnabled(true);
}

void RSMConfigMainWindow::onDeviceDataChanged(int dataAddress,const QVariant& value)
{
    switch (dataAddress) {
    //    RsmCMD_FirstTempData = 0x0000,
    case TopTest::RsmCMD_FirstTempData:
    {
        QList<double> tempValueList = RSMUsb::instance()->getPortTempValues();
        for(int i=0;i<ui->tableWidget_portValue->columnCount();i++)
        {
            ui->tableWidget_portValue->setItem(
                        3,i,
                        new QTableWidgetItem(QString::number(tempValueList[i],'f',1)));
        }
        break;
    }
    //    RsmCMD_FirstVolageData = 0x0010,
    case TopTest::RsmCMD_FirstVolageData:
    {
        QList<double > volValueList = RSMUsb::instance()->getPortVolValues();
        for(int i=0;i<ui->tableWidget_portValue->columnCount();i++)
        {
            ui->tableWidget_portValue->setItem(
                        2,i,
                        new QTableWidgetItem(QString::number(volValueList[i],'f',2)));
        }
        break;
    }
//    ///user data
//    //    RsmCMD_CurrentScaleLevel = 0x0020,
//    case TopTest::RsmCMD_CurrentScaleLevel:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_CurrentScaleLevel]
//                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_InnerTemp;
//    }
    //    RsmCMD_InnerTemp = 0x0021,
    case TopTest::RsmCMD_InnerTemp:
    {
        double pcbTemp = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmCMD_InnerTemp).toDouble();
        ui->label_pcbTemp->setText(QString("PcbTemp:%1(C)").arg(pcbTemp));
        break;
    }
//    //    RsmCMD_BatteryVolage = 0x0022,
//    case TopTest::RsmCMD_BatteryVolage:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_BatteryVolage]
//                = digtalDataList[dataAddress] * 0.01;
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_ErrorCode;
//    }
    //    RsmCMD_ErrorCode = 0x0023,
    case TopTest::RsmCMD_ErrorCode:
    {
        int errorCode = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmCMD_ErrorCode).toInt();
        QString errorString ;
        if(errorCode == TopTest::RsmDeviceError_NoneError)
            errorString = tr("没有错误");
        else
        {
            if(errorCode & TopTest::RsmDeviceError_StorageError)
                errorString += tr("错误");
            if(errorCode & TopTest::RsmDeviceError_TempSensorError)
                errorString += tr("温度传感器18B20错误");
            if(errorCode & TopTest::RsmDeviceError_ClockChipError)
                errorString += tr("时钟芯片错误");
            if(errorCode & TopTest::RsmDeviceError_CrystalError)
                errorString += tr("晶振错误");
        }
        ui->label_errorState->setText(errorString);
        break;
    }
//    //    RsmCMD_SaveCMDCode_Begin = 0x0024,
//    case TopTest::RsmCMD_SaveCMDCode_Begin:
//    {
////        activeSerialData->userData[TopTest::RsmCMD_SaveCMDCode_Begin]
////                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_Device_DateTime;
//    }
//    //    RsmCMD_Device_Data_Year= 0x0030,
//    //    RsmCMD_Device_Data_Moth= 0x0031,
//    //    RsmCMD_Device_Data_Day= 0x0032,
//    //    RsmCMD_Device_Time_Hour= 0x0033,
//    //    RsmCMD_Device_Time_Minuter= 0x0034,
//    //    RsmCMD_Device_Time_Second= 0x0035,
//    case TopTest::RsmCMD_Device_DateTime:
//    {

//        break;
//    }
////    RsmCMD_Port_Number = 0x0036,
//    case TopTest::RsmCMD_Port_Number:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_Port_Number]
//                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_RTU_Address;
//    }
//    RsmCMD_RTU_Address = 0x0037,
    case TopTest::RsmCMD_RTU_Address:
    {
        int address = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmCMD_RTU_Address).toInt();
        ui->tableWidget_settings->setItem(
                    0,0,new QTableWidgetItem(QString("%1").arg(address)));
        break;
    }
////    RsmCMD_Sample_Time_Interal = 0x0038,
//    case TopTest::RsmCMD_Sample_Time_Interal:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_Sample_Time_Interal]
//                = digtalDataList[dataAddress] * 0.05;
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_Start_Mode;
//    }
////    RsmCMD_Start_Mode = 0x0039,
//    case TopTest::RsmCMD_Start_Mode:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_Start_Mode]
//                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_PressStart_DelayTime;
//    }
////    RsmCMD_PressStart_DelayTime = 0x003A,
//    case TopTest::RsmCMD_PressStart_DelayTime:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_PressStart_DelayTime]
//                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_FirstTempStart_Start_Temp;
//    }
////    RsmCMD_FirstTempStart_Start_Temp = 0x003B,
//    case TopTest::RsmCMD_FirstTempStart_Start_Temp:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Start_Temp]
//                = digtalDataList[dataAddress] * 0.1;
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_FirstTempStart_Stop_Temp;
//    }
////    RsmCMD_FirstTempStart_Stop_Temp = 0x003C,
//    case TopTest::RsmCMD_FirstTempStart_Stop_Temp:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Stop_Temp]
//                = digtalDataList[dataAddress] * 0.1;
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_FirstTempStart_Stop_DelayTime;
//    }
////    RsmCMD_FirstTempStart_Stop_DelayTime = 0x003D,
//    case TopTest::RsmCMD_FirstTempStart_Stop_DelayTime:
//    {
//        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Stop_DelayTime]
//                = digtalDataList[dataAddress];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmCMD_PortModel;
//    }
////    RsmCMD_SecondTempStart_Start_Temp = 0x003E,
////    RsmCMD_SecondTempStart_Stop_Temp = 0x003F,
////    RsmCMD_SecondTempStart_Stop_DelayTime = 0x0040,

////    RsmCMD_ThirdAndForwardTempStart_Start_Temp = 0x0041,
////    RsmCMD_ThirdAndForwardTempStart_Stop_Temp = 0x0042,
////    RsmCMD_ThirdAndForwardTempStart_Stop_DelayTime = 0x0043,

////    RsmCMD_TempChart_JudgeMode = 0x0044,
////    RsmCMD_TempChart_ProtJudgeState = 0x0045,

////    RsmCMD_TempChart_JudgeTime1 = 0x0046,
////    RsmCMD_TempChart_JudgeTime2 = 0x0047,
////    RsmCMD_TempChart_JudgeTemp_Max = 0x0048,
////    RsmCMD_TempChart_JudgeTemp_Min = 0x0049,

//    RsmCMD_PortModel = 0x004A,
    case TopTest::RsmCMD_PortModel:
    {
        //0:K 1:J 2:R 3:S 4:B 5:E 6:T 7:N
        if(1/*rows.contains(5)*/)
        {
            int sensorType = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmCMD_PortModel).toInt();
            QString sensorTypeStr;
            switch (sensorType) {
            case 0:
                sensorTypeStr = "K";
                break;
            case 1:
                sensorTypeStr = "J";
                break;
            case 2:
                sensorTypeStr = "R";
                break;
            case 3:
                sensorTypeStr = "S";
                break;
            case 4:
                sensorTypeStr = "B";
                break;
            case 5:
                sensorTypeStr = "E";
                break;
            case 6:
                sensorTypeStr = "T";
                break;
            case 7:
                sensorTypeStr = "N";
                break;
            default:
                sensorTypeStr = "Unknown";
                break;
            }
            ui->tableWidget_settings->setItem(6,0,new QTableWidgetItem(sensorTypeStr));

        }
        break;
    }
////    RsmCMD_PowerRadio = 0x004B,
////    RsmCMD_WirelessSwitch = 0x004B,
    case TopTest::RsmCMD_WirelessSwitch:
    {
        int wirelessSwitch = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmCMD_WirelessSwitch).toInt();
        ui->toolButton_wirelessSwitch->setChecked(wirelessSwitch);
        QString switchStateText;
        if(wirelessSwitch)
            switchStateText = tr("Wireless On");
        else
            switchStateText = tr("Wireless Off");
        ui->label_wirelessState->setText(switchStateText);
    }
        break;

    ///configure data
//        RsmConfig_ConfigPassword = 0x0100,
    case TopTest::RsmConfig_ConfigPassword:
    {
        int passwd = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmConfig_ConfigPassword).toInt();
        ui->toolButton_writeLock->setChecked(passwd == 0x1234);
        QString writeLockStateText;
        if(ui->toolButton_writeLock->isChecked())
            writeLockStateText = tr("可以写入");
        else
            writeLockStateText = tr("不可写入");
        ui->label_lockState->setText(writeLockStateText);
        break;
    }
//        RsmConfig_BatteryType = 0x0101,
    case TopTest::RsmConfig_BatteryType:
    {
        if(1/*rows.contains(1)*/)
        {
            int batteryType = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_BatteryType).toInt();
            QString batteryTypeText;
            if(batteryType == 0)
                batteryTypeText = tr("锂电");
            else
                batteryTypeText = tr("镍氢电池");
            ui->tableWidget_settings->setItem(2,0,new QTableWidgetItem(batteryTypeText));
        }
        break;
    }
//        RsmConfig_PortNumber = 0x0102,
    case TopTest::RsmConfig_PortNumber:
    {
        if(1/*rows.contains(2)*/)
        {
            int maxPortNum = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_PortNumber).toInt();
            ui->tableWidget_settings->setItem(3,0,new QTableWidgetItem(QString("%1").arg(maxPortNum)));
        }
        break;
    }
////        RsmConfig_AdjustCMD = 0x0103,
//    case TopTest::RsmConfig_AdjustCMD:
//    {
//        activeSerialData->configData[TopTest::RsmConfig_AdjustCMD]
//                = digtalDataList[dataAddress - TopTest::RsmConfig_ConfigPassword];
//        emit this->deviceDataChanged(dataAddress);
//        dataAddress = TopTest::RsmConfig_ActiveDevice;
//    }
////        RsmConfig_ActiveDevice = 0x0104,
    case TopTest::RsmConfig_ActiveDevice:
    {
        bool active = RSMUsb::instance()
                ->getDeviceMapValue(TopTest::RsmConfig_ActiveDevice).toBool();
        ui->toolButtona_activeState->setChecked(active);
        QString activeStateText;
        if(active)
            activeStateText = tr("Activated");
        else
            activeStateText = tr("Unactivated");
        ui->label_activeState->setText(activeStateText);
        break;
    }
//        RsmConfig_DeviceCode = 0x0105,
    case TopTest::RsmConfig_DeviceCode:
    {
        if(1/*rows.contains(0)*/)
        {
            ushort deviceID = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_DeviceCode).toUInt();
            qDebug()<<"update device code"<<deviceID;
            ui->tableWidget_settings->setItem(1,0,new QTableWidgetItem(QString("0x%1").arg(deviceID,4,16,QChar('0'))));
            ui->label_id->setText(QString("0x%1").arg(deviceID,4,16,QChar('0')));
        }
        break;
    }
////        RsmConfig_LowScale_ZeroAdjust_Begin = 0x0106,
////        RsmConfig_LowScale_ZeroAdjust_End = 0x0115,
////        RsmConfig_HighScale_ZeroAdjust_Begin = 0x0116,
////        RsmConfig_HighScale_ZeroAdjust_End = 0x0125,
    case TopTest::RsmConfig_LowScale_ZeroAdjust_Begin:
    {
        if(1/*rows.contains(0)*/)
        {
            QList<int> lowZeroValueList = RSMUsb::instance()->getPortZeroAdjustValues();
            for(int i=0;i<16;i++)
            {
                ui->tableWidget_adjust->setItem(0,i, new QTableWidgetItem(QString::number((double)lowZeroValueList[i])));
            }
        }
        break;
    }

////        RsmConfig_LowScale_WholeAdjust_Begin = 0x0126,
////        RsmConfig_LowScale_WholeAdjust_End = 0x0135,
    case TopTest::RsmConfig_LowScale_WholeAdjust_Begin:
    {
        if(1/*rows.contains(1)*/)
        {
            QList<int> lowWholeValueList = RSMUsb::instance()->getPortWholeAdjustValues();
            for(int i=0;i<16;i++)
            {
                ui->tableWidget_adjust->setItem(1,i, new QTableWidgetItem(QString::number((double)lowWholeValueList[i])));
            }
        }
        break;
    }
////        RsmConfig_HighScale_WholeAdjust_Begin = 0x0136,
////        RsmConfig_HighScale_WholeAdjust_End = 0x0145,

////        RsmConfig_AD_Data_Begin = 0x0146,
////        RsmConfig_AD_Data_End = 0x0155,
    case TopTest::RsmConfig_AD_Data_Begin:
    {
        QList<int> adValueList = RSMUsb::instance()->getPortADValues();
        for(int i=0;i<ui->tableWidget_portValue->columnCount();i++)
        {
            ui->tableWidget_portValue->setItem(
                        1,i, new QTableWidgetItem(QString("%1").arg(adValueList[i])));
        }
        break;
    }

////        RsmConfig_MaxValid_Temp = 0x0156,
    case TopTest::RsmConfig_MaxValid_Temp:
    {
        if(1/*rows.contains(3)*/)
        {
            double maxTemp = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_MaxValid_Temp).toDouble();
            ui->tableWidget_settings->setItem(4,0,new QTableWidgetItem(QString("%1").arg(maxTemp)));
        }
        break;
    }
////        RsmConfig_CoolEnd_Adjust_TempValue = 0x0157,
    case TopTest::RsmConfig_CoolEnd_Adjust_TempValue:
    {
        if(1/*rows.contains(4)*/)
        {
            double adjustTemp =  RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_CoolEnd_Adjust_TempValue).toDouble();
            ui->tableWidget_settings->setItem(5,0,new QTableWidgetItem(QString("%1").arg(adjustTemp)));
        }
        break;
    }
////        RsmConfig_User_Data_Begin = 0x0160,
////        RsmConfig_User_Data_ProductNo = RsmConfig_User_Data_Begin,
    case TopTest::RsmConfig_User_Data_ProductNo:
    {
        if(1/*rows.contains(8)*/)
        {
            QString productNo = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_User_Data_ProductNo).toString();
            QString productNoStr = QString("%1").arg(productNo);
            if(productNo == 0)
                productNoStr = tr("暂未设置");
            ui->tableWidget_settings->setItem(9,0,new QTableWidgetItem(productNoStr));
        }
        break;

    }
////        RsmConfig_User_Data_ProduceDateTimeBegin = RsmConfig_User_Data_ProductNo+2,
    case TopTest::RsmConfig_User_Data_ProduceDateTimeBegin:
    {
        if(1/*rows.contains(7)*/)
        {
            QDateTime productDateTime =  RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_User_Data_ProduceDateTimeBegin).toDateTime();
            QString productDateTimeStr = productDateTime.toString("yyyy/MM/dd");
            if(productDateTimeStr.isEmpty())
                productDateTimeStr = tr("暂未设置");
            ui->tableWidget_settings->setItem(8,0,new QTableWidgetItem(productDateTimeStr));
        }
        break;

    }
////        RsmConfig_User_Data_DeviceModel = RsmConfig_User_Data_ProduceDateTimeBegin+6,
    case TopTest::RsmConfig_User_Data_DeviceModel:
    {
        if(1/*rows.contains(6)*/)
        {
            QString deviceModelStr = RSMUsb::instance()
                    ->getDeviceMapValue(TopTest::RsmConfig_User_Data_DeviceModel).toString();
    //        QString deviceModelStr = QString("0x%1").arg(productNo,0,16);
            if(deviceModelStr.isNull())
                deviceModelStr = tr("暂未设置");
            ui->tableWidget_settings->setItem(7,0,new QTableWidgetItem(deviceModelStr));
        }
        break;
    }
////        RsmConfig_User_Data_DeviceModel_End = RsmConfig_User_Data_DeviceModel+10,
////        RsmConfig_User_Data_End = 0x09F

    default:
        break;
    }
}

void RSMConfigMainWindow::on_spinBox_rtu_valueChanged(int arg1)
{
    qDebug()<<"on_spinBox_rtu_valueChanged"<<arg1;
    this->reset();
    RSMUsb::instance()->setDeviceAddress(arg1);
    refreshWhenPortChanged();
}

void RSMConfigMainWindow::on_comboBox_linkType_currentIndexChanged(int index)
{
    qDebug()<<"on_comboBox_linkType_currentIndexChanged"<<index;
    this->reset();
    int baudRate = ui->comboBox_linkType->currentData().toInt();
    RSMUsb::instance()->setBaudRate(baudRate);
    RSMUsb::instance()->closeCurrentPort();

    qDebug()<<"set port baud rate"<<RSMUsb::instance()->getBaudRate();

    if(!index)
    {
        ui->spinBox_rtu->setValue(255);
        ui->label_rtu->setVisible(false);
        ui->spinBox_rtu->setVisible(false);
    }
    else
    {
        ui->label_rtu->setVisible(true);
        ui->spinBox_rtu->setVisible(true);
    }

    if(!deviceDetectTimer.isActive())
    {
        deviceDetectTimer.start();
    }

}

void RSMConfigMainWindow::resetLink()
{
    this->setEnabled(false);
    qDebug()<<"FormLinkSetting::resetLink"<<true;

    RSMUsb::instance()->closeCurrentPort();
    RSMUsb::instance()->setActivePortName(QString());
    ui->comboBox_deviceChoose->blockSignals(true);
    ui->comboBox_deviceChoose->clear();
    if(!deviceDetectTimer.isActive())
    {
        deviceDetectTimer.start();
    }
//    ui->label_state->setText(tr("连接断开"));
//    ui->label_wirelessSwitch->setEnabled(false);
//    ui->checkBox_wireless->setEnabled(false);
    ui->comboBox_deviceChoose->blockSignals(false);

//    emit this->deviceDisconnected();

    connect(RSMUsb::instance(),SIGNAL(aviablePortChanged(QStringList)),
            this,SLOT(onAviablePortChanged(QStringList)));
    this->setEnabled(true);
}

void RSMConfigMainWindow::onWriteLockToggled(bool checked)
{
    if(checked && ui->toolButton_writeLock->isEnabled())
    {
        ui->toolButtona_activeState->setEnabled(true);
    }else
    {
        ui->toolButtona_activeState->setEnabled(false);
    }
}

void RSMConfigMainWindow::onCurrentDeviceNoRespond(bool reset)
{
    qDebug()<<"RSMConfigMainWindow::onCurrentDeviceNoRespond"<<reset;
    if(reset)
    {
        qDebug()<<"RSMConfigMainWindow::onCurrentDeviceNoRespond"<<reset;
        this->blockSignals(true);
        ui->comboBox_deviceChoose->clear();
        this->reset();
        this->blockSignals(false);
        if(!deviceDetectTimer.isActive())
        {
            deviceDetectTimer.start();
        }

    }
}
