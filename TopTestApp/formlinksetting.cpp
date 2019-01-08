#pragma execution_character_set("utf-8")


#include "formlinksetting.h"
#include "ui_formlinksetting.h"

#include "rsmusb.h"
#include "serialporttransfer.h"
#include "rsmcoresignalandslotcontrolcenter.h"

#include <QDebug>

#include <Windows.h>
#include <dbt.h>

FormLinkSetting * formLink = 0;
FormDialog* dialogLink = 0;

FormLinkSetting *FormLinkSetting::instance()
{
    if(!formLink)
        formLink = new FormLinkSetting;
    return formLink;
}

//bool FormLinkSetting::judgeLink()
//{
//    if
//}

FormDialog *FormLinkSetting::dialogInstance()
{
    if(!dialogLink)
    {
        dialogLink = new FormDialog(FormLinkSetting::instance());
        dialogLink->setWindowTitle(
                    QCoreApplication::translate("FormLinkSetting","连接设置"));
    }
    return dialogLink;
}

FormLinkSetting::FormLinkSetting(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::FormLinkSetting)
{
    ui->setupUi(this);
    init();

    firstShow = true;

    connect(&deviceDetectTimer,SIGNAL(timeout()),
            SLOT(refreshDeviceList()));

    connect(RSMUsb::instance(),SIGNAL(currentDeviceNoRespond(bool)),
            this,SLOT(onCurrentDeviceNoRespond(bool)));
    connect(RSMUsb::instance(),SIGNAL(currentDeviceError()),
            this,SLOT(onCurrentDeviceError()));

    connect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataReady(int,QVariant)));

    connect(RSMUsb::instance(),SIGNAL(activePortChanged(QString)),
            this,SLOT(onActivePortChanged(QString)));

    deviceDetectTimer.setInterval(1000);
}

FormLinkSetting::~FormLinkSetting()
{
    qDebug()<<"~FormLinkSetting()";
    delete ui;
}

void FormLinkSetting::onButtonClicked(int button)
{

}

int FormLinkSetting::standerButtons()
{
    return QDialogButtonBox::Ok;
}

void FormLinkSetting::showEvent(QShowEvent *event)
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        if(!deviceDetectTimer.isActive())
            deviceDetectTimer.start();
    }
    emit RSMCoreSignalAndSlotControlCenter::instance()->startShowDialog();
}

void FormLinkSetting::hideEvent(QHideEvent *event)
{
    emit RSMCoreSignalAndSlotControlCenter::instance()->endShowDialog();
}



void FormLinkSetting::init()
{
    ui->spinBox_rtu->blockSignals(true);
    ui->comboBox_linkType->blockSignals(true);

    ui->spinBox_rtu->setMinimum(1);
    ui->spinBox_rtu->setMaximum(255);
    ui->spinBox_rtu->setValue(255);

    ui->comboBox_linkType->addItem(tr("Usb"),500000);
    ui->comboBox_linkType->addItem(tr("Wireless"),115200);

    ui->label_wirelessSwitch->setEnabled(false);
    ui->checkBox_wireless->setEnabled(false);

    ui->spinBox_rtu->blockSignals(false);
    ui->comboBox_linkType->blockSignals(false);

    this->resetLink();
}

void FormLinkSetting::on_comboBox_linkType_currentIndexChanged(const QString &arg1)
{
    RSMUsb::instance()->setBaudRate(ui->comboBox_linkType->currentData().toInt());

    if(ui->comboBox_linkType->currentData().toInt() == 500000)
    {
        ui->label_wirelessSwitch->setVisible(true);
        ui->checkBox_wireless->setVisible(true);
    }
    else
    {
        ui->label_wirelessSwitch->setVisible(false);
        ui->checkBox_wireless->setVisible(false);
    }

    resetLink();
}

void FormLinkSetting::refreshDeviceList()
{
    qDebug() << "FormLinkSetting::refreshDeviceList()";

    RSMUsb::instance()->refreshAviablePortList();


}

void FormLinkSetting::on_comboBox_deviceChoose_currentTextChanged(const QString &arg1)
{
    qDebug()<<"on_comboBox_deviceChoose_currentTextChanged"<<arg1;
    RSMUsb::instance()->setActivePortName(arg1);
    emit this->activePortChanged(arg1);
}

void FormLinkSetting::on_spinBox_rtu_valueChanged(int arg1)
{
    qDebug()<<"on_spinBox_rtu_valueChanged"<<arg1;
//    this->reset();
    RSMUsb::instance()->setDeviceAddress(arg1);
    resetLink();
}

void FormLinkSetting::onCurrentDeviceNoRespond(bool reset)
{
    qDebug()<<"FormLinkSetting::onCurrentDeviceNoRespond"<<reset;
    if(reset)
    {
        resetLink();
    }
}

void FormLinkSetting::onCurrentDeviceError()
{
    resetLink();
}

void FormLinkSetting::onAviablePortChanged(const QStringList &portList)
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

void FormLinkSetting::hideLinkTypeAndRTUAddress()
{
    ui->label_rtu->setVisible(false);
    ui->label_linkType->setVisible(false);
    ui->spinBox_rtu->setVisible(false);
    ui->comboBox_linkType->setVisible(false);
    ui->label_rtuWarning->setVisible(false);
}

void FormLinkSetting::showLinkTypeAndRTUAddress()
{
    ui->label_rtu->setVisible(true);
    ui->label_linkType->setVisible(true);
    ui->spinBox_rtu->setVisible(true);
    ui->comboBox_linkType->setVisible(true);
    ui->label_rtuWarning->setVisible(true);
}

void FormLinkSetting::resetLink()
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
    ui->label_state->setText(tr("连接断开"));
    ui->label_wirelessSwitch->setEnabled(false);
    ui->checkBox_wireless->setEnabled(false);
    ui->comboBox_deviceChoose->blockSignals(false);

    emit this->deviceDisconnected();

    connect(RSMUsb::instance(),SIGNAL(aviablePortChanged(QStringList)),
            this,SLOT(onAviablePortChanged(QStringList)));
    this->setEnabled(true);
}


bool FormLinkSetting::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
                        deviceDetectTimer.start();
                }
                   break;

                default:
                    qDebug() << msg->wParam;
                }
//                outputDrives();
          }
    }
    return QWidget::nativeEvent(eventType,message,result);
}

void FormLinkSetting::on_checkBox_wireless_clicked(bool checked)
{
    RSMUsb::instance()->writeDeviceValueItem(TopTest::RsmCMD_WirelessSwitch,checked);
}

void FormLinkSetting::onDeviceDataReady(int address, const QVariant &value)
{
    qDebug()<<"FormLinkSetting::onDeviceDataReady"<<address<<value;
    switch (address) {
    case RsmCMD_WirelessSwitch:
        ui->checkBox_wireless->setChecked(
                    RSMUsb::instance()->getDeviceMapValue(RsmCMD_WirelessSwitch).toBool());
        break;
    default:
        break;
    }
}

void FormLinkSetting::onActivePortChanged(const QString &port)
{
    emit this->deviceConnected();
    ui->label_state->setText(tr("已连接"));
    ui->label_wirelessSwitch->setEnabled(true);
    ui->checkBox_wireless->setEnabled(true);
    RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_WirelessSwitch);
}

void FormLinkSetting::on_pushButton_reset_clicked()
{
    this->resetLink();
}
