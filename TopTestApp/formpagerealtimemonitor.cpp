#pragma execution_character_set("utf-8")

#include "formpagerealtimemonitor.h"
#include "ui_formpagerealtimemonitor.h"

#include "toptestenum.h"
#include "rsmcoresignalenum.h"

#include "dialogharddevicesetting.h"
#include "rsmcoresignalandslotcontrolcenter.h"

#include "formdialog.h"
#include "rsmusb.h"
#include "serialporttransfer.h"
#include "dialogportdefine.h"
#include "formlinksetting.h"
#include "dialogsavedatafile.h"
#include "clabso_ft_dpi.h"
#include "formpagedatavisual.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

FormPageRealtimeMonitor::FormPageRealtimeMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPageRealtimeMonitor)
{
    recordTime = 0;
    monitorOn = false;
    portNum = 16;
    sampleTime = 1.0;
    lineVelocity = 0.3;
//    inited = false;

    ui->setupUi(this);

    init();

    initToolButtonGroup();

//    ui->widget_chart->getToolGroupWidget()->setVisible(false);

    autoRefresh.setInterval(1000);
//    autoRefresh.start();
    connect(&autoRefresh,SIGNAL(timeout()),
            this,SLOT(onTimeOut()));

    connect(ui->widget_chart,SIGNAL(currentTimeChanged(double)),
            this,SLOT(onRulerSliderChanged(double)));

    connect(RSMUsb::instance(),SIGNAL(currentDeviceNoRespond(bool)),
            this,SLOT(onCurrentDeviceNoRespond(bool)));

    connect(RSMUsb::instance(),SIGNAL(deviceDataChanged(int,QVariant)),
            this,SLOT(onDeviceDataChanged(int,QVariant)));

    connect(this,SIGNAL(sendCountChanged(int)),
            this,SLOT(onSendReciveChanged()));
    connect(this,SIGNAL(reciveCountChanged(int)),
            this,SLOT(onSendReciveChanged()));

    connect(this,SIGNAL(dataCountChanged()),
            this,SLOT(onDataCountChanged()));

    connect(this,SIGNAL(monitorOnChanged(bool)),
            SLOT(onMonitorChanged(bool)));

    connect(this,SIGNAL(recordStateOnChanged(bool)),
            SLOT(onRecordOnChanged(bool)));

    ui->groupBox_PortValue_2->setVisible(false);

    ui->widget_chart->setVelocity(0.3);

}

FormPageRealtimeMonitor::~FormPageRealtimeMonitor()
{
    autoRefresh.stop();
    delete ui;
}

void FormPageRealtimeMonitor::initLink()
{
    if(RSMUsb::instance()->getActivePortName().isEmpty())
        return;

    RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_Port_Number);
    RSMUsb::instance()->waitForMSecs(30);
    RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_Sample_Time_Interal);

    if(RSMUsb::instance()->getBaudRate() == 500000)
        autoRefresh.setInterval(1000);
    else
        autoRefresh.setInterval(1000);
    autoRefresh.start();

}

int FormPageRealtimeMonitor::getReciveDataCount() const
{
    return reciveDataCount;
}

void FormPageRealtimeMonitor::setReciveDataCount(int value)
{
    if(reciveDataCount == value)
        return;
    reciveDataCount = value;
    emit this->dataCountChanged();
}

int FormPageRealtimeMonitor::getSendDataCount() const
{
    return sendDataCount;
}

void FormPageRealtimeMonitor::setSendDataCount(int value)
{
    if(sendDataCount == value)
        return;
    sendDataCount = value;
    emit this->dataCountChanged();
}

int FormPageRealtimeMonitor::getReciveCount() const
{
    return reciveCount;
}

void FormPageRealtimeMonitor::setReciveCount(int value)
{    if(value == reciveCount)
        return;

    reciveCount = value;
    qDebug()<<"emit this->reciveCountChanged(value);"<<value;
    emit this->reciveCountChanged(value);
     //    onSendReciveChanged();
}

void FormPageRealtimeMonitor::resetTime()
{
    recordTime = 0.0;

    this->testDateTime = QDateTime::currentDateTime();
    ui->widget_chart->setTestDateTime(this->testDateTime);
}

int FormPageRealtimeMonitor::getSendCount() const
{
    return sendCount;
}

void FormPageRealtimeMonitor::setSendCount(int value)
{
    if(value == sendCount)
        return;

    sendCount = value;
    qDebug()<<"emit this->sendCountChanged(value);"<<value;
    emit this->sendCountChanged(value);
//    onSendReciveChanged();
}

void FormPageRealtimeMonitor::onNeedToShowDailog(QWidget* w,int sig)
{

    if(w != this)
        return;

    qDebug()<<"FormPageRealtimeMonitor::onNeedToShowDailog"<<sig;

    emit RSMCoreSignalAndSlotControlCenter::instance()->startShowDialog();


    switch (sig) {
    case Dialog_Monitor_LinkSetting:
    {
        FormLinkSetting::instance()->setVisible(true);
        FormLinkSetting::dialogInstance()->exec();
//        FormDialog formDialog(FormLinkSetting::instance(),this);
//        formDialog.setWindowTitle(tr("连接设备"));
//        formDialog.exec();
    }
        break;
    case Dialog_Monitor_Switch:
    {

        if(RSMUsb::instance()->getActivePortName().isEmpty())
        {
            if(monitorOn)
                setMonitorOn(false);
            else
                QMessageBox::critical(this,tr("Error"),tr("Please connect device first!"));
            break;
        }

        if(!autoRefresh.isActive())
        {
            initLink();
        }

        setMonitorOn(!monitorOn);
    }
        break;

    case Dialog_Monitor_Record_Switch:
    {
        if(monitorOn)
        {
            QMessageBox::critical(this,tr("Error"),tr("Please turn off monitor switch first!"));
            break;
        }

//        if(RSMUsb::instance()->getActivePortName().isEmpty())
//        {
//            if(monitorOn)
//                setMonitorOn(false);
//            else
//                QMessageBox::critical(this,tr("Error"),tr("Please connect device first!"));
//            break;
//        }

//        else if(!monitorOn)
//        {
//            QMessageBox::critical(this,tr("Error"),tr("Please turn on monitor switch first!"));
//            break;
//        }

//        if(!autoRefresh.isActive())
//        {
//            initLink();
//        }

        setRecordStateOn(!recordStateOn);
    }
        break;


    case Dialog_DataPortDefine:
    {
        DialogPortDefine* dialog = new  DialogPortDefine(this);
        dialog->setCurrentPortInfo(ui->widget_chart->getPortInfoMap());
        connect(dialog,SIGNAL(portInfoChanged(QMap<int,QPair<QString,QColor> >)),
                ui->widget_chart,SLOT(setPortInfos(QMap<int,QPair<QString,QColor> >)));

        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("通道定义"));
        formDialog.exec();
    }
        break;
    case Dialog_Monitor_Reset:
    {
        this->onReset();
    }
        break;
    case Dialog_SetVelocity:
    {
        bool ok = false;
        double velocity = QInputDialog::getDouble(this,
                                tr("Set Line Velocity"),
                                tr("Line Velocity (m/min)"),
                                ui->widget_chart->getVelocity() * 60,
                                0,
                                1e8,
                                3,
                                &ok);
        if(ok)
            ui->widget_chart->setVelocity(velocity/60);

//        this->onReset();
    }
        break;
    case Dialog_FileSave:
    {

        if(monitorOn)
        {
            QMessageBox::critical(this,tr("Error"),tr("Please stop monitor first!"));
            break;
        }

        DialogSaveDataFile* dialog = new DialogSaveDataFile(this);

        connect(dialog,SIGNAL(saveCurrentFileToPath(QString)),
                this,SLOT(onSaveFileToPath(QString)));

        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("数据输出"));
        formDialog.exec();
    }
        break;
    default:
        break;
    }

    emit RSMCoreSignalAndSlotControlCenter::instance()->endShowDialog();

}

void FormPageRealtimeMonitor::showRealLCDValueList(uchar portNum, QList<double> tempValueList)
{

    for(int i = 0; (i< tempValueList.count()) && (i < portNum) ; i++ )
    {
        QLCDNumber* lcdNumber = this->findChild<QLCDNumber*>(QString("lcdNumber_%1").arg(i+1));
        if(lcdNumber)
        {
//            qDebug()<<"go to display value of port"<<i+1;
            lcdNumber->display(tempValueList[i]);
        }
    }
}

void FormPageRealtimeMonitor::showCurrentLCDValueList(uchar portNum, QList<double> tempValueList)
{
    for(int i = 0; (i< tempValueList.count()) && (i < portNum) ; i++ )
    {
        QLCDNumber* lcdNumber = this->findChild<QLCDNumber*>(QString("lcdNumberCurrent_%1").arg(i+1));
        if(lcdNumber)
        {
//            qDebug()<<"go to display value of port"<<i+1;
            lcdNumber->display(tempValueList[i]);
        }
    }
}

void FormPageRealtimeMonitor::setPortNum(uchar portNum)
{
    foreach (QLCDNumber* lcdNumber, this->findChildren<QLCDNumber*>()) {
        if(QString(lcdNumber->objectName().split("_").last()).toInt()>portNum)
            lcdNumber->setVisible(false);
        else
            lcdNumber->setVisible(true);
    }
    ui->widget_chart->setPortNum(portNum);
}

void FormPageRealtimeMonitor::init()
{
//    QFont TNRFont =  this->font();

    foreach (QLCDNumber* lcdNumber, this->findChildren<QLCDNumber*>()) {

//        lcdNumber->setStyleSheet("font: 75 10pt \"Times New Roman\";");

    //    TNRFont.setFamily("Times New Roman");
//        TNRFont.setPointSize(10);
//        TNRFont.setBold(true);
//        lcdNumber->setFont(TNRFont);
        qDebug()<<"lcdNumber"<<lcdNumber;
        lcdNumber->setDigitCount(6);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);
        //设置数字字体：Outline,轮廓突出，颜色为背景色；Filled，内部填充型，颜色为黑色；Flat,平面型
        QString qss = QString("QLCDNumber{background:#ffffff;color:blue;}");
        lcdNumber->setStyleSheet(qss);
//        lcdNumber->setMinimumWidth(ClabsoFTDpi::convertSize(50));
//        lcdNumber->setMaximumWidth(ClabsoFTDpi::convertSize(50));
    }
}

void FormPageRealtimeMonitor::refresh()
{
    qDebug()<<"FormPageRealtimeMonitor::refresh()";
    ///get data from hardware
//    autoRefresh.stop();
#ifdef Test
    addTestDataToMonitor();
#else
    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        autoRefresh.start();
        return;
    }


    if(recordStateOn && recordState != 2)
    {
        qDebug()<<"try read record state";
        RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_RecordState);
        return;
    }

    ///重置之后第一次读取数据为起点
    if(firstRecordStart)
    {
        resetTime();
        firstRecordStart = false;
    }

    setSendCount(sendCount+1);
    setSendDataCount(sendDataCount+readCount);

    if(readCount>5)
        readCount = 5;
    else if(readCount<1)
        readCount = 1;

    qDebug()<<"FormPageRealtimeMonitor::refresh() readCount"<<readCount<<this->portNum;

    RSMUsb::instance()->readRealtimeFileContentNew(2 * this->portNum * readCount );
    if(readCount == 1 && !(sendCount % 5) )
    {
        RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_InnerTemp);
    }
#endif

//    autoRefresh.start();
}

void FormPageRealtimeMonitor::onRulerSliderChanged(double time)
{
    //show port values in time
    QList<double> tempValueList = ui->widget_chart->getPortValueListInTime(time);
    qDebug()<<"origin temp value list"<<tempValueList;

    showCurrentLCDValueList(portNum,tempValueList);
}

void FormPageRealtimeMonitor::addTestDataToMonitor()
{
    this->setPortNum(2);
    qDebug()<<"add test data"<<recordTime;
//    ui->widget_chart->setSampleTime(1);
//    ui->widget_chart->addOneSampleData(recordTime,{sin(recordTime/10.0),cos(recordTime/10.0)});
    showRealLCDValueList(2,{sin(recordTime/10.0),cos(recordTime/10.0)});
}

void FormPageRealtimeMonitor::onTimeOut()
{
    if(readCount)
    {
        ui->label_state->setText(tr("Data Lost"));
    }
    else
    {
        ui->label_state->setText(tr("Normal"));
    }
    readCount++;
    if(monitorOn)
    {
        if(!this->testDateTime.isValid())
        {
            this->testDateTime = QDateTime::currentDateTime();
            ui->widget_chart->setTestDateTime(this->testDateTime);
        }
        refresh();
    }
    recordTime += autoRefresh.interval() * 0.001;
}

void FormPageRealtimeMonitor::onCurrentDeviceNoRespond(bool reset)
{
    ui->label_state->setText(tr("Device Error"));
//    if(reset)
//        autoRefresh.start();
//    autoRefresh.stop();
//    QMessageBox::warning(this,tr("Warning"),tr("Monitor is paused caused by disconnection!"));
}

void FormPageRealtimeMonitor::onReset()
{
    qDebug()<<"FormPageRealtimeMonitor::onReset()";
    bool on = monitorOn;
    int rState = recordState;
    autoRefresh.stop();
    monitorOn = false;
    recordState = 0;

    readCount = 0;
    setSendCount(0);
    setReciveCount(0);
    setSendDataCount(0);
    setReciveDataCount(0);

    //reset data in chart
    ui->widget_chart->clearData();
    //reset data in table
    initLink();

    resetTime();

    firstRecordStart = true;
    monitorOn = on;
    recordState = rState;

    RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_RecordState);
}

void FormPageRealtimeMonitor::onSaveFileToPath(const QString& filePath)
{
    qDebug()<<"FormPageRealtimeMonitor::onSaveFileToPath"<<filePath;
    TopTest::FileData fileData;
    fileData.pointData = ui->widget_chart->getCurrentCurveData();
    TopTest::RecordInfoData info;
    info.dateTime = this->testDateTime;
    info.portNum = this->portNum;
    info.sampleTime = autoRefresh.interval()/1000.0 /*this->sampleTime*/;
    info.LineVelocity = ui->widget_chart->getVelocity();
    info.fileName = "RsmData_"+info.dateTime.toString("yyyyMMdd-hhmmss")
            +"_"+QString::number(portNum)+"_"+QString::number(info.sampleTime);
    fileData.info = info;
    FormPageDataVisual::saveDataFile(filePath,fileData,
                                     QList<QMap<QString,QVariant>>()
                                     <<QMap<QString,QVariant>()
                                     <<QMap<QString,QVariant>()
                                     <<QMap<QString,QVariant>(),
                                     TopTest::WaveInfo());

}

void FormPageRealtimeMonitor::onRuntimeFileDataReady(const QByteArray &array)
{
    //on file data ready
    if(array.contains("read file error"))
    {
        qDebug()<<"FormPageRealtimeMonitor::onRuntimeFileDataReady"<<"file error";
        return;
    }

    auto recordList = RSMUsb::instance()->parserFileDataToRecordList(this->portNum,array);
    if(recordList.size() > readCount)
        return;

    setReciveCount(reciveCount+1);
    setReciveDataCount(reciveDataCount + recordList.size());

    qDebug()<<"recive/send"<<reciveCount<<"/"<<sendCount;

    double value = recordTime ;
    int nowIndex = 0;
    foreach (auto tempValueList, recordList) {
        ///show it on lcd
        qDebug()<<"temp value list"<<tempValueList;

        showRealLCDValueList(this->portNum, tempValueList);
//        showCurrentLCDValueList(this->portNum, tempValueList);
        ///add it on curve
        ui->widget_chart->addCurveData(value,array.mid(nowIndex,2 * this->portNum));

        ///update slider position
        ui->widget_chart->moveSliderToTime(value);
        qDebug()<<"move slider"<<value;

        value -= this->sampleTime;
        nowIndex += 2 * this->portNum;
    }

    readCount=0;
}

void FormPageRealtimeMonitor::onDeviceDataChanged(int dataAddress, const QVariant &data)
{
    this->blockSignals(true);

    //    this->portNum = RSMUsb::instance()
    //            ->getDeviceMapValue(TopTest::RsmCMD_Port_Number).toInt();
    //    this->sampleTime = RSMUsb::instance()
    //            ->getDeviceMapValue(TopTest::RsmCMD_Sample_Time_Interal).toDouble();
        qDebug()<<"portNum is "<<portNum<<"sampleTime "<<sampleTime;


    switch (dataAddress) {
    case TopTest::RsmCMD_InnerTemp:
    {
        double internalTemp = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_InnerTemp).toDouble();

        ui->label_temp->setText(QString("%1 %2")
              .arg(QString::number(internalTemp,'f',1))
               .arg(TopTestCore::instance()->getCurrentTemputureUnitStr()));
        qDebug()<<"FormPageRealtimeMonitor::onDeviceDataChanged temp is"<<internalTemp;
    }
        break;
    case TopTest::RsmCMD_Port_Number:
    {
       portNum = RSMUsb::instance()->getDeviceMapValue(
                   TopTest::RsmCMD_Port_Number).toInt();
       setPortNum(portNum);
       break;
    }
    case TopTest::RsmCMD_Sample_Time_Interal:
    {
       this->sampleTime = RSMUsb::instance()
                        ->getDeviceMapValue(TopTest::RsmCMD_Sample_Time_Interal).toDouble();
       break;
    }

    case TopTest::RsmCMD_RecordState:
    {
        this->recordState = RSMUsb::instance()->getDeviceMapValue(
                    TopTest::RsmCMD_RecordState).toInt();
        break;
    }

    default:
        break;
    }

    this->blockSignals(false);

}

void FormPageRealtimeMonitor::onSendReciveChanged()
{
    qDebug()<<"onSendReciveChanged";
    ui->label_network->setText(QString("%1 : %2").arg(reciveCount).arg(sendCount));
}

void FormPageRealtimeMonitor::initToolButtonGroup()
{

//    ui->widget_chart->setVelocity(0.3);

    ui->listWidget->setMaximumWidth(120);

    ui->listWidget->addGroup(tr("公共"));
    ui->listWidget->addItemToCurrentGroup(tr("硬件连接"),
                                          ":/images/ic_home_yingjianshezhi.png",
                                          tr("Set link to device"),
                                          Dialog_Monitor_LinkSetting);
    ui->listWidget->addItemToCurrentGroup(tr("仿真开关"),
                                          ":/images/ic_weixuanzhongtubiao.png",
                                          tr("Start/Pause Monitor"),
                                          Dialog_Monitor_Switch);

    ui->listWidget->addItemToCurrentGroup(tr("标志位检查"),
                                          ":/images/ic_weixuanzhongtubiao.png",
                                          tr("Check Device State before read data"),
                                          Dialog_Monitor_Record_Switch);

    ui->listWidget->addItemToCurrentGroup(tr("重置"),
                                          ":/images/ic_home_yingjianshezhi.png",
                                          tr("Clear data and reset"),
                                          Dialog_Monitor_Reset);
    ui->listWidget->addGroup(tr("设置"));
//    ui->listWidget->addItemToCurrentGroup(tr("AxisSet"),
//                                          ":/images/ic_home_yingjianshezhi.png",
//                                          tr("Set chart axis infos"),
//                                          Chart_SetAxis
//                                          );
    ui->listWidget->addItemToCurrentGroup(tr("通道定义"),
                                          ":/images/ic_tongdaodingyi.png",
                                          tr("set port infos"),
                                          Dialog_DataPortDefine
                                          );
    ui->listWidget->addItemToCurrentGroup(tr("设置线速度"),
                                          ":/images/ic_tongdaodingyi.png",
                                          tr("Set Line velocity"),
                                          Dialog_SetVelocity
                                          );

    ui->listWidget->addItemToCurrentGroup(tr("生成文件"),
                                          ":/images/ic_tongdaodingyi.png",
                                          tr("Create data file from monitor"),
                                          Dialog_FileSave
                                          );

    ui->listWidget->rootWidget = this;

    connect(RSMCoreSignalAndSlotControlCenter::instance(),SIGNAL(needChangeToPage(QWidget*,int)),
            SLOT(onNeedToShowDailog(QWidget*,int)));

    ///TODO:完成重置和保存结果

}

bool FormPageRealtimeMonitor::getRecordStateOn() const
{
    return recordStateOn;
}

void FormPageRealtimeMonitor::setRecordStateOn(bool value)
{
    if(recordStateOn == value)
        return;
    recordStateOn = value;
    emit this->recordStateOnChanged(recordStateOn);
}

bool FormPageRealtimeMonitor::getMonitorOn() const
{
    return monitorOn;
}

void FormPageRealtimeMonitor::setMonitorOn(bool value)
{
    if(monitorOn == value)
        return;
    monitorOn = value;
    emit this->monitorOnChanged(monitorOn);
}

void FormPageRealtimeMonitor::on_pushButton_refresh_clicked()
{
    setSendCount(0);
    setReciveCount(0);
}

void FormPageRealtimeMonitor::onMonitorChanged(bool on)
{
    QString iconPath = ":/images/ic_weixuanzhongtubiao.png";
    if(on)
    {
        iconPath = ":/images/ic_xuanzhongxiaoguo.png";
    }
    ui->listWidget->setCurrentItemHeaderPath(iconPath);

    if(on)
    {
        connect(RSMUsb::instance(),SIGNAL(fileContentReady(QByteArray)),
                this,SLOT(onRuntimeFileDataReady(QByteArray)));
    }
    else
    {
        disconnect(RSMUsb::instance(),SIGNAL(fileContentReady(QByteArray)),
                this,SLOT(onRuntimeFileDataReady(QByteArray)));
    }
}

void FormPageRealtimeMonitor::onRecordOnChanged(bool on)
{
    this->onReset();

    QString iconPath = ":/images/ic_weixuanzhongtubiao.png";
    if(on)
    {
        iconPath = ":/images/ic_xuanzhongxiaoguo.png";
    }
    ui->listWidget->setCurrentItemHeaderPath(iconPath);

//    if(on)
//    {
//        connect(RSMUsb::instance(),SIGNAL(fileContentReady(QByteArray)),
//                this,SLOT(onRuntimeFileDataReady(QByteArray)));
//    }
//    else
//    {
//        disconnect(RSMUsb::instance(),SIGNAL(fileContentReady(QByteArray)),
//                this,SLOT(onRuntimeFileDataReady(QByteArray)));
//    }
}

void FormPageRealtimeMonitor::onDataCountChanged()
{
    ui->label_data->setText(QString("%1 : %2").arg(reciveDataCount).arg(sendDataCount));
}
