#pragma execution_character_set("utf-8")

#include "formpagedatavisual.h"
#include "ui_formpagedatavisual.h"

#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QMdiSubWindow>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>

#include "rsmcoresignalenum.h"
#include "rsmcoresignalandslotcontrolcenter.h"

#include "dialogdatadownload.h"
#include "dialogsavedatafile.h"
#include "dialogfilesettingchoose.h"
#include "dialogportdefine.h"
#include "formdatapage.h"
#include "formfileinfo.h"
#include "dialogprintoutput.h"
#include "dialogexceldataoutput.h"
#include "dialogopenfile.h"
#include "dialoghandlewave.h"

#include "formdialog.h"

#include "rsmusb.h"
#include "formlinksetting.h"
#include "serialporttransfer.h"
#include <QShortcut>

#include "quiwidget.h"

const qint32 rsmFileMagicNumber = 0xAABBCCDD;

double multiLine(double x)
{
    if(x<50)
        return 40;
    else if(50 <= x && x < 100)
        return 40 + 80.0/50 * ( x - 50);
    else if( 100 <= x && x < 480 )
        return 120 + 100.0/380 * ( x - 100);
    else if(480 <= x && x< 600 )
        return 220 - ( x - 480 );
    else
        return 100;
}

/*QByteArray*/void  testCurveData()
{
    double sampleTime = 1.0;
    double MaxTime = 650;
    double currentTime = 0;
    int portNum = 8;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString fileName = "D:/testFile.rsm";

    QByteArray array;
    QDataStream ds(&array,QIODevice::WriteOnly);
    while(currentTime<=MaxTime)
    {
        for(int i=0; i<portNum; i++)
        {
            ds<<int16_t(multiLine(currentTime)*10);
        }
        currentTime += sampleTime;
    }


    QFile saveFile(fileName);
    if(!saveFile.open(QFile::WriteOnly))
    {
        qDebug()<<"open file error "<<saveFile.fileName();
        return;
    }

    QDataStream fds(&saveFile);
    fds.setVersion(QDataStream::Qt_5_9);
    fds
      <<rsmFileMagicNumber
      <<fileName
      <<dateTime
      <<portNum
      <<sampleTime
      <<(qint32)array.length()
      <<array
        ;
    saveFile.close();

//    return array;
}

FormPageDataVisual::FormPageDataVisual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPageDataVisual)
{
    ui->setupUi(this);
    initToolButtonGroup();
    connect(RSMCoreSignalAndSlotControlCenter::instance(),SIGNAL(needChangeToPage(QWidget*,int)),
            SLOT(onShowDialog(QWidget*,int)));
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    this->setAcceptDrops(true);

    connect(ui->mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this,SLOT(onSubWindowActivated(QMdiSubWindow*)));

    ui->mdiArea->setDocumentMode(true);

    QShortcut* fullScreenSC = new QShortcut(Qt::Key_F11, this);
    QObject::connect(fullScreenSC, SIGNAL(activated()), this, SLOT(onFullScreen()));

    refreshToolButtonState();
}

FormPageDataVisual::~FormPageDataVisual()
{
    delete ui;
}


void FormPageDataVisual::initToolButtonGroup()
{
    ui->listWidget->setMaximumWidth(120);

    ui->listWidget->addGroup(tr("数据操作"));
    ui->listWidget->addItemToCurrentGroup(tr("数据下载"),
                                          ":/images/ic_shujuxiazai.png",
                                          tr("从仪器下载数据"),
                                          Dialog_DataDownload);
    ui->listWidget->addItemToCurrentGroup(tr("通道定义"),
                                          ":/images/ic_tongdaodingyi.png",
                                          tr("定义各通道的显示属性"),
                                          Dialog_DataPortDefine);
//    ui->listWidget->addItemToCurrentGroup(tr("截取分析"),
//                                          ":/images/ic_jiequfenxi.png",
//                                          tr("按条件截取部分数据进行重点分析"),
//                                          Dialog_DataCut);
//    ui->listWidget->addItemToCurrentGroup(tr("对比分析"),
//                                          ":/images/ic_duibifenxi.png",
//                                          tr("比较不同的测试曲线"),
//                                          Dialog_DataCompare);

    ui->listWidget->addGroup(tr("文件操作"));
    ui->listWidget->addItemToCurrentGroup(tr("配置选择"),
                                          ":/images/ic_shezhiwenjianxuanze.png",
                                          tr("选择曲线所需的各项配置文件"),
                                          Dialog_FileOptionChoose);

    ui->listWidget->addItemToCurrentGroup(tr("设置波段"),
                                          ":/images/ic_shezhiwenjianxuanze.png",
                                          tr("手动设置曲线波段"),
                                          Dialog_FileHandleWave);

    ui->listWidget->addItemToCurrentGroup(tr("文件信息"),
                                          ":/images/ic_wenjianxinxi.png",
                                          tr("修改当前曲线文件的信息"),
                                          Dialog_FileInfoSet);
    ui->listWidget->addItemToCurrentGroup(tr("浏览其他"),
                                          ":/images/ic_youlanqita.png",
                                          tr("浏览打开其他曲线文件"),
                                          Dialog_FileOpenOther);
    ui->listWidget->addItemToCurrentGroup(tr("保存文件"),
                                          ":/images/ic_baocunwenjian.png",
                                          tr("保存对当前曲线的修改"),
                                          Dialog_FileSave);
    ui->listWidget->addItemToCurrentGroup(tr("删除文件"),
                                          ":/images/ic_shanchuwenjian.png",
                                          tr("删除当前曲线"),
                                          Dialog_FileDelete);


    ui->listWidget->addGroup(tr("输出报表"));
    ui->listWidget->addItemToCurrentGroup(tr("数据输出"),
                                          ":/images/ic_shujushuchu.png",
                                          tr("将数据输出到剪切板"),
                                          Dialog_OutToData);
    ui->listWidget->addItemToCurrentGroup(tr("打印输出"),
                                          ":/images/ic_dayinshuchu.png",
                                          tr("打印成电子或纸质文件"),
                                          Dialog_OutToFile);

    ui->listWidget->rootWidget = this;
//    ui->stackedWidget->setCurrentIndex(1);
}

void FormPageDataVisual::refreshToolButtonState()
{
//    if(ui->mdiArea->subWindowList().isEmpty())
//    {
//        ui->stackedWidget->setCurrentIndex(1);
//    }
//    else
//    {
//        ui->stackedWidget->setCurrentIndex(0);
//    }
}

QString FormPageDataVisual::formOutputString(FormDataPage* currentPage)
{
    QString str;
    QTextStream ts(&str);
    TopTest::FileData fileData = currentPage->getFileData();
    TopTest::RecordInfoData infoData = fileData.info;
    //1.file info
    ts<<tr("FileInfo:\n");
    ts<<tr("Company:\t %1 \t FileName:\t %2 \t DateTime:\t %3 \n")
        .arg(infoData.Company).arg(infoData.fileName).arg(infoData.dateTime.toString("yyyy/MM/dd hh:mm:ss"));
    ts<<tr("User:\t %1 \t Spy:\t %2 \n")
        .arg(infoData.User).arg(infoData.Spy);
    ts<<tr("Comment:\t %1 \n")
        .arg(infoData.Comment);
    //2.file setting choose
    ts<<tr("Product:\t %1 \t Progress:\t %2 \t Device:\t %3 \n")
        .arg(infoData.Product).arg(infoData.Progress).arg(infoData.Device);
    //3.progress result table in str
    ts<<tr("PortNum:\t %1 \t SampleTime:\t %2 \t DataCount:\t %3 \n")
        .arg(infoData.portNum).arg(infoData.sampleTime).arg(infoData.dataCount);
    ts<<tr("\nAnalysisResult:\n");
    ts<<currentPage->getResultTableContentInStr(false);
    //4.device table
    ts<<tr("\nDevice Information:\n");
    ts<<currentPage->getDeviceTableContentInStr(false);
    //5.source data
    ts<<tr("\nSource Data:\n");
    ts<<currentPage->getSourceDataString();

    return str;
}


void FormPageDataVisual::onShowDialog(QWidget* w,int signal)
{
    if( w != this)
        return;
    qDebug()<<"FormPageDataVisual::onShowDialog"<<signal;

    if( signal < Dialog_DataDownload ||  signal > Dialog_OutToFile )
    {
        qDebug()<<"It's not Data Visual dialog";
        return;
    }

    emit RSMCoreSignalAndSlotControlCenter::instance()->startShowDialog();

    switch (signal) {
    ///Data operation
    case Dialog_DataDownload:
    {

        if(realMonitorOn)
        {
            QMessageBox::critical(this,tr("Error"),tr("Realtime monitor is on,please turn off it first!"));
            break;
        }


        if(RSMUsb::instance()->getBaudRate() == 115200)
        {
            QMessageBox::critical(this,tr("Error"),tr("This is not support under wireless mode!"));
//            FormLinkSetting::dialogInstance()->show();
            break;
        }
//        else if(RSMUsb::instance()->getActivePortName().isNull())
//        {
//            QMessageBox::critical(this,tr("Error"),tr("Please connect device first!"));
////            FormLinkSetting::dialogInstance()->show();
//            break;
//        }

        DialogDataDownload* dialog = new DialogDataDownload(this);
        connect(dialog,SIGNAL(fileRecordDownloadSuccess(QString,QDateTime,double,int,QByteArray)),
                SLOT(createFileSubWindow(QString,QDateTime,double,int,QByteArray)));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("数据下载"));
        formDialog.exec();
    }
        break;
    case Dialog_DataPortDefine:
    {
        FormDataPage* dataPage = getCurrentDataPage();
        if(!dataPage)
            break;
        DialogPortDefine* dialog = new  DialogPortDefine(this);
        dialog->setCurrentPortInfo(dataPage->getPortInfoMap());
        connect(dialog,SIGNAL(portInfoChanged(QMap<int,QPair<QString,QColor> >)),
                dataPage,SLOT(setPortInfos(QMap<int,QPair<QString,QColor> >)));

        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("通道定义"));
        formDialog.exec();
    }
        break;
//    case Dialog_DataCut:
//    {
//        DialogDataDownload dialog(this);
//        dialog.exec();
//    }
//        break;
//    case Dialog_DataCompare:
//    {
//        DialogDataDownload dialog(this);
//        dialog.exec();
//    }
//        break;

    ///File operation
    case Dialog_FileOptionChoose:
    {

        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;
        DialogFileSettingChoose* dialog = new  DialogFileSettingChoose(this);
        dialog->setVelocity(page->getVelocity());
        dialog->setCurrentDevice(page->getDeviceName());
        dialog->setCurrentProcess(page->getProcessName());
        dialog->setCurrentProduct(page->getProductName());
        //dialog->;
        connect(dialog,SIGNAL(velocityChanged(double)),
                page,SLOT(setVelocity(double)));
        connect(dialog,SIGNAL(progressChooseChanged(QString)),
                page,SLOT(applyProcess(QString)));
        connect(dialog,SIGNAL(deviceChooseChanged(QString)),
                page,SLOT(applyDevice(QString)));
        connect(dialog,SIGNAL(productChooseChanged(QString)),
                page,SLOT(applyProduct(QString)));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("设置文件选择"));
        formDialog.exec();
    }
        break;

    case Dialog_FileHandleWave:
    {
        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;
        DialogHandleWave* dialog = new  DialogHandleWave(this);
        dialog->setWaveInfo(page->getWaveInfo());
//        dialog->setVelocity(page->getVelocity());
//        dialog->setCurrentDevice(page->getDeviceName());
//        dialog->setCurrentProcess(page->getProcessName());
//        dialog->setCurrentProduct(page->getProductName());
        //dialog->;
//        connect(dialog,SIGNAL(velocityChanged(double)),
//                page,SLOT(setVelocity(double)));
//        connect(dialog,SIGNAL(progressChooseChanged(QString)),
//                page,SLOT(applyProcess(QString)));
//        connect(dialog,SIGNAL(deviceChooseChanged(QString)),
//                page,SLOT(applyDevice(QString)));
//        connect(dialog,SIGNAL(productChooseChanged(QString)),
//                page,SLOT(applyProduct(QString)));
        connect(dialog,SIGNAL(waveInfoChanged(TopTest::WaveInfo)),
                page,SLOT(setWaveInfo(TopTest::WaveInfo)));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("手动设置波段"));
        formDialog.exec();
    }
        break;

    case Dialog_FileInfoSet:
    {

        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;
        FormFileInfo *dialog = new FormFileInfo(this);
        dialog->fillCurrentFileInfo(page->getFileData().info.toVariantMap());
        connect(dialog,SIGNAL(fileInfoChanged(QVariantMap)),
                this,SLOT(onCurrentFileInfoChanged(QVariantMap)));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("文件信息"));
        formDialog.exec();
    }
        break;
    case Dialog_FileOpenOther:
    {
//        QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"),
//                                     "D:/","RSM files (*.rsm);;");
//        if(!filePath.isEmpty())
//        {
//            onOpenDataFile(filePath);
//        }
        DialogOpenFile* dialog = new DialogOpenFile(this);

        connect(dialog,SIGNAL(needToOpenFile(QString)),
                this,SLOT(onOpenDataFile(QString)));

        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("打开文件"));
        formDialog.exec();
    }
        break;
    case Dialog_FileSave:
    {
        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;

        DialogSaveDataFile* dialog = new DialogSaveDataFile(this);
        dialog->setFileInfo(& page->getFileData().info);
//        dialog->setCurrentFileName(page->getFileData().info.fileName);
//        dialog->setCurrentProductName(page->getFileData().info.Product);

        connect(dialog,SIGNAL(saveCurrentFileToPath(QString)),
                this,SLOT(onSaveFileToPath(QString)));

        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("数据保存"));
        formDialog.exec();
    }
        break;
    case Dialog_FileDelete:
    {
        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;

        //delete current file
        if(QMessageBox::Yes == QMessageBox::question(
                    this,tr("删除文件"),tr("是否删除当前曲线数据文件！")))
            {
                deleteCurrentFile();
            }
    }
        break;

    ///out operation
    case Dialog_OutToData:
    {
        FormDataPage* page = getCurrentDataPage();
        if(!page)
            break;
        DialogExcelDataOutput *dialog = new DialogExcelDataOutput(this);
        dialog->setOutputStr(formOutputString(page));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("数据输出"));
        formDialog.exec();
    }
        break;
    case Dialog_OutToFile:
    {

        FormDataPage* page = getCurrentDataPage();
        qDebug()<<"try to print data"<<page;
        if(!page)
            break;
        qDebug()<<"show DialogPrintOutput";
        DialogPrintOutput *dialog = new DialogPrintOutput(this);
        connect(dialog,SIGNAL(needToPrintOutput(int,int)),
                page,SLOT(printOutput(int,int)));
        FormDialog formDialog(dialog,this);
        formDialog.setWindowTitle(tr("打印输出"));
        formDialog.exec();
    }
        break;

    default:
        break;
    }


    emit RSMCoreSignalAndSlotControlCenter::instance()->endShowDialog();
}

void FormPageDataVisual::createFileSubWindow(const QString &fileName,
                                             const QDateTime &recordTime,
                                             const double &sampleTime,
                                             const int &portNum,
                                             const QByteArray &recordContent)
{
    qDebug()<<"go to create sub window";
    FormDataPage* dataPage = new FormDataPage;
    TopTest::RecordInfoData infoData;
    infoData.fileName = fileName;
    infoData.dateTime = recordTime;
    infoData.portNum = portNum;
    infoData.sampleTime = sampleTime;

    TopTest::FileData fileData;
    fileData.info = infoData;
    fileData.pointData = recordContent;
//    dataPage->setWindowTitle(fileName);

    dataPage->setFileName(fileName);
    dataPage->setTestDateTime(recordTime);
    dataPage->setPortNum(portNum);
    dataPage->setSampleTime(sampleTime);

    dataPage->setFileData(fileData);
    dataPage->addCurveData(recordContent);

    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(dataPage);
    subWindow->showMaximized();

//    refreshToolButtonState();
}

void FormPageDataVisual::createFileSubWindow(const QString& filePath ,
                                             const TopTest::RecordInfoData &infoData,
                                             const QByteArray &recordContent,
                                             const QList<QMap<QString, QVariant> >& settingFileMapList,
                                             const TopTest::WaveInfo& waveInfo)
{
    qDebug()<<"createFileSubWindow";
    FormDataPage* dataPage = new FormDataPage;

    TopTest::FileData fileData;
    fileData.info = infoData;
    fileData.pointData = recordContent;
//    dataPage->setWindowTitle(fileName);

    dataPage->setFileName(infoData.fileName);
    dataPage->setTestDateTime(infoData.dateTime);
    dataPage->setPortNum(infoData.portNum);
    dataPage->setSampleTime(infoData.sampleTime);

    QString productLabel = infoData.Product;
    if(!infoData.Product.isEmpty() && !productLabel.startsWith("BuiltIn:"))
    {
        productLabel = "BuiltIn:"+ productLabel;
    }
    dataPage->setProductName(productLabel);

    QString processLabel = infoData.Progress;
    if(!infoData.Progress.isEmpty() && !processLabel.startsWith("BuiltIn:"))
        processLabel = "BuiltIn:"+ processLabel;
    dataPage->setProcessName(processLabel);

    QString deviceLabel = infoData.Device;
    if(!infoData.Device.isEmpty() && !deviceLabel.startsWith("BuiltIn:"))
        deviceLabel = "BuiltIn:"+ deviceLabel;
    dataPage->setDeviceName(deviceLabel);

    dataPage->setSettingFileMapList(settingFileMapList);

    dataPage->setFileData(fileData);
    dataPage->addCurveData(recordContent);

    dataPage->setFilePath(filePath);
    dataPage->setWaveInfo(waveInfo);

    qDebug()<<"go to add sub window";
    QMdiSubWindow* subWindow = ui->mdiArea->addSubWindow(dataPage);
    subWindow->showMaximized();

//    refreshToolButtonState();
}



void FormPageDataVisual::onSaveFileToPath(const QString &path)
{
    QMdiSubWindow* subWindow = ui->mdiArea->currentSubWindow();
    FormDataPage* page = qobject_cast<FormDataPage*>(subWindow->widget());
    if(!page)
        return;

    bool success = saveDataFile(path,page->getFileData(),page->getSettingFileMapList(),page->getWaveInfo());
    if(success)
    {
        page->getFileData().info.saveFilePath = path;
        page->setFileName(QFileInfo(path).completeBaseName());
    }

    //    windowFileMap
}


//QDataStream& operator <<(QDataStream& ds, const QVariant& value)
//{
//    return ds;
//}

bool FormPageDataVisual::saveDataFile(const QString &path,
        const TopTest::FileData& fileData,
        const QList<QMap<QString, QVariant> > &settingFileMapList,
        const TopTest::WaveInfo& waveInfo)
{
    qDebug()<<"begin save data file"<<path;

    TopTest::RecordInfoData infoData = fileData.info;
    QFile saveFile(path);
    if(!saveFile.open(QFile::WriteOnly))
    {

        qDebug()<<"open file error "<<saveFile.fileName();
        QMessageBox::critical(0,tr("保存文件出错"),tr("无法打开文件 %1 ！").arg(path));
        return false;
    }

    qDebug()<<"Device Product Process"
           <<infoData.Device
           <<infoData.Product
           <<infoData.Progress;


    QDataStream ds(&saveFile);
    ds.setVersion(QDataStream::Qt_5_9);
    ds
      <<rsmFileMagicNumber
      <<infoData.fileName
      <<infoData.Comment
      <<infoData.Company
      <<infoData.Spy
      <<infoData.User
      <<infoData.Line
      <<infoData.dateTime

      //data related

      <<infoData.portNum
      <<infoData.sampleTime
      <<infoData.LineVelocity

      <<infoData.Product
      <<infoData.Device
      <<infoData.Progress

      //product content
      //product content
      //product content
      <<settingFileMapList.at(0)
      <<settingFileMapList.at(1)
      <<settingFileMapList.at(2)

      //handle wave
      <<waveInfo.useHandle
      <<waveInfo.waveCount
      <<waveInfo.waveNameMap.value(1)
      <<waveInfo.waveNameMap.value(2)
      <<waveInfo.waveNameMap.value(3)
      <<waveInfo.waveTimeSection.value(1)
      <<waveInfo.waveTimeSection.value(2)
      <<waveInfo.waveTimeSection.value(3)



      <<(qint32)fileData.pointData.length()
      <<fileData.pointData
        ;

    saveFile.close();

    QMessageBox::information(0,tr("保存成功"),tr("文件已经成功保存到：%1").arg(path));
    return true;
}

void FormPageDataVisual::onOpenDataFile(const QString &filePath)
{
    qDebug()<<"onOpenDataFile"<<filePath;
    foreach (QMdiSubWindow* window, ui->mdiArea->subWindowList()) {
        FormDataPage* dataPage = dynamic_cast<FormDataPage*> (window->widget());
        qDebug()<<"current has opend"<<dataPage->getFilePath();
        if(dataPage && dataPage->getFilePath() == filePath)
        {
            ui->mdiArea->setActiveSubWindow(window);
            return;
        }
    }



    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<"open file error "<<file.fileName();
        return;
    }

    QList<QMap<QString, QVariant> > settingFileMapList;

    QMap<QString, QVariant> ProductMap;
    QMap<QString, QVariant> DeviceMap;
    QMap<QString, QVariant> ProgressMap;

    TopTest::WaveInfo waveInfo;

    qint32 rsmFileMagicNumber = 0;

    TopTest::RecordInfoData infoData;
    qint32 baLength = 0 ;
    QByteArray dataArray;

    QDataStream ds(&file);
    ds.setVersion(QDataStream::Qt_5_9);
    ds
      >>rsmFileMagicNumber
      >>infoData.fileName
      >>infoData.Comment
      >>infoData.Company
      >>infoData.Spy
      >>infoData.User
      >>infoData.Line
      >>infoData.dateTime
      >>infoData.portNum
      >>infoData.sampleTime
      >>infoData.LineVelocity

      >>infoData.Product
      >>infoData.Device
      >>infoData.Progress

      >>ProductMap
      >>DeviceMap
      >>ProgressMap

      >>waveInfo.useHandle
      >>waveInfo.waveCount
      >>waveInfo.waveNameMap[1]
      >>waveInfo.waveNameMap[2]
      >>waveInfo.waveNameMap[3]
      >>waveInfo.waveTimeSection[1]
      >>waveInfo.waveTimeSection[2]
      >>waveInfo.waveTimeSection[3]

      >>baLength
      >>dataArray
            ;

    infoData.saveFilePath = filePath;

    qDebug()<<"rsmFileMagicNumber:"<<rsmFileMagicNumber;

    qDebug()<<"infoData.fileName"<<infoData.fileName;
    qDebug()<<"infoData.Commment"<<infoData.Comment;
    qDebug()<<"infoData.Spy"<<infoData.Spy;
    qDebug()<<"infoData.User"<<infoData.User;

    qDebug()<<"infoData.dateTime"<<infoData.dateTime;

    qDebug()<<"infoData.portNum"<<infoData.portNum;

    qDebug()<<"infoData.sampleTime"<<infoData.sampleTime;

    qDebug()<<"dataArray length is:"<<baLength;

    qDebug()<<"dataArray is:"<<dataArray;

    qDebug()<<"ProductMap is:"<<ProductMap;
    qDebug()<<"DeviceMap is:"<<DeviceMap;
    qDebug()<<"ProgressMap is:"<<ProgressMap;

    file.close();

    settingFileMapList<<ProductMap<<DeviceMap<<ProgressMap;

    createFileSubWindow(filePath,infoData,
                        dataArray,settingFileMapList,waveInfo);

}

void FormPageDataVisual::deleteCurrentFile()
{
    ui->mdiArea->currentSubWindow()->close();

    refreshToolButtonState();
}

FormDataPage *FormPageDataVisual::getCurrentDataPage()
{
    QMdiSubWindow* window =  ui->mdiArea->currentSubWindow();
    if(window)
        return dynamic_cast<FormDataPage*>(window->widget());
    else
        return 0;
}

void FormPageDataVisual::onSubWindowActivated(QMdiSubWindow *window)
{
    if(!window)
        return;

    foreach (QMdiSubWindow* subWindow, ui->mdiArea->subWindowList()) {
        FormDataPage* dataPage = dynamic_cast<FormDataPage*>(subWindow->widget());
        if(dataPage)
        {
            dataPage->hide();
        }
    }

    FormDataPage* dataPage = dynamic_cast<FormDataPage*>(window->widget());
    if(dataPage)
    {
        dataPage->show();
    }

    refreshToolButtonState();
}

void FormPageDataVisual::onCurrentFileInfoChanged(const QVariantMap &currentMap)
{
    FormDataPage* dataPage = getCurrentDataPage();
    if(dataPage)
    {
        dataPage->getFileData().info.loadMap(currentMap);
        dataPage->setWindowTitle(currentMap.value("FileName").toString());
        dataPage->setTestDateTime(currentMap.value("DateTime").toDateTime());
    }
}

void FormPageDataVisual::onFullScreen()
{
    qDebug()<<"FormPageDataVisual::onFullScreen()";
    if (this->FullScreenWindow)
    {
      this->FullScreenWindow->layout()->removeWidget(ui->mdiArea);
      QGridLayout* gl = dynamic_cast<QGridLayout*> (this->layout());
      gl->addWidget(ui->mdiArea,0,1);
      delete FullScreenWindow;
      FullScreenWindow = 0;
    }
    else
    {
      QWidget* fullScreenWindow = new QWidget(this, Qt::Window);
      this->FullScreenWindow = fullScreenWindow;
      fullScreenWindow->setObjectName("FullScreenWindow");
      qDebug()<<"The layout is "<<this->layout();
//      ui->splitter->layout()->removeWidget(ui->stackedWidget_View);

      QVBoxLayout* vbox = new QVBoxLayout(fullScreenWindow);
      vbox->setSpacing(0);
      vbox->setMargin(0);

      vbox->addWidget(ui->mdiArea);
      fullScreenWindow->showFullScreen();
      fullScreenWindow->show();

      QShortcut* esc = new QShortcut(Qt::Key_Escape, fullScreenWindow);
      QObject::connect(esc, SIGNAL(activated()), this, SLOT(onFullScreen()));
      QShortcut* f11 = new QShortcut(Qt::Key_F11, fullScreenWindow);
      QObject::connect(f11, SIGNAL(activated()), this, SLOT(onFullScreen()));
    }
}

void FormPageDataVisual::onMonitorSwitchChanged(bool on)
{
    realMonitorOn = on;
}


//void FormPageDataVisual::applyProcess(const QString &processName)
//{

//}

void FormPageDataVisual::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
            event->acceptProposedAction();
    }
}

inline int16_t tempToDigtalValue(const QString& dataStr)
{
    double value = dataStr.toDouble();
    return value * 10;
}

void FormPageDataVisual::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream ts(&file);
//    QList<QStringList> lineDatasList;
    QByteArray dataArray;
    QDataStream ds(&dataArray,QIODevice::WriteOnly);

    QStringList lineDatas =
            ts.readLine().split(",",QString::SkipEmptyParts);
    int portCount = lineDatas.count() - 2;
    int firstPortCount = portCount;

    if(portCount <= 0)
        return;

    QDateTime recordDateTime;
    recordDateTime.setDate(QDate::fromString(lineDatas.at(0),"yyyy-M-d"));
    recordDateTime.setTime(QTime::fromString(lineDatas.at(1),"hh:mm:ss"));
    qDebug()<<lineDatas<<recordDateTime;
    if(!recordDateTime.isValid())
    {
        recordDateTime = QDateTime::currentDateTime();
        if(QMessageBox::No ==  QMessageBox::question(this,tr("Warning!"),tr("Can't parser data time in this data file,"
                                                     "use current date time instead")))
        {
            return;
        }
    }

    qDebug()<<lineDatas;
//    lineDatasList<<lineDatas;
    for(int i =2; i<lineDatas.count();i++)
        ds<<tempToDigtalValue(lineDatas[i]);

    while(!ts.atEnd() && portCount > 0 && portCount == firstPortCount)
    {
        lineDatas =
                ts.readLine().split(",",QString::SkipEmptyParts);
        portCount = lineDatas.count() - 2;
        if(portCount == firstPortCount)
        {
            qDebug()<<lineDatas;
//            lineDatasList<<lineDatas;
            for(int i =2; i<lineDatas.count();i++)
                ds<<tempToDigtalValue(lineDatas[i]);
        }
    }

    createFileSubWindow(QFileInfo(fileName).fileName(),
                        recordDateTime,
                        1,
                        firstPortCount,
                        dataArray);


}
