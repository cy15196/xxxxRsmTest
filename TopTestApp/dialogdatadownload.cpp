#pragma execution_character_set("utf-8")

#include "dialogdatadownload.h"
#include "ui_dialogdatadownload.h"

#include <QDateTime>
#include <QCheckBox>
#include <QDebug>

#include "dialogharddevicesetting.h"
#include "formdialog.h"
#include "rsmusb.h"
#include "formlinksetting.h"

DialogBase* DialogDataDownload::linkWidgetTemp = 0;

inline QCheckBox* addCheckBoxToTable(QTableWidget* table, int row, int column)
{

    QWidget* widget = new QWidget();
    widget->setLayout(new QHBoxLayout);
    widget->layout()->setMargin(0);

    QCheckBox* checkBox = new QCheckBox(widget);
    widget->layout()->addWidget(checkBox);
    widget->layout()->setAlignment(checkBox,Qt::AlignCenter);

    table->setCellWidget(row, column, widget);

    return checkBox;
}

inline QList<TopTest::RecordInfoData> parseRecordString(QString str)
{
    if(str.isEmpty())
        return {};
    QList<TopTest::RecordInfoData> ret;
    QStringList recordList = str.split(";");
    qDebug()<<"str"<<str<<"recordList"<<recordList;
    if(recordList.isEmpty())
        return {};
    recordList.removeAll("");
    QString headerInfo = recordList.takeFirst();
    foreach (QString fileStr, recordList) {
        QStringList fileInfoList = fileStr.split(",");
        Q_ASSERT(fileInfoList.count()==2);
        TopTest::RecordInfoData data;
        QString fileName = QString(fileInfoList.first()).split(":").at(1);
        QStringList nameInfoList = fileName.split("-");
        data.fileName = fileName;
        data.dateTime = QDateTime::fromString(nameInfoList.takeFirst(),"yyyyMMddhhmmss");
        data.portNum = nameInfoList.takeFirst().toInt();
        data.sampleTime = nameInfoList.takeFirst().remove(".bin").toDouble();
        data.dataCount = QString(fileInfoList.at(1)).split(":").at(1).toInt()/2 / data.portNum ;
        ret<<data;
    }

    return  ret;
}

DialogDataDownload::DialogDataDownload(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogDataDownload)
{
    ui->setupUi(this);

    portNum = 16;

    linkWidgetTemp = FormLinkSetting::dialogInstance()->getCentralDialog();
    qDebug()<<"linkWidgetTemp"<<linkWidgetTemp;
    ui->linkLayout->addWidget(linkWidgetTemp);
    FormLinkSetting::instance()->hideLinkTypeAndRTUAddress();
    FormLinkSetting::instance()->setVisible(true);
    FormLinkSetting::dialogInstance()->hide();

    connect(FormLinkSetting::instance(),SIGNAL(deviceConnected()),
            this,SLOT(refreshDataRecord()));

    connect(ui->pushButton_refresh,SIGNAL(clicked(bool)),
            SLOT(refreshDataRecord()));

    connect(ui->pushButton_clear,SIGNAL(clicked(bool)),
            SLOT(clearRecord()));

    connect(RSMUsb::instance(),SIGNAL(fileContentReady(QByteArray)),
            this,SLOT(onFileDataReady(QByteArray)));

    connect(RSMUsb::instance(),SIGNAL(needUpdateReadFileProgress(int)),
            this,SLOT(onFileProgressUpdate(int)));

    init();
    this->setWindowTitle(tr("数据下载"));


}

DialogDataDownload::~DialogDataDownload()
{
    ui->linkLayout->removeWidget(linkWidgetTemp);
    linkWidgetTemp->setParent(FormLinkSetting::dialogInstance());
    FormLinkSetting::dialogInstance()->setCentralDialog(linkWidgetTemp);
    qDebug()<<"linkWidgetTemp"<<linkWidgetTemp;
    linkWidgetTemp = 0 ;
    FormLinkSetting::instance()->showLinkTypeAndRTUAddress();
    FormLinkSetting::instance()->setVisible(false);
    FormLinkSetting::dialogInstance()->hide();

    delete ui;
}

void DialogDataDownload::refreshDataRecord()
{
    rowRecordDataMap.clear();

    while(ui->tableWidget_record->rowCount())
        ui->tableWidget_record->removeRow(0);

    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        FormLinkSetting::dialogInstance()->show();

        QStringList aList = RSMUsb::instance()->getAviablePortList();
        if(!aList.isEmpty())
            RSMUsb::instance()->setActivePortName(aList.first());
    }

    if(RSMUsb::instance()->getActivePortName().isEmpty())
    {
        this->setEnabled(false);
        return;
    }

    this->setEnabled(true);

    updateStateAndProgress(tr("刷新"),20);

    //refresh battery volocity
    RSMUsb::instance()->waitForMSecs(100);
    RSMUsb::instance()->refreshUserData();
    RSMUsb::instance()->waitForMSecs(100);

    double v = RSMUsb::instance()
            ->getDeviceMapValue(TopTest::RsmCMD_BatteryVolage).toDouble();
    ui->label_batteryVolgate->setText(QString(" %1 V").arg(v));

    int portNum = RSMUsb::instance()
            ->getDeviceMapValue(TopTest::RsmCMD_Port_Number).toInt();
    qDebug()<<"portNum is"<<portNum;
    setPortNum(portNum);

    updateStateAndProgress(tr("刷新"),30);

    mode = ReadRecord;
    RSMUsb::instance()->readFileRecordListInfo();
}

void DialogDataDownload::downloadRecords()
{

    this->setEnabled(false);
    downloadFileQueueList.clear();

    QList<int> choosePorts = getChoosePortList();
    qDebug()<<"choose ports is"<<choosePorts;

    updateStateAndProgress(tr("下载"),10);

    int wholeLength = 1;

    ///TODO:Change this to signal and slot mode
    for(int i=0; i < ui->tableWidget_record->rowCount() ; i++)
    {

//        updateStateAndProgress(tr("下载"),20 + 60.0 * i / (double) ui->tableWidget_record->rowCount());
//        currentLength = 1;

        QList<QCheckBox*> list =
                ui->tableWidget_record->cellWidget(i,0)->findChildren<QCheckBox*>();
        if(list.isEmpty())
            continue;

        QCheckBox* ckb = list.first();
        if(ckb && ckb->isChecked())
        {
            TopTest::RecordInfoData data = rowRecordDataMap[i];

            downloadFileQueueList.push_back(QPair<TopTest::RecordInfoData,QList<int>>(data,choosePorts));

            wholeLength += data.dataCount* 2 * data.portNum;
//            int currentLength = data.dataCount* 2 * data.portNum;
//            QByteArray fileContent = RSMUsb::instance()->readFileContent(
//                        data.fileName,
//                        currentLength );
//            qDebug()<<"row "<<i<<"file content"<<fileContent ;
//            qDebug()<<"file length"<<fileContent.length() ;

//            fileContent = filterChoosePorts(fileContent, portNum, choosePorts);

//            emit this->fileRecordDownloadSuccess(
//                        "RsmData-" +data.dateTime.toString("yyyyMMdd-hhmmss"),
//                        data.dateTime,
//                        data.sampleTime,
//                        choosePorts.count(),
//                        fileContent);
        }

//        currentLength = 1;
    }

    ui->progressBar->setMaximum(wholeLength + 50);

    foreach (auto pair, downloadFileQueueList) {

        TopTest::RecordInfoData data = pair.first;
        int currentLength = data.dataCount* 2 * data.portNum;
        QByteArray fileContent = RSMUsb::instance()->readFileContent(
                    data.fileName,
                    currentLength );
//        qDebug()<<"row "<<i<<"file content"<<fileContent ;
        qDebug()<<"file length"<<fileContent.length() ;

        fileContent = filterChoosePorts(fileContent, portNum, choosePorts);

        emit this->fileRecordDownloadSuccess(
                    "RsmData-" +data.dateTime.toString("yyyyMMdd-hhmmss"),
                    data.dateTime,
                    data.sampleTime,
                    choosePorts.count(),
                    fileContent);
    }

//    ///滞留主函数于此


//    while(!downloadFileQueueList.isEmpty())
//    {
//        qApp->processEvents();
//        downloadOneFile();
//    }

    this->setEnabled(true);
    downloadFileQueueList.clear();

    updateStateAndProgress(tr("完成"),100);

}

void DialogDataDownload::clearRecord()
{
    RSMUsb::instance()->clearFileRecord();

    refreshDataRecord();
}

void DialogDataDownload::onFileDataReady(const QByteArray &fileContent)
{
    switch (mode) {
    case ReadRecord:
    {
        qDebug()<<"DialogDataDownload::onFileDataReady"<<fileContent;

        QList<TopTest::RecordInfoData> recordList =
            parseRecordString(fileContent);

        updateStateAndProgress(tr("刷新"),60);
        qDebug()<<"record count"<<recordList.count();

        foreach (TopTest::RecordInfoData data, recordList) {
            addRecordInfo(data.dateTime,data.sampleTime,data.portNum,data.dataCount);
            rowRecordDataMap.insert(ui->tableWidget_record->rowCount()-1,
                                    data);
    //        updateStateAndProgress(tr("刷新"),60 + );
        }

        updateStateAndProgress(tr("完成"),100);

        mode = None;
    }
        break;
    case ReadFileContent:
    {
        TopTest::RecordInfoData data = currentFilePair.first;
        QList<int> choosePorts = currentFilePair.second;
        currentFileContent += fileContent;
        qDebug()<<"currentFileContentLength"<<currentFileContent.length()<<"need length"<<data.dataCount* 2 * data.portNum;
        if(currentFileContent.length() == data.dataCount* 2 * data.portNum)
        {

            updateStateAndProgress(tr("下载"),50);

            qDebug()<<"file "<<data.fileName;/*<<"file content"<<currentFileContent ;*/
            qDebug()<<"file length"<<currentFileContent.length() ;

            QByteArray realContent = filterChoosePorts(currentFileContent, portNum, choosePorts);

            emit this->fileRecordDownloadSuccess(
                        "RsmData-" +data.dateTime.toString("yyyyMMdd-hhmmss"),
                        data.dateTime,
                        data.sampleTime,
                        choosePorts.count(),
                        realContent);

            currentFilePair = QPair<TopTest::RecordInfoData,QList<int>>();
            mode = None;
            downloadFileQueueList.takeFirst();
        }
    }
    default:
        break;
    }

}

void DialogDataDownload::downloadOneFile()
{
    TopTest::RecordInfoData data = currentFilePair.first;
    if(mode == ReadFileContent
       || !data.fileName.isEmpty()
       || downloadFileQueueList.isEmpty()
      )
    {
        return;
    }

    qDebug()<<__func__;

    updateStateAndProgress(tr("下载"),30);

    currentFilePair = downloadFileQueueList.first();
    data = currentFilePair.first;

    mode = ReadFileContent;
    RSMUsb::instance()->readFileContent(
                        data.fileName,
                data.dataCount* 2 * data.portNum);
}

void DialogDataDownload::onFileProgressUpdate(const int &addLength)
{
    int value = ui->progressBar->value();
    updateStateAndProgress(tr("下载"),value+addLength);
}

QList<int> DialogDataDownload::getChoosePortList()
{
    QList<int> ret;
    for(int i = 0 ; i < ui->tableWidget_portChoose->columnCount(); i++)
    {
        QList<QCheckBox*> list =
                ui->tableWidget_portChoose->cellWidget(1,i)->findChildren<QCheckBox*>();
        if(list.isEmpty())
            continue;

        QCheckBox* ckb = list.first();
        if(ckb && ckb->isChecked())
        {
            ret.push_back(i);
        }
    }
    return ret;
}

QByteArray DialogDataDownload::filterChoosePorts(QByteArray originData,
                                                 const int &portNum,
                                                 const QList<int> &choosePortList)
{
    qDebug()<<"filterChoosePorts";
    if(choosePortList.count() == portNum)
        return originData;

    QByteArray ret;
    QDataStream rds(&originData, QIODevice::ReadOnly);
    QDataStream wds(&ret, QIODevice::WriteOnly);
    int recordNo = 0;
    while(!rds.atEnd())
    {
        int16_t tempData = 0;
        rds>>tempData;
//        qDebug()<<"recordNo"<<recordNo<<"portNo"<<recordNo%6<<"temp"<<tempData;
        if(choosePortList.contains(recordNo % portNum))
        {
            wds<<tempData;
        }
        recordNo++;
    }

    qDebug()<<originData.length()<<ret.length();
//    qDebug()<<ret.toHex();
    return ret;
}

void DialogDataDownload::resetUnwriteChanges()
{

}

int DialogDataDownload::standerButtons()
{
    return DialogBase::standerButtons() | QDialogButtonBox::Apply;
}

void DialogDataDownload::deleteFirstRecord()
{
    RSMUsb::instance()->removeFileRecordHeadOnes();

    refreshDataRecord();
}

void DialogDataDownload::init()
{
    ui->tableWidget_record->setColumnWidth(0,40);
    ui->tableWidget_record->setColumnWidth(1,40);
    ui->tableWidget_record->setColumnWidth(2,150);
    ui->tableWidget_record->setColumnWidth(4,50);

    updateStateAndProgress("",0);

    for(int i=0;i<ui->tableWidget_portChoose->columnCount();i++)
    {
//        ui->tableWidget_portChoose->setColumnWidth(i,35);
        QCheckBox* cb = addCheckBoxToTable(ui->tableWidget_portChoose,1,i);
        cb->setChecked(true);
        QTableWidgetItem* noItem = new QTableWidgetItem;
        noItem->setText(QString::number(i+1));
        noItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_portChoose->setItem(0,i,noItem);
    }

//    addRecordInfo(QDateTime::currentDateTime(),0.1,200);
//    addRecordInfo(QDateTime::currentDateTime(),1,300);

    refreshDataRecord();
}

void DialogDataDownload::addRecordInfo(const QDateTime &testDateTime, const double &sampleInteralTime, const int &portNum, const int &dataLength)
{
    int row = ui->tableWidget_record->rowCount();
    int column =0;
    ui->tableWidget_record->insertRow(row);

    addCheckBoxToTable(ui->tableWidget_record,row,column++);

    QTableWidgetItem* noItem = new QTableWidgetItem;
    noItem->setText(QString(" %1 ").arg(row+1));
    noItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_record->setItem(row,column++,noItem);

    QTableWidgetItem* testDateTimeItem = new QTableWidgetItem;
    testDateTimeItem->setText(testDateTime.toString("yyyy/MM/dd hh:mm:ss"));
    testDateTimeItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_record->setItem(row,column++,testDateTimeItem);

    QTableWidgetItem* sampleTimeItem = new QTableWidgetItem;
    sampleTimeItem->setText(QString(" %1 s ").arg(sampleInteralTime));
    sampleTimeItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_record->setItem(row,column++,sampleTimeItem);

    QTableWidgetItem* portNumItem = new QTableWidgetItem;
    portNumItem->setText(QString(" %1 ").arg(portNum));
    portNumItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_record->setItem(row,column++,portNumItem);

    QTableWidgetItem* dataLengthItem = new QTableWidgetItem;
    dataLengthItem->setText(QString(" %1 ").arg(dataLength));
    dataLengthItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_record->setItem(row,column++,dataLengthItem);



    if(row == 0)
    {
        QWidget *widget;
        QHBoxLayout *hLayout;
        QPushButton *ckb = new QPushButton(tr("删除"));
        ckb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        hLayout = new QHBoxLayout();
        widget = new QWidget(ui->tableWidget_record);
        hLayout->addWidget(ckb);
        hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
        hLayout->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hLayout);
        ui->tableWidget_record->setCellWidget(row, column, widget);

        connect(ckb,SIGNAL(clicked(bool)),
                this,SLOT(deleteFirstRecord()));
    }
}

int DialogDataDownload::getPortNum() const
{
    return portNum;
}

void DialogDataDownload::setPortNum(int value)
{
    if(value == portNum)
        return;
    portNum = value;

    ui->tableWidget_portChoose->setColumnCount(portNum);

    for(int i=0;i<ui->tableWidget_portChoose->columnCount();i++)
    {
//        ui->tableWidget_portChoose->setColumnWidth(i,35);
        QCheckBox* cb = addCheckBoxToTable(ui->tableWidget_portChoose,1,i);
        cb->setChecked(true);
        QTableWidgetItem* noItem = new QTableWidgetItem;
        noItem->setText(QString::number(i+1));
        noItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_portChoose->setItem(0,i,noItem);
    }

    ui->tableWidget_portChoose->setColumnCount(portNum);
//    int columnWidth = (ui->tableWidget_portChoose->width() - 40) / portNum;
//    for(int i = 0 ; i < ui->tableWidget_portChoose->columnCount() - 1; i++)
//    {
//        ui->tableWidget_portChoose->setColumnWidth(i,columnWidth);
    //    }
}

void DialogDataDownload::updateStateAndProgress(const QString &name, int progress)
{
    ui->label_state->setText(name);
    ui->progressBar->setValue(progress);
}


void DialogDataDownload::onButtonClicked(int role)
{
    switch (role) {
    case QDialogButtonBox::Apply:
    case QDialogButtonBox::Ok:
    {
        downloadRecords();
    }
        break;
    case QDialogButtonBox::Cancel:
    {
        resetUnwriteChanges();
    }
        break;
    default:
        break;
    }
}
