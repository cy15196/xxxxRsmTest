#pragma execution_character_set("utf-8")

#include "formdatachart.h"
#include "ui_formdatachart.h"

#include <QToolButton>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

#include "rsmcoresignalenum.h"
#include "dataprogresshandlealgorithm.h"

#include "formchartaxissetting.h"
#include "formdialog.h"

#include "clabso_ft_dpi.h"
#include "formlinemanager.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageBox>


FormDataChart::FormDataChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDataChart)
{
    ui->setupUi(this);

    initToolButtons();
    ui->widget_chart->setColorLengend(ui->widget_legend);
    settingTypeToEditMap["Device"] = ui->label_Device;
    settingTypeToEditMap["Product"] = ui->label_Product;
    settingTypeToEditMap["Progress"] = ui->label_Progress;
//    ui->label_Device->setText("-");
//    ui->label_Product->setText("-");
//    ui->label_Progress->setText("-");
    ui->label_fileName->setVisible(false);
    initFileSettings();

    ui->widget_tools->setFixedWidth(ClabsoFTDpi::convertSize(60));


    {
        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
        shadow_effect->setOffset(0, 0);
        shadow_effect->setColor(QColor("#e3e9f1"));
        shadow_effect->setBlurRadius(12);
        ui->widget_legend->setGraphicsEffect(shadow_effect);
    }
    {
        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
        shadow_effect->setOffset(0, 0);
        shadow_effect->setColor(QColor("#e3e9f1"));
        shadow_effect->setBlurRadius(12);
        ui->widget_tools->setGraphicsEffect(shadow_effect);
    }



    connect(ui->widget_chart,SIGNAL(currentTimeChanged(double)),
            this,SIGNAL(currentTimeChanged(double)));

    connect(ui->widget_chart,SIGNAL(timeTypeChanged(int)),
            this,SIGNAL(timeTypeChanged(int)));
    connect(ui->widget_chart,SIGNAL(tempTypeChanged(int)),
            this,SIGNAL(tempTypeChanged(int)));

//    ui->widget_chart->hide();


}

FormDataChart::~FormDataChart()
{
    delete ui;
}

void FormDataChart::setFileName(const QString &name)
{
    ui->label_fileName->setText(tr("FileName:%1").arg(name));
}

void FormDataChart::setDeviceName(const QString &name)
{
    ui->label_Device->setText(tr("Device:%1").arg(name));
}

void FormDataChart::setProcessName(const QString &name)
{
    ui->label_Progress->setText(tr("Process:%1").arg(name));
}

void FormDataChart::setProductName(const QString &name)
{
    ui->label_Product->setText(tr("Product:%1").arg(name));
}

void FormDataChart::setTestDateTime(const QDateTime &datetime)
{
    ui->label_dateTime->setText(
                tr("TestDateTime:%1")
                .arg(datetime.toString("yyyy/MM/dd hh:mm:ss")));
    ui->widget_chart->setStartDateTime(datetime);
}

void FormDataChart::setSampleTime(const double &sampleTime)
{
    ui->widget_chart->setSampleTimeInteral(sampleTime);
}

void FormDataChart::setVelocity(const double &velocity)
{
    ui->label_velocity->setText(tr("Velocity:%1 m/s").arg(velocity));
    labelValueMap["Velocity"] = velocity;
    ui->widget_chart->setVelocity(velocity);
}

QString FormDataChart::getDeviceName()
{
    QStringList tempList = ui->label_Device->text().split(":",QString::SkipEmptyParts);
    if(tempList.size() >= 2)
        return ui->label_Device->text().remove(tempList.at(0)+":");
    else
        return QString();
}

QString FormDataChart::getProductName()
{
    QStringList tempList = ui->label_Product->text().split(":",QString::SkipEmptyParts);
    if(tempList.size() >= 2)
        return ui->label_Product->text().remove(tempList.at(0)+":");
    else
        return QString();
}

QString FormDataChart::getProcessName()
{
    QStringList tempList = ui->label_Progress->text().split(":",QString::SkipEmptyParts);
    if(tempList.size() >= 2)
        return ui->label_Progress->text().remove(tempList.at(0)+":");
    else
        return QString();
}

QDateTime FormDataChart::getTestDateTime()
{
    QString timeStr = ui->label_dateTime->text().split(":").last();
    return QDateTime::fromString(timeStr,"yyyy/MM/dd hh:mm:ss");
}

double FormDataChart::getVelocity()
{
    return labelValueMap["Velocity"].toDouble();
}

void FormDataChart::setPortNum(const int &portNum)
{
    ui->widget_chart->setPortNum(portNum);
}

int FormDataChart::getPortNum() const
{
    return ui->widget_chart->getPortNum();
}

void FormDataChart::setCurveData(const QByteArray &data)
{
    ui->widget_chart->setChartData(data);
}

void FormDataChart::addCurveData(const double &sampleTime,const QByteArray &data)
{
    ui->widget_chart->addOneSampleData(data,sampleTime);
}

void FormDataChart::addOneSampleData(const double &sampleTime, const QList<double> &portList)
{
    ui->widget_chart->addOneSampleData(portList,sampleTime);
}

void FormDataChart::setSliderEditable(bool enable)
{
//    ui->rulerSlider->setEnabled(enable);
}

void FormDataChart::initFileSettings()
{
    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    bool needInitFileSetting = false;
    foreach (QString fileType, appSetting.childGroups()) {
        qDebug()<<"fileType:"<<fileType;
        appSetting.beginGroup(fileType);
        QString emptySettingFile = tr(fileType.toUtf8().data()) + ":-" ;
        settingTypeToEditMap[fileType]->setText(emptySettingFile);
        if(!appSetting.childKeys().isEmpty())
        {
            QString firstSettingFile = tr(fileType.toUtf8().data()) + ":" + appSetting.childKeys().first();
            settingTypeToEditMap[fileType]->setText(firstSettingFile);
        }
        else
            needInitFileSetting = true;
        appSetting.endGroup();
    }
    appSetting.endGroup();

    if(needInitFileSetting)
    {
        QMessageBox::warning(this,tr("警告"),tr("某些设置文件为空，请在进行分析时先设置"));
    }
}

void FormDataChart::initToolButtons()
{

    ui->widget_tools->setTranslationContext(this->metaObject()->className());

    QWidget* groupBox =  ui->widget_tools->addGroup("tools",true);

    QList<QToolButton*> toolButtonList;

    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_xixiangjiancha.png"),tr("Chart_ShowDetail"),Chart_ShowDetail,"Chart_ShowDetail");
    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidonghengxian.png"),tr("Chart_FitAll"),Chart_FitAll,"Chart_FitAll");
    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidongquxian.png"),tr("Chart_SetAxis"),Chart_SetAxis,"Chart_SetAxis");
//    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidongquxian.png"),tr("Chart_SetDistanceAndVelocity"),Chart_SetDistanceAndVelocity,"Chart_SetDistanceAndVelocity");
//    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidongquxian.png"),tr("Chart_MoveCurve"),Chart_MoveCurve,"Chart_MoveCurve");
//    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidongwenqu.png"),tr("Chart_MoveSection"),Chart_MoveSection,"Chart_MoveSection");
    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidonghengxian.png"),tr("Chart_AddLine"),Chart_AddHLine,"Chart_AddLine");
//    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidongshuxian.png"),tr("Chart_AddVLine"),Chart_AddVLine,"Chart_AddVLine");

    QToolButton* showTempSectionButton =
     ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidonghengxian.png"),tr("Chart_ShowTempSection"),Chart_ShowTempSection,"Chart_ShowTempSection");
    showTempSectionButton->setCheckable(true);
    showTempSectionButton->setChecked(false);
    toolButtonList<< showTempSectionButton;
    toolButtonList<< ui->widget_tools->addToolButton(groupBox,QIcon(":/images/ic_yidonghengxian.png"),tr("Chart_Reapply"),Chart_Reapply,"Chart_Reapply");


    ui->widget_tools->addSpaceItemToGroup(groupBox);

    foreach (QToolButton* button, toolButtonList) {
        button->setCheckable(true);
//        button->setAutoExclusive(true);
    }

    ui->widget_tools->setGroupToolButtonSize(groupBox,QSize(60,60),0.7);

    connect(ui->widget_tools,SIGNAL(toolButtonToggled(int)),
            this,SLOT(onToolButtonClicked(int)));


//    connect(ui->widget_tools,SIGNAL(toolButtonToggled(int)),
//            this,SIGNAL(pageTriggerButtonChecked(int)));

//    connect(this,SIGNAL(pageTriggerButtonChecked(int)),
//            SLOT(onPageTriggerButtonChecked(int)));

//    toolButtonList.first()->setChecked(true);
//    onPageTriggerButtonChecked(Page_Home);

 //   ui->toolButton_lan->setPopupMode(QToolButton::MenuButtonPopup);
 //   QMenu* lanMenu = new QMenu(this);
 //   ui->toolButton_lan->setDefaultAction(lanMenu->addAction(QIcon(":/ic_zh_CN"),tr("中文")));
 //   lanMenu->addAction(QIcon(":/ic_en_US"),tr("English"));
 //   ui->toolButton_lan->setMenu(lanMenu);
 //   initToolButton(ui->toolButton_lan,":/images/ic_zhongwen.png");
 //   ui->toolButton_lan->hide();

 //   initPixmapLabel(ui->label_lan,":/ic_zh_CN");

//    ui->toolButton_lan->hide();
//    ui->label_lan->hide();


//    ui->comboBox_lan->addItem(/*QIcon(":/ic_zh_CN"),*/tr("中文"),"zh_CN");
//    ui->comboBox_lan->addItem(/*QIcon(":/ic_en_US"),*/tr("English"),"en_US");
//   ui->comboBox_lan->setStyleSheet(ui->comboBox_lan->styleSheet()+
 //                                   "QWidget{"
 //                                   "background-color:rgba(200, 200, 233, 25);"
 //                                   "}"
 //                                   );

//    connect(ui->comboBox_lan,SIGNAL(currentIndexChanged(int)),
//            SLOT(onLanChooseChanged(int)));

//    initPixmapLabel(ui->label_log,":/images/ic_dengluzhihui.png");

}

inline QStringList getTypeParametrKeys(int type)
{

    QStringList ret;

    ret<<"No";

    switch (type) {
    case TopTest::IncreaseTime:
//    case TopTest::RemainTime:
    case TopTest::TestValuedTime:
    case TopTest::TemputureBetweenTime:
    {
        ret<<"TemperatureMinValue"<<"TemperatureMaxValue"
          <<"RefMinValue"<<"RefMaxValue"
         <<"Warning"
        <<"PortChoosenList";
    }
        break;
    case TopTest::AboveTime:
    {
        ret<<"TemperatureValue"
          <<"RefMinValue"<<"RefMaxValue"
         <<"Warning"
        <<"PortChoosenList";
    }
        break;
//    case TopTest::ReturnTime:
//    {
//        ret<<"TemperatureMinValue"<<"TemperatureMaxValue"
//          <<"RefMinValue"<<"RefMaxValue"
//         <<"Warning"
//        <<"PortChoosenList";
//    }
//        break;
    case TopTest::DecreaseTime:
    {
        ret<<"TemperatureMaxValue"<<"TemperatureMinValue"
         <<"RefMinValue"<<"RefMaxValue"
         <<"Warning"
         <<"PortChoosenList";
    }
        break;
    case TopTest::IncreaseRatio:
    {

        ret
          <<"TemperatureMinValue"<<"TemperatureMaxValue"
          <<"RatioSampleTime"
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::DecreaseRatio:
    {

        ret
          <<"TemperatureMaxValue"<<"TemperatureMinValue"
          <<"RatioSampleTime"
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::MaxIncreaseRatio:
    case TopTest::MaxDecreaseRatio:
    {
        ret
          <<"RatioSampleTime"
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::MaxIncreaseTime:
    case TopTest::MaxDecreaseTime:
    case TopTest::MaxPortMaxDeltaValue:
    {
        ret
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::DistanceToMaxTimeSection:
    {
        ret
          <<"SectionHeight"
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::AverageRatio:
    {

        ret
          <<"TemperatureMinValue"<<"TemperatureMaxValue"
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    case TopTest::MaxTemputure:
    {
        ret
          <<"RefMinValue"<<"RefMaxValue"
          <<"Warning"
          <<"PortChoosenList";
    }
        break;
    default:
        break;
    }

    return ret;
}

QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > > FormDataChart::
    parserSpecialProgress(const QString &progressName)
{
    qDebug()<<"go to apply progress"<<progressName;
    QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > > ret;

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup("Progress");

    if(appSetting.allKeys().isEmpty())
        return ret;

    QString firstKeyName = appSetting.allKeys().first();
    QString realProcessName = progressName;

    qDebug()<<"firstKeyName"<<firstKeyName;
    qDebug()<<"realProcessName"<<realProcessName;

    QVariant progressSetting  = appSetting.value(realProcessName);
    QMap<QString,QVariant> typeProgressMap = progressSetting.toMap()/*.value("widget_DataTable").toMap()*/;
    qDebug()<<"process map is"<<typeProgressMap;

    appSetting.endGroup();
    appSetting.endGroup();

    return parserSpecialProgress(typeProgressMap);
}

QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > >
FormDataChart::parserSpecialProgress(const QMap<QString, QVariant> &processMap)
{
    QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > > ret;


    foreach (QString typeStr, processMap.keys()) {
        int type = typeStr.toInt();
        QList<QPair<QList<int>, QMap<QString, double> > > typeRecordList ;
        QStringList keyWordList = getTypeParametrKeys(type);
        QMap<QString,QVariant> recordListMap=  processMap.value(typeStr).toMap();
        qDebug()<<"recordListMap"<<recordListMap;
        foreach (QString rowStr,recordListMap.keys() ) {
            QMap<QString,QVariant> columnMap = recordListMap.value(rowStr).toMap();
            qDebug()<<"columnMap"<<columnMap;
            QMap<QString,double> parameterMaps;
            QList<int> portList;
            QStringList portStrList  = columnMap.value(QString::number(keyWordList.count()-1)).toString().split(";");
            foreach (QString portStr, portStrList) {
                portList.push_back(portStr.toInt());
            }

            for(int i= 0; i < keyWordList.count()-1; i++)
            {
                parameterMaps.insert(
                            keyWordList.at(i),
                            columnMap.value(QString::number(i)).toDouble());
            }

            typeRecordList.push_back({portList,parameterMaps});
        }

        ret.insert(type,typeRecordList);
    }

    qDebug()<<"process data is"<<ret;

    return ret;
}

//void FormDataChart::applyOneProgressToPortList(int algType, QPair<QList<int>, QMap<QString, double> > oneRecordProgress)
//{
//    foreach (int portNo, oneRecordProgress.first) {
//        DataProgressHandleAlgorithm::instance()->setInput(ui->widget_chart->getPortPointFVector(portNo));
////        QMap<QString,double> paramters;
////        paramters.insert("TemperatureMinValue",200.0);
////        paramters.insert("TemperatureMaxValue",750.0);
////        paramters.insert("TemperatureRemainValue",500);
//        DataProgressHandleAlgorithm::instance()->setParameters(oneRecordProgress.second);

//        DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::IncreaseTime);
//        DataProgressHandleAlgorithm::instance()->update();

////        DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::AboveTime);
////        DataProgressHandleAlgorithm::instance()->update();
//    }
//}

QMap<QString, QLabel *> FormDataChart::getSettingTypeToEditMap() const
{
    return settingTypeToEditMap;
}

void FormDataChart::setSettingTypeToEditMap(const QMap<QString, QLabel *> &value)
{
    settingTypeToEditMap = value;
}

QMap<int, QMap<int, QList<QMap<int, QVariant> > > > FormDataChart::applyProgress(
        const QString &progressName)
{
    qDebug()<<"go to apply progress"<<progressName;
    QMap<int, QMap<int, QList<QMap<int, QVariant> > > > ret;

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup("Progress");

    if(appSetting.allKeys().isEmpty())
        return ret;

    QString firstKeyName = appSetting.allKeys().first();
    QString realProcessName = progressName;

    qDebug()<<"firstKeyName"<<firstKeyName;
    qDebug()<<"realProcessName"<<realProcessName;

    QVariant progressSetting  = appSetting.value(realProcessName);
    QMap<QString,QVariant> typeProgressMap = progressSetting.toMap()/*.value("widget_DataTable").toMap()*/;
    qDebug()<<"process map is"<<typeProgressMap;

    appSetting.endGroup();
    appSetting.endGroup();

    return applyProgress(typeProgressMap);
}

QMap<int, QMap<int, QList<QMap<int, QVariant> > > > FormDataChart::applyProgress(
        const QMap<QString, QVariant> &processMap)
{
    QMap<int, QMap<int, QList<QMap<int, QVariant> > > > progressResult;

    QMap<QString, QVariant> waveTableDataMap = processMap.value("tableWidget_wave").toMap();
    if(waveTableDataMap.values().count() && waveTableDataMap.values().first().toMap().isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("工艺分析项目为空！"));
    }

    //clear now wave range
    ui->widget_chart->setWaveRangeList({});

    foreach (QString rowStr, waveTableDataMap.keys()) {
        int waveRow = rowStr.toInt();
        QMap<QString,QVariant > rowMap = waveTableDataMap[QString::number(waveRow)].toMap();
        bool waveEnable = true;
        waveEnable = rowMap[QString::number(0)].toInt() == Qt::Checked;
        //parser wave is enabled and its value
        if(waveEnable == false)
            break;
        QPointF waveStartAndEndTemp;
        waveStartAndEndTemp.setX(rowMap[QString::number(1)].toDouble());
        waveStartAndEndTemp.setY(rowMap[QString::number(2)].toDouble());

        QMap<int, QList<QMap<int, QVariant> > > waveProgressResult;

        ///set and add current wave start and end temp point
//        DataProgressHandleAlgorithm::instance()->setCurrentWaveNo(waveRow+1);
//        DataProgressHandleAlgorithm::instance()->insertWaveStartEnd(waveRow+1,waveStartAndEndTemp);
        ui->widget_chart->appendWaveDef(waveStartAndEndTemp);

        QMap<QString, QVariant> waveDataMap = processMap.value("widget_DataTable_"+QString::number(waveRow+1)).toMap();
        QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > > progressData =
                parserSpecialProgress(waveDataMap);
        qDebug()<<"wave"<<waveRow+1<<"parsered process is"<<progressData;

        foreach (int algoType , progressData.keys()) {
            qDebug()<<"begin algoType"<<algoType;
            QList<QMap<int, QVariant> > algoTypeResultList;
            QList<QPair<QList<int>, QMap<QString, double>>> typeDataList =
                    progressData.value(algoType);
            int recordNo = 0;
            foreach (auto oneRecord, typeDataList) {
                qDebug()<<"begin record"<<recordNo;
                QMap<int, QVariant> oneResult;
                double pwiValue = 0;
                ///can do some parrllel in future
                QMap<QString,QVariant> resultMap;
                double maxValue = -1e6;
                double minValue = 1e6;
                foreach (int portNo, oneRecord.first) {
                    qDebug()<<"begin port"<<portNo;
                    DataProgressHandleAlgorithm::instance()->setInput(
                                ui->widget_chart->getPortWaveData(portNo,waveRow));
                    DataProgressHandleAlgorithm::instance()->setParameters(oneRecord.second);
                    DataProgressHandleAlgorithm::instance()->setAlgoType(algoType);
                    DataProgressHandleAlgorithm::instance()->update();
                    resultMap =  DataProgressHandleAlgorithm::instance()->getCurrentAlgoResult();
                    qDebug()<<"port result"<<resultMap;
                    QVariant value =resultMap["Result"];

                    bool ok;
                    double currentValue = value.toDouble(&ok);
                    if(ok)
                    {
                        if(currentValue > maxValue)
                            maxValue = currentValue;
                        else if(currentValue < minValue)
                            minValue = currentValue;
                    }

                    oneResult.insert(1,
                                     resultMap["PresetStr"].toString()
                                     +" ("+resultMap["PresetUnit"].toString()+")");
                    oneResult.insert(portNo+1,value);

                    double currentPWI = resultMap["PWI"].toDouble();
                    if(abs(currentPWI) > abs(pwiValue))
                    {
                        pwiValue = currentPWI;
                    }
                }

                if(algoType == TopTest::MaxPortMaxDeltaValue)
                {
                    double delta = maxValue - minValue;
                    foreach (int portNo, oneRecord.first)
                    {
                        oneResult.insert(portNo+1,delta);
                    }
                }

                oneResult.insert(this->getPortNum()+2,QString("%1%").arg(pwiValue));
    //            oneResult.insert(this->getPortNum()+2,resultMap["PresetStr"]);
    //            oneResult.insert(this->getPortNum()+3,resultMap["PresetUnit"]);
                oneResult.insert(this->getPortNum()+3,resultMap["RefStr"]);
                oneResult.insert(this->getPortNum()+4,resultMap["RefUnit"]);

                algoTypeResultList<<oneResult;
                recordNo++;
            }
            waveProgressResult.insert(algoType,algoTypeResultList);
        }

        progressResult.insert(waveRow+1,waveProgressResult);

    }


    ui->widget_chart->replot();

    qDebug()<<"process result is"<<progressResult;

    return progressResult;
}

QMap<int, QPair<QString, QColor> > FormDataChart::getPortInfoMap()
{
   return  ui->widget_chart->getPortInfoMap();
}

void FormDataChart::setPortInfos(const QMap<int, QPair<QString, QColor> > &portInfoMap)
{
    ui->widget_chart->setPortInfos(portInfoMap);
}

void FormDataChart::moveSliderToTime(const double &time)
{
    ui->widget_chart->moveToTime(time);
}

void FormDataChart::setSliderRange(const double &max, const double &min)
{
//    ui->widget_chart->setSliderRange(max,min);
}

void FormDataChart::drawDeviceTempSection(const QString &deviceName)
{
    qDebug()<<"go to apply device"<<deviceName;

    QSettings appSetting(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    appSetting.beginGroup("FileSettings");
    appSetting.beginGroup("Device");

    if(!appSetting.allKeys().isEmpty())
    {
        QString firstKeyName = appSetting.allKeys().first();
        qDebug()<<"firstDeviceName"<<firstKeyName;
    }

    QString realProcessName = deviceName;
    qDebug()<<"realDeviceName"<<realProcessName;

    QVariant progressSetting  = appSetting.value(realProcessName);
    QMap<QString,QVariant> progressMap =  progressSetting.toMap();
    QMap<QString,QVariant> typeDeviceMap = progressMap.value("tableWidget_device").toMap();


    appSetting.endGroup();
    appSetting.endGroup();

    qDebug()<<"begin draw temp section";
    qDebug()<<"device map is"<<progressMap;
    qDebug()<<"table map is"<<typeDeviceMap;

    TopTest::TempSection section;
    section.isDoubleSection = progressMap.value("radioButton_3").toBool();
    qDebug()<<"isDoubleSection"<<section.isDoubleSection ;

    double startTemp = progressMap.value("doubleSpinBox_startTemp").toDouble();
    double endTemp = progressMap.value("doubleSpinBox_endTemp").toDouble();
    double wholeLength = progressMap.value("spinBox_length").toDouble();

    QPointF timeRange = DataProgressHandleAlgorithm::instance()->parserTempSectionRange(
                ui->widget_chart->getAllPortPointFVector(),
                startTemp,endTemp);

    qDebug()<<"parser time range is"<<timeRange;
    double currentTime = timeRange.x();
    double radio = (timeRange.y() - timeRange.x()) /  wholeLength;

    for(int i = 0 ;i < typeDeviceMap.keys().count();i++ ) {
        QMap<QString,QVariant> columnMap = typeDeviceMap.value(QString::number(i)).toMap();
        TopTest::TempSectionItem item;
        item.name = columnMap.value("1").toString();
        item.startTime = currentTime;
        double length = columnMap.value("2").toDouble();
        item.endTime = currentTime + radio * length ;
        currentTime = item.endTime ;
        item.upValue = columnMap.value("3").toDouble();
        item.downValue = columnMap.value("4").toDouble();
        qDebug()<<"section item"<<item.name<<item.startTime<<item.endTime<<item.upValue<<item.downValue;
        section.sections.append(item);
    }

    ui->widget_chart->reDrawTempSection(section);
}

void FormDataChart::drawDeviceTempSection(const QMap<QString, QVariant> &deviceMap)
{

    QMap<QString,QVariant> progressMap =  deviceMap;
    QMap<QString,QVariant> typeDeviceMap = progressMap.value("tableWidget_device").toMap();


    qDebug()<<"begin draw temp section";
    qDebug()<<"device map is"<<progressMap;
    qDebug()<<"table map is"<<typeDeviceMap;

    TopTest::TempSection section;
    section.isDoubleSection = progressMap.value("radioButton_3").toBool();
    qDebug()<<"isDoubleSection"<<section.isDoubleSection ;

    double startTemp = progressMap.value("doubleSpinBox_startTemp").toDouble();
    double endTemp = progressMap.value("doubleSpinBox_endTemp").toDouble();
    double wholeLength = progressMap.value("spinBox_length").toDouble();

    QPointF timeRange = DataProgressHandleAlgorithm::instance()->parserTempSectionRange(
                ui->widget_chart->getAllPortPointFVector(),
                startTemp,endTemp);

    qDebug()<<"parser time range is"<<timeRange;
    double currentTime = timeRange.x();
    double radio = (timeRange.y() - timeRange.x()) /  wholeLength;

    for(int i = 0 ;i < typeDeviceMap.keys().count();i++ ) {
        QMap<QString,QVariant> columnMap = typeDeviceMap.value(QString::number(i)).toMap();
        TopTest::TempSectionItem item;
        item.name = columnMap.value("1").toString();
        item.startTime = currentTime;
        double length = columnMap.value("2").toDouble();
        item.endTime = currentTime + radio * length ;
        currentTime = item.endTime ;
        item.upValue = columnMap.value("3").toDouble();
        item.downValue = columnMap.value("4").toDouble();
        qDebug()<<"section item"<<item.name<<item.startTime<<item.endTime<<item.upValue<<item.downValue;
        section.sections.append(item);
    }

    ui->widget_chart->reDrawTempSection(section);
}



QList<double> FormDataChart::getPortValueListInTime(const double &time)
{
//    return {};
    return ui->widget_chart->getCurrentPortsTempList(time);
}

//void FormDataChart::setRulerSliderPos(int  timeCount)
//{

//}

QString FormDataChart::getSourceDataString()
{
    QString dataString;
    dataString += tr("Date")+","+tr("Time");

    QMap<int, QPair<QString, QColor> > portInfoMap = this->getPortInfoMap();
    foreach (int portNo, portInfoMap.keys()) {
        dataString += "," + QString("%1:%2").arg(portNo).arg(portInfoMap.value(portNo).first);
    }
    dataString +="\n"+ ui->widget_chart->getSourceDataString();

    return dataString;
}

QWidget *FormDataChart::getToolGroupWidget()
{
    return ui->widget_tools;
}

void FormDataChart::clearData()
{
    ui->widget_chart->clearChart();
}

QByteArray FormDataChart::getCurrentCurveData()
{
    return ui->widget_chart->getCurveData();
}

QPixmap FormDataChart::formChartPixmap()
{
    ui->widget_chart->setMinimumHeight(500);
//    ui->widget_chart->hideForGrab(true);
    QPixmap map = ui->widget_chart->grab();
//    ui->widget_chart->hideForGrab(false);
    ui->widget_chart->setMinimumHeight(200);
    return map;
}

QPixmap FormDataChart::formLengendPixmap()
{
    return ui->widget_legend->grab();
}

void FormDataChart::setDrawWave(bool drawWave)
{
    ui->widget_chart->setDrawWave(drawWave);
}

void FormDataChart::setWaveInfo(const TopTest::WaveInfo &waveInfo)
{
    ui->widget_chart->setWaveInfo(waveInfo);
}

TopTest::WaveInfo FormDataChart::getWaveInfo()
{
    return ui->widget_chart->getWaveInfo();
}

void FormDataChart::onToolButtonClicked(int button)
{
    qDebug()<<"FormDataChart::onToolButtonClicked"<<button;
    switch (button) {
    case Chart_ShowDetail:
    {
        ui->widget_chart->changeToolTipEnable();
    }
        break;

    case Chart_FitAll:
    {
        ui->widget_chart->changeTimeFitType();
    }
        break;

    case Chart_SetAxis:
    {
        FormChartAxisSetting *dialog = new FormChartAxisSetting(this);
        dialog->setAxisLableInfos(ui->widget_chart->getAxisLabelInfos());
        connect(dialog,SIGNAL(axisLabelInfoChanged(QMap<QString,TopTest::AxisLabelInfo>)),
                ui->widget_chart,SLOT(setAxisLabelInfo(QMap<QString,TopTest::AxisLabelInfo>)));
        FormDialog fd(dialog);
        fd.setWindowTitle(tr("设置坐标轴"));
        fd.exec();
    }
        break;

    case Chart_AddHLine:
    {
//        ui->widget_chart->changeToDrawHLineMode();
        FormLineManager *dialog = new FormLineManager(this);
        dialog->setCurrentLines({});
        connect(dialog,SIGNAL(needToAddLine(TopTest::LineInfo)),
                ui->widget_chart,SLOT(addLine(TopTest::LineInfo)));
        connect(dialog,SIGNAL(needToClearAll()),
                ui->widget_chart,SLOT(clearAllLine()));
        FormDialog fd(dialog);
        fd.setWindowTitle(tr("添加线条"));
        fd.exec();
    }
        break;

//    case Chart_AddVLine:
//    {
////        ui->widget_chart->changeToDrawVLineMode();
//    }
//        break;

    ///show temp section
    case Chart_ShowTempSection:
    {
        ui->widget_chart->changeTempSectionVisiable();
    }
        break;

    ///reapply progress
    case Chart_Reapply:
    {
        emit this->needToReapply();
        ui->widget_chart->reFit();
    }
        break;

    default:
        break;
    }
}

void FormDataChart::showTimeLineInValue(const double &value)
{

}
