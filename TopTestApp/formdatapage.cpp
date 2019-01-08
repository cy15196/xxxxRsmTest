#pragma execution_character_set("utf-8")

#include "formdatapage.h"
#include "ui_formdatapage.h"

#include <QLabel>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QPrinter>
#include <QPainter>
#include <QPixmap>

#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QFontMetrics>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfWriter>

#include "dialogprintoutput.h"

#include "formfilesettingbasewidget.h"

FormDataPage::FormDataPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDataPage)
{
    ui->setupUi(this);
    connect(ui->widget_chart,SIGNAL(needToReapply()),
            this,SLOT(reapply()));

    ui->widget_chart->setDrawWave(true);
//    ui->widget_chart->hide();
//    init();
}

FormDataPage::~FormDataPage()
{
    delete ui;
}

void FormDataPage::setFileName(const QString &name)
{
    this->setWindowTitle(name);
    ui->widget_chart->setFileName(name);
    this->fileData.info.fileName = name;
}

void FormDataPage::setDeviceName(const QString &name)
{
    ui->widget_chart->setDeviceName(name);
    this->fileData.info.Line = this->getDeviceName()+"-"+this->getProcessName();
}

void FormDataPage::setProcessName(const QString &name)
{
    ui->widget_chart->setProcessName(name);
    this->fileData.info.Line = this->getDeviceName()+"-"+this->getProcessName();
}

void FormDataPage::setPortNum(const int &portNum)
{
    ui->widget_chart->setPortNum(portNum);
}

void FormDataPage::setTestDateTime(const QDateTime &datetime)
{
    ui->widget_chart->setTestDateTime(datetime);
}

void FormDataPage::setSampleTime(const double &sampleTime)
{
    ui->widget_chart->setSampleTime(sampleTime);
}

void FormDataPage::addCurveData(const QByteArray &dataArray)
{
    if(dataArray.isEmpty())
        return;

    qDebug()<<"current process"<<fileData.info.Progress;
    qDebug()<<"current device"<<fileData.info.Device;

    ui->widget_chart->setCurveData(dataArray);
    fileData.pointData = dataArray;

    if(progressMap.isEmpty())
    {
//        QLabel* label = ui->widget_chart->getSettingTypeToEditMap()["Progress"];
//        applyProcess(label->text().split(":",QString::SkipEmptyParts).at(1));
        if(!fileData.info.Progress.isEmpty())
            applyProcess(fileData.info.Progress);
        else
        {
            fileData.info.Progress = FileSettingWidget::getLastEditRecordName("Progress");
            applyProcess(fileData.info.Progress);
        }

    }
    else
        applyProcess(progressMap);

    if(deviceMap.isEmpty())
    {
//        QLabel* devcieLabel = ui->widget_chart->getSettingTypeToEditMap()["Device"];
//        applyDevice(devcieLabel->text().split(":",QString::SkipEmptyParts).at(1));
        if(!fileData.info.Device.isEmpty())
            applyDevice(fileData.info.Device);
        else
        {
            fileData.info.Device = FileSettingWidget::getLastEditRecordName("Device");
            applyDevice(fileData.info.Device);
        }
    }
    else
        applyDevice(deviceMap);
}

void FormDataPage::applyProcess(const QString &processName)
{
    if(processName.startsWith("BuiltIn:"))
    {
        applyProcess(progressMap);
        this->setProcessName(processName);
        return;
    }

    qDebug()<<"FormDataPage::apply process"<<processName;
    while(ui->tabWidget->count() > 1)
    {
        QWidget* tabWidget = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete tabWidget;
    }

    resultTableList.clear();

    auto result = ui->widget_chart->applyProgress(processName);
    qDebug()<<"FormDataPage::fill port"<<processName;
    foreach (int waveNo, result.keys()) {
        FormResultTable* resultTable = new  FormResultTable(this);
        resultTable->setPortInfo(ui->widget_chart->getPortInfoMap());
        resultTable->fillResult(result.value(waveNo));
        resultTableList<<resultTable;
        QString tabTitle = tr("波段")+QString::number(waveNo);
        resultTable->setWindowTitle(tabTitle);
        ui->tabWidget->insertTab(waveNo-1,resultTable,tabTitle);
        ///处理图表显示时间单位的变化
        connect(ui->widget_chart,SIGNAL(timeTypeChanged(int)),
                resultTable,SLOT(onTimeTypeChanged(int)));
        connect(ui->widget_chart,SIGNAL(tempTypeChanged(int)),
                resultTable,SLOT(onTempTypeChanged(int)));
//        ui->widget_resultTable->setPortInfo(ui->widget_chart->getPortInfoMap());
//        ui->widget_resultTable->fillResult(result);
    }

    this->setProcessName(processName);

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");

    appSetting.beginGroup("Progress");
    QMap<QString,QVariant> ProgressMap =  appSetting.value(processName).toMap();
    appSetting.endGroup();

    appSetting.endGroup();
    progressMap = ProgressMap/*.value("widget_DataTable").toMap()*/;
}

void FormDataPage::applyDevice(const QString &deviceName)
{
    if(deviceName.startsWith("BuiltIn:"))
    {
        applyDevice(deviceMap);
        this->setDeviceName(deviceName);
        return;
    }

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup("Device");
    QMap<QString,QVariant> DeviceMap =  appSetting.value(deviceName).toMap();
    appSetting.endGroup();
    appSetting.endGroup();

    applyDevice(DeviceMap);
    this->setDeviceName(deviceName);
}

void FormDataPage::applyProduct(const QString &productName)
{
    if(productName.startsWith("BuiltIn:"))
    {
        applyProduct(progressMap);
        this->setProductName(productName);
        return;
    }

    this->setProductName(productName);

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");

    appSetting.beginGroup("Product");
    QMap<QString,QVariant> ProductMap =  appSetting.value(productName).toMap();
    appSetting.endGroup();

    appSetting.endGroup();
    productMap = ProductMap;
}

void FormDataPage::applyProcess(const QMap<QString, QVariant> &processMap)
{
    qDebug()<<"got to apply builtIn process"<<processMap;
    while(ui->tabWidget->count() > 1)
    {
        QWidget* tabWidget = ui->tabWidget->widget(0);
        ui->tabWidget->removeTab(0);
        delete tabWidget;
    }

    resultTableList.clear();

    auto result = ui->widget_chart->applyProgress(processMap);
    foreach (int waveNo, result.keys()) {
        FormResultTable* resultTable = new  FormResultTable(this);
        resultTable->setPortInfo(ui->widget_chart->getPortInfoMap());
        resultTable->fillResult(result.value(waveNo));
        resultTableList<<resultTable;
        QString tabTitle = tr("波段")+QString::number(waveNo);
        resultTable->setWindowTitle(tabTitle);
        ui->tabWidget->insertTab(waveNo-1,resultTable,tabTitle);

        ///处理图表显示时间单位的变化
        connect(ui->widget_chart,SIGNAL(timeTypeChanged(int)),
                resultTable,SLOT(onTimeTypeChanged(int)));
        connect(ui->widget_chart,SIGNAL(tempTypeChanged(int)),
                resultTable,SLOT(onTempTypeChanged(int)));
//        ui->widget_resultTable->setPortInfo(ui->widget_chart->getPortInfoMap());
//        ui->widget_resultTable->fillResult(result);
    }
}

void FormDataPage::applyDevice(const QMap<QString, QVariant> &deviceMap)
{
    qDebug()<<"got to apply builtIn Device"<<deviceMap;
    ui->widget_deviceInfo->applyDevice(deviceMap);
    ui->widget_chart->drawDeviceTempSection(deviceMap);
    this->deviceMap = deviceMap;

    this->setVelocity(deviceMap.value("doubleSpinBox_velocity").toDouble());

}

void FormDataPage::applyProduct(const QMap<QString, QVariant> &productMap)
{

}

QMap<int, QPair<QString, QColor> > FormDataPage::getPortInfoMap()
{
    return ui->widget_chart->getPortInfoMap();
}

void FormDataPage::setPortInfos(const QMap<int, QPair<QString, QColor> > &portInfoMap)
{
    qDebug()<<"FormDataPage::setPortInfos";
    ui->widget_chart->setPortInfos(portInfoMap);
    ui->widget_resultTable->setPortInfo(portInfoMap);
//    ui->widget_deviceInfo->setPortInfo(portInfoMap);
}

#define SmallGap 20
#define MediumGap 30
#define LargeGap 50




inline void printChart(QPainter *painter,FormDataChart* chartPage)
{
    QPixmap chartMap = chartPage->formChartPixmap();
    painter->drawPixmap(SmallGap,0,chartMap);

    QPixmap legendMap = chartPage->formLengendPixmap();
    painter->drawPixmap( 1000 , LargeGap,legendMap);
//    painter->drawPixmap(painter->window().width() -legendMap.width() ,
//                        -chartMap.height(),legendMap);

    painter->translate( 0,chartMap.height() + SmallGap);
}



inline void printBox(QPainter *painter, const QString &str,
                     const QFont &font, const QBrush &brush,
                     bool alignmentCenter = false,
                     bool drawBox = false)
{
    painter->save();
    painter->setFont(font);
    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * SmallGap;
    int maxHeight = painter->window().height();
    QRect textRect;
    if(alignmentCenter)
    {

       textRect = painter->boundingRect(SmallGap + textWidth/2 - painter->fontMetrics().width(str)/2, SmallGap, textWidth/2, maxHeight,
                              Qt::TextWordWrap, str);
       qDebug()<<str<<"width is"<<painter->fontMetrics().width(str)<<"rect is"<<textRect;
    }
    else
    {
       textRect = painter->boundingRect(SmallGap, SmallGap,textWidth, maxHeight,
                              Qt::TextWordWrap, str);
    }
    int boxHeight = textRect.height() + 2 * SmallGap;
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(brush);
    if(drawBox)
        painter->drawRect(0, 0, boxWidth, boxHeight);
    painter->drawText(textRect, Qt::TextWordWrap, str);
    painter->translate(0, boxHeight);
}

inline void printMultiText(QPainter *painter,const QString& text)
{
#define lineCount 100
    QStringList lineTextList = text.split("\n");
    foreach (QString lineText, lineTextList) {
        int start = 0;
        while(start < lineText.length())
        {
            printBox(painter,lineText.mid(start,lineCount),QFont(),Qt::white);
            start += lineCount;
        }
    }
}

inline void printTester(QPainter *painter,TopTest::RecordInfoData info)
{
    painter->save();
    painter->translate(0, LargeGap);

    QFont font;
//    font.setPointSize(16);
    printBox(painter,
             QCoreApplication::translate("FormDataPage","Tester:\t") + info.User
             +"                      "+ QCoreApplication::translate("FormDataPage","Assessor:\t") + info.Spy,
             font,
             Qt::white);

}

///print table box use simple text and line method
void printTableValue(QPainter* painter,QTableWidget* table)
{
    int columnCount = table->columnCount();
    int startColumn = 2;

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int  originHeight = table->height();
    int shotHeight = table->rowHeight(0) * table->rowCount() + 2 ;
    table->setMinimumHeight(shotHeight);


#define maxColumn 8

    while(columnCount - 2 > 0)
    {
        ///hide all data count
        for(int j= 2 ; j < table->columnCount(); j++)
        {
            table->setColumnWidth(j,0);
        }
        ///show current 8 col
        for(int j= startColumn ; j < startColumn + maxColumn; j++)
        {
            table->setColumnWidth(j,120);
        }

        int shotWidth =  table->columnWidth(0)
                        + table->columnWidth(1)
                        + 120 * maxColumn + 2;

        table->setMinimumWidth(shotWidth);

        QPixmap nowPixmap =table->grab(QRect(QPoint(0,0),QSize(shotWidth,shotHeight)));
        painter->drawPixmap(SmallGap,0,nowPixmap);
        painter->translate(0,shotHeight + 10);

//        painter->drawPixmap();

        columnCount -= maxColumn;
        startColumn += maxColumn;
    }

    table->setMinimumHeight(originHeight);

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

}

void printFileInfo(QPainter *painter,TopTest::RecordInfoData info)
{
    painter->save();
    painter->translate(0, LargeGap);

    QFont font;
    printBox(painter, QCoreApplication::translate("FormDataPage","Company:\t")+ info.Company, font, Qt::white);
    printBox(painter, QCoreApplication::translate("FormDataPage","FileName:\t")+ info.fileName, font, Qt::white);
    printBox(painter, QCoreApplication::translate("FormDataPage","Line:\t")+ info.Line, font, Qt::white);
    printBox(painter, QCoreApplication::translate("FormDataPage","TestTime:\t")+ info.dateTime.toString("hh:mm:ss yyyy/MM/dd"), font, Qt::white);

//    printBox(painter, QCoreApplication::translate("FormDataPage","Comment:\t")+ info.Comment, font, Qt::white);

    painter->translate(0, LargeGap);
}


//void printPage(QPainter *painter,
//               const QStringList &entries, int pageNumber)
//{
//    painter->save();
//    painter->translate(0, LargeGap);
//    foreach (QString entry, entries) {
//        QStringList fields = entry.split(": ");
//        QString title = fields[0];
//        QString body = fields[1];
//        printBox(painter, title, titleFont, Qt::lightGray);
//        printBox(painter, body, bodyFont, Qt::white);
//        painter->translate(0, MediumGap);
//    }
//    painter->restore();
//    painter->setFont(footerFont);
//    painter->drawText(painter->window(),
//                      Qt::AlignHCenter | Qt::AlignBottom,
//                      QString::number(pageNumber));
//}

inline void testIfNeedNewPage(QPainter *pdf_painter, QPdfWriter *pdf_writer,double nextItemHeight = 2000)
{
    if(pdf_painter->transform().dy() + nextItemHeight >= pdf_painter->viewport().height() - 10 * LargeGap)
    {
        pdf_writer->newPage();
        pdf_painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
        pdf_painter->setBrush(Qt::white);

        pdf_painter->resetTransform();

        pdf_painter->drawRect(0, 0,
                                pdf_painter->window().width(),
                                pdf_painter->window().height() - 10 * LargeGap);
//        pdf_painter->transform().
    }
}

void FormDataPage::printOutput(int type,int printContent)
{

    QString pdfFileName = QFileDialog::getSaveFileName(this,tr("Choose pdf save path and name"),
                                 "D:/"+this->getFileData().info.fileName+".pdf","PDF (*.pdf)");
    if(pdfFileName.isEmpty())
        return;

    QFile pdf_file(pdfFileName);
    if(!pdf_file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,tr("错误"),tr("打印失败：创建PDF文件 %1 失败 ").arg(pdfFileName));
        return;
    }
    QPdfWriter *pdf_writer = new QPdfWriter(&pdf_file);
    QPainter *pdf_painter= new QPainter(pdf_writer);
    pdf_writer->setResolution(1200);
//    QPagedPaintDevice::Margins ms;
//    ms.bottom = 30;
//    ms.top = 30;
//    ms.left = 30;
//    ms.right = 30;
//    pdf_writer->setMargins(ms);
    pdf_writer->setPageSize(QPagedPaintDevice::A4);


//    QPrinter printer(QPrinter::HighResolution);
//    printer.setPageSize(QPrinter::A4);  //设置纸张大小为A4
//    printer.setOutputFormat(QPrinter::PdfFormat);  //设置输出格式为pdf
//    printer.setOutputFileName(pdfFileName);   //设置输出路径

    QPixmap chartPixmap = ui->widget_chart->grab();
//    QList<QPair<QString,QPixmap> > resultPicList;
//    foreach (FormResultTable* rt, resultTableList) {
//        resultPicList<< QPair<QString,QPixmap>(rt->windowTitle(),rt->grab()) ;
//    }

    QPixmap devicePixmap = ui->widget_deviceInfo->grab(
                QRect(QPoint(0,0),
                      QSize(ui->widget_deviceInfo->width(),
                      ui->widget_deviceInfo->getTableWidget()->rowCount() *
                      ui->widget_deviceInfo->getTableWidget()->rowHeight(0)+2)
                      )
                );

//    switch (type) {
//    case DialogPrintOutput::SaveToPic:
//    {

//    }
//        break;
//    case DialogPrintOutput::SaveToPDF:
//    {

//    }
//    case DialogPrintOutput::PrintToFile:
//    {

//    }
//    default:
//        break;
//    }

    //init QPainter
//    QPainter painter_pixmap;    // 使用QPainter在绘图设备上绘制
//    pdf_painter->begin(&printer);
    QRect rect = pdf_painter->viewport();

    //print header
    QFont titleFont;
    titleFont.setPointSize(20);
    QFont tabFont;
    tabFont.setPointSize(15);

    pdf_painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    pdf_painter->setBrush(Qt::white);
    pdf_painter->drawRect(0, 0,
                            pdf_painter->window().width(),
                            pdf_painter->window().height() - 10 * LargeGap);

    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","RSM Temperature Test Report"),
             titleFont , Qt::white ,true ,false);
    pdf_painter->translate(0,LargeGap );


    //print file info content
    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","FileInfo:\t"), tabFont, Qt::lightGray,false,true);
    printFileInfo(pdf_painter,fileData.info);

    //add chart
    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","ChartInfo:\t"), tabFont, Qt::lightGray,false,true);
    pdf_painter->translate(0,LargeGap );
    int multiple = rect.width()/chartPixmap.width();
    pdf_painter->scale(multiple,multiple);
    printChart(pdf_painter,ui->widget_chart);
//    pdf_painter->drawPixmap(0, 0, chartPixmap);  // 调用QPainter的对应函数画图
//    pdf_painter->translate(0,chartPixmap.height()  + 50 );

    //add result table
    pdf_painter->scale(1.0/multiple,1.0/multiple);
    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","ResultTable:\t"), tabFont, Qt::lightGray,false,true);
    pdf_painter->translate(0,LargeGap );
    pdf_painter->scale(multiple,multiple);

    multiple = rect.width()/this->width();

    foreach (FormResultTable* rt, resultTableList) {
        QFont font;
        pdf_painter->scale(1.0/multiple,1.0/multiple);

        qDebug()<<pdf_painter->transform().dy()<<"of"<<pdf_painter->viewport().height();

        testIfNeedNewPage(pdf_painter,pdf_writer);

        printBox(pdf_painter, "  "+rt->windowTitle(), font, Qt::white);
        pdf_painter->scale(multiple,multiple);
        printTableValue(pdf_painter,rt->getTableWidget());

        qDebug()<<pdf_painter->transform().dy()<<"of"<<pdf_painter->viewport().height();
    }

    testIfNeedNewPage(pdf_painter,pdf_writer);


//    pdf_painter->drawPixmap(0, 0, progresstablePixmap);  // 调用QPainter的对应函数画图
//    pdf_painter->translate(0,progresstablePixmap.height() + 50 );

    //add device table
    pdf_painter->scale(1.0/multiple,1.0/multiple);
    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","DeviceTable:\t"), tabFont, Qt::lightGray,false,true);
    pdf_painter->translate(0,LargeGap );
    pdf_painter->scale(multiple,multiple);

//    multiple = rect.width()/devicePixmap.width();
//    printTableValue(pdf_painter,ui->widget_deviceInfo->getTableWidget());
    pdf_painter->drawPixmap(SmallGap, 0, devicePixmap);  // 调用QPainter的对应函数画图
    pdf_painter->translate(0, devicePixmap.height() + LargeGap );

    pdf_painter->scale(1.0/multiple,1.0/multiple);

    testIfNeedNewPage(pdf_painter,pdf_writer);

    ///comment
    printBox(pdf_painter, QCoreApplication::translate("FormDataPage","Commnet:\t"), tabFont, Qt::lightGray,false,true);
    pdf_painter->translate(0,LargeGap );
    printMultiText(pdf_painter,fileData.info.Comment);

    QTransform qt;
    qt.translate(0,pdf_painter->window().height() - 10 * LargeGap);
    pdf_painter->setWorldTransform(qt);

    printTester(pdf_painter,fileData.info);
    pdf_painter->end();

//    pdf_painter->drawText(QRect(100, 100, 2000, 200), "pdf writer1");//第一个页面
//    pdf_writer->newPage();
//    pdf_painter->drawText(QRect(100, 100, 2000, 200), "pdf writer2");//第二个页面
    delete pdf_painter;
    delete pdf_writer;
    pdf_file.close();

    QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(pdfFileName)));
}


void FormDataPage::reapply()
{
    QString processName = this->getProcessName();
    QString deviceName = this->getDeviceName();
    QString productName = this->getProductName();

    QString errorString;
    if(processName.isEmpty())
        errorString += tr("Process is empty!\n");
    else
        this->applyProcess(processName);

    if(deviceName.isEmpty())
        errorString += tr("Device is empty!\n");
    else
        this->applyDevice(deviceName);

    if(!errorString.isEmpty())
        QMessageBox::critical(this,tr("Error"),errorString);

}

void FormDataPage::markUnsavedChanges(bool unsaved)
{

}

void FormDataPage::setWaveInfo(const TopTest::WaveInfo &waveInfo)
{
    ui->widget_chart->setWaveInfo(waveInfo);
    if(waveInfo.useHandle)
        this->reapply();
}




void FormDataPage::init()
{
    //    QLabel* label = ui->widget_chart->getSettingTypeToEditMap()["Progress"];
//    applyProcess(label->text());
}

QString FormDataPage::getFilePath() const
{
    return filePath;
}

void FormDataPage::setFilePath(const QString &value)
{
    filePath = value;
}

QList<QMap<QString, QVariant> > FormDataPage::getSettingFileMapList() const
{
    QList<QMap<QString, QVariant> > ret;
    ret<<this->productMap
       <<this->deviceMap
       <<this->progressMap;
    return ret;
}

void FormDataPage::setSettingFileMapList(const QList<QMap<QString, QVariant> > &mapList)
{
    this->productMap = mapList.at(0);
    this->deviceMap = mapList.at(1);
    this->progressMap = mapList.at(2);
}

TopTest::WaveInfo FormDataPage::getWaveInfo()
{
    return ui->widget_chart->getWaveInfo();
}

TopTest::FileData& FormDataPage::getFileData()
{
    fileData.info.Product = getProductName();
    fileData.info.Progress = getProcessName();
    fileData.info.Device = getDeviceName();

    return fileData;
}

void FormDataPage::setFileData(const TopTest::FileData &value)
{
    fileData = value;
}

QString outputTableWidgetContent(QTableWidget* table,bool sutabileSection)
{
    QString str;
    QTextStream ts(&str);

    if(!sutabileSection)
    {
        for(int i=0;i<table->rowCount();i++)
        {
            for(int j=0;j<table->columnCount();j++)
            {
                QTableWidgetItem* item = table->item(i,j);
                if(item)
                {
                    ts<<item->data(Qt::DisplayRole).toString();
                }

                ts<<"\t";
            }

            ts<<"\n";
        }
    }

    else
    {
        const int maxSectionNum = 10;

        for(int sectionNo = 0; sectionNo < (table->columnCount() - 2 )/ maxSectionNum + 1; sectionNo++)
        {
            for(int i=0;i<table->rowCount();i++)
            {
                for(int j=0;j<2;j++)
                {
                    QTableWidgetItem* item = table->item(i,j);
                    if(item)
                    {
                        ts<<item->data(Qt::DisplayRole).toString();
                    }

                    ts<<"\t";
                }

                for(int j= 2 + sectionNo*maxSectionNum ; j< 12 + sectionNo*maxSectionNum && j<table->columnCount();j++)
                {
                    QTableWidgetItem* item = table->item(i,j);
                    if(item)
                    {
                        ts<<item->data(Qt::DisplayRole).toString();
                    }

                    ts<<"\t";
                }

                ts<<"\n";
            }
        }
    }

    return str;
}


QString FormDataPage::getResultTableContentInStr(bool sutabileSection)
{
    QString ret;
    ///only need one
    foreach (FormResultTable* resultTable,resultTableList ) {
        ret += resultTable->windowTitle() + "\n";
        ret += outputTableWidgetContent(resultTable->getTableWidget(),sutabileSection);
        ret += "\n";
    }
    return ret;
}

QString FormDataPage::getDeviceTableContentInStr(bool sutabileSection)
{
    return outputTableWidgetContent(ui->widget_deviceInfo->getTableWidget(),sutabileSection);
}

QString FormDataPage::getSourceDataString()
{
    return ui->widget_chart->getSourceDataString();
}

double FormDataPage::getVelocity()
{
    return ui->widget_chart->getVelocity();
}

QString FormDataPage::getDeviceName()
{
    return ui->widget_chart->getDeviceName();
}

QString FormDataPage::getProductName()
{
    return ui->widget_chart->getProductName();
}

QString FormDataPage::getProcessName()
{
    return ui->widget_chart->getProcessName();
}

void FormDataPage::setProductName(const QString &name)
{
    ui->widget_chart->setProductName(name);
}

void FormDataPage::setVelocity(const double &velocity)
{
    ui->widget_chart->setVelocity(velocity);
}

QByteArray FormDataPage::getChartData() const
{
    return fileData.pointData;
}
