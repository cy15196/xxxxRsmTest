#pragma execution_character_set("utf-8")

#include "qcpchartwidget.h"

#include "rsmusb.h"
#include "formcolorlengend.h"

#include "rulerslider.h"
#include "clabso_ft_dpi.h"

#include "qcplabeltracer.h"

#include <QGridLayout>
#include <QDataStream>
#include <QDateTime>

QCPChartWidget::QCPChartWidget(QWidget *parent)
    :customPlot(new QCustomPlot(this)),
//      rs(new RulerSlider),
      QWidget(parent)
{
    QGridLayout* layout = new QGridLayout;
    this->setLayout(layout);

    layout->addWidget(customPlot);
//    layout->addWidget(rs);
//    rs->setFixedHeight(ClabsoFTDpi::convertSize(40));

    setDrawWave(false);


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    customPlot->xAxis->setTicker(timeTicker);

    customPlot->axisRect()->addAxes(QCPAxis::atBottom);
//    customPlot->axisRect()->axis(QCPAxis::atLeft,0)->setPadding(30);
    customPlot->axisRect()->axis(QCPAxis::atBottom,0)->setPadding(20);
//    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setScaleRatio(
//                customPlot->axisRect()->axis(QCPAxis::atBottom,0),0.3);
    customPlot->xAxis->setLabel(
                tr("时间:")
                +getTimeLabelTypeToString(timeType).split("_").last());
    customPlot->yAxis->setLabel(
                tr("温度:")
                +getTemperatureLabelTypeToString(tempType).split("_").last());
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setLabel(
                tr("距离:")
                +getDistanceLabelTypeToString(disType).split("_").last());

    customPlot->xAxis->setSelectableParts(QCPAxis::spAxis);
    customPlot->yAxis->setSelectableParts(QCPAxis::spAxis);
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setSelectableParts(QCPAxis::spAxis);

    HLine = new QCPItemStraightLine(customPlot);
    VLine = new QCPItemStraightLine(customPlot);

    Pos = new QCPItemEllipse(customPlot);
    mCenterTracer = new QCPItemTracer(customPlot);
    mCenterTracer->setStyle(QCPItemTracer::tsNone);
    Pos->topLeft->setParentAnchor(mCenterTracer->position);
    Pos->bottomRight->setParentAnchor(mCenterTracer->position);
    Pos->topLeft->setCoords(-2,-2);
    Pos->bottomRight->setCoords(2,2);
    Pos->topLeft->setType(QCPItemPosition::ptAbsolute);
    Pos->bottomRight->setType(QCPItemPosition::ptAbsolute);

    xAxisTracer = new QCPLabelTracer(customPlot,QCPLabelTracer::XAxisTracer);
    yAxisTracer = new QCPLabelTracer(customPlot,QCPLabelTracer::YAxisTracer);
    rAxisTracer = new QCPLabelTracer(customPlot,QCPLabelTracer::RAxisTracer);

//    xLabelUnit = new QCPLabelTracer(customPlot,QCPLabelTracer::AxisUnit);
//    yLabelUnit = new QCPLabelTracer(customPlot,QCPLabelTracer::AxisUnit);
//    rLabelUnit = new QCPLabelTracer(customPlot,QCPLabelTracer::AxisUnit);

//    xLabelUnit->setVisible(true);
//    yLabelUnit->setVisible(true);
//    rLabelUnit->setVisible(true);

//    xLabelUnit->setAxisType(QCPLabelTracer::XAxis);
//    yLabelUnit->setAxisType(QCPLabelTracer::YAxis);
//    rLabelUnit->setAxisType(QCPLabelTracer::RAxis);

//    xLabelUnit->updatePosition(0.5,1);
//    yLabelUnit->updatePosition(0,0.5);
//    rLabelUnit->updatePosition(0.5,1);



//    customPlot->axisRect()->setMargins(QMargins(50,50,50,50));

    customPlot->legend->setVisible(false);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->legend->setRowSpacing(-3);
//    customPlot->legend->setBrush(QColor(255,255,255,50));

//    HLine->setVisible(false);
//    VLine->setVisible(false);
//    Pos->setVisible(false);
//    xAxisTracer->setVisible(false);
//    yAxisTracer->setVisible(false);
//    rAxisTracer->setVisible(false);

    reinit();

//    this->setStyleSheet("font: 75 14pt \"Times New Roman\";");

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this, SLOT(onTimeRangeChanged(QCPRange)));


    connect(customPlot,SIGNAL(mouseMove(QMouseEvent*)),
            this,SLOT(onMouseMove(QMouseEvent*)));

    connect(customPlot,SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,SLOT(onAxisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));


    connect(&autoScrollTimer,SIGNAL(timeout()),
            this,SLOT(turnAutoScrollOn()));

}

QCPChartWidget::~QCPChartWidget()
{
    delete customPlot;
}

void QCPChartWidget::reinit()
{

    customPlot->axisRect()->setupFullAxesBox(true);

    QFont TNRFont =  customPlot->xAxis->tickLabelFont();
//    TNRFont.setFamily("Times New Roman");
//    TNRFont.setPointSize(10);
    TNRFont.setBold(true);
    customPlot->xAxis->setTickLabelFont(TNRFont);
//    customPlot->xAxis->setTickLabelColor(QColor(Qt::black));
    customPlot->yAxis->setTickLabelFont(TNRFont);
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setTickLabelFont(TNRFont);

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);

    ///鼠标交互
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);

    QPen subpen = customPlot->xAxis->grid()->subGridPen();
    subpen.setColor(QColor(0,0,0,120));
    customPlot->xAxis->grid()->setSubGridPen(subpen);
    customPlot->yAxis->grid()->setSubGridPen(subpen);

    QPen pen = customPlot->xAxis->grid()->pen();
    pen.setColor(QColor(0,0,0,180));
    pen.setStyle(Qt::DashLine);
    customPlot->xAxis->grid()->setPen(pen);
    customPlot->yAxis->grid()->setPen(pen);

    changeTimeLabelType(0);
    changeTempLabelType(0);
    changeDistanceLabelType(0);
}

inline QVector<QPointF> getPointData(QCPGraph* line)
{
    QVector<QPointF> lineData;
    QSharedPointer<QCPGraphDataContainer> dataList = line->data();
    for(int i=0;i<dataList->size();i++)
    {
        const QCPGraphData* data = dataList->at(i);
        lineData<<QPointF(data->key,data->value);
    }
    return lineData;
}

QVector<QPointF> QCPChartWidget::getPortPointFVector(int portNo)
{
    QVector<QPointF> ret;
    QCPGraph* line = portToGraphMap.value(portNo);
    if(line)
    {
        ret = getPointData(line);
    }
    qDebug()<<"QCPChartWidget::ret"<<ret;
    return ret;
}




QList<QVector<QPointF> > QCPChartWidget::getAllPortPointFVector()
{
    QList<QVector<QPointF> > ret;
    foreach (QCPGraph* line, portToGraphMap.values()) {
        ret<<getPointData(line);
    }

    return ret;
}

QList<double> QCPChartWidget::getCurrentPortsTempList(double currentTime)
{
    QList<double> ret;
    foreach (QCPGraph* graph, portToGraphMap.values()) {
        double value = 100.0;
        auto iter = graph->data()->findBegin(currentTime);
        value = iter->mainValue();
        ret<<value;
    }
    return ret;
}

void QCPChartWidget::setChartData(const QByteArray &dataArray)
{
    clearChart();
    this->curveData = dataArray;

    if(this->portNum == 0 /*|| !this->startDateTime.isValid()*/)
        return;

    reinit();

    qDebug()<<"add curve data to chart "<<dataArray.length()<<"of port num = "<<this->portNum;
//    this->curveData = dataArray;
     this->tempTime= 0.0;
    if( this->sampleInteral == 0.0)
          this->sampleInteral = 0.1;

    auto dataList = RSMUsb::parserFileDataToRecordList(portNum,dataArray);
    qDebug()<<"record count num is"<<dataList.count();
    foreach (QList<double> portValueList, dataList) {
        addOneSampleData(portValueList,tempTime);
         this->tempTime += this->sampleInteral;
    }

    reFit();

//    QMap<int,QList<QPointF>> portWaveEndPointListMap;

//    portWaveEndPointListMap.insert(1,{QPointF(20,400),QPointF(600,1800)});
//    portWaveEndPointListMap.insert(2,{QPointF(30,500),QPointF(700,1750)});
//    portWaveEndPointListMap.insert(3,{QPointF(50,350),QPointF(550,1900)});

//    drawWave(portWaveEndPointListMap);
}

void QCPChartWidget::clearChart()
{
    customPlot->clearGraphs();
    portToGraphMap.clear();
    this->curveData.clear();

    replot();
}

void QCPChartWidget::setPortNum(const int &portNum)
{
    if(this->portNum == portNum)
        return;

    this->portNum = portNum;
    colorLengend->setPortNum(portNum);
}

void QCPChartWidget::setStartDateTime(const QDateTime &testDateTime)
{
    this->startDateTime = testDateTime;
    if(timeType == TimeLabel_Time)
    {
        QSharedPointer<QCPAxisTickerOffsetDateTime> tp =
            customPlot->xAxis->ticker().dynamicCast<QCPAxisTickerOffsetDateTime>();
        tp->setOffset(this->startDateTime);
    }
}

void QCPChartWidget::setSampleTimeInteral(const double &sampleInteral)
{
    this->sampleInteral = sampleInteral;
}

void QCPChartWidget::reFit()
{

    QCPRange yrange;
    yrange.lower = 5000;
    yrange.upper = -5000;

    QCPRange xrange;
    xrange.lower = 0.0;
    xrange.upper = 5.0;

    if(inFullRange)
    {
        for(int i=0;i<this->portNum;i++)
        {
           QCPGraph* line = portToGraphMap.value(i+1);
           if(!line || !line->visible())
               continue;
           bool ok;
           QCPRange tempRange = line->getValueRange(ok);
           if(ok)
           {
               if(tempRange.lower < yrange.lower)
                   yrange.lower = tempRange.lower;
               if(tempRange.upper > yrange.upper)
                   yrange.upper = tempRange.upper;
           }

           tempRange = line->getKeyRange(ok);
           if(ok)
           {
               if(tempRange.lower < xrange.lower)
                   xrange.lower = tempRange.lower;
               if(tempRange.upper > xrange.upper)
                   xrange.upper = tempRange.upper;
           }
        }
    }

    else{

        for(int i=0;i<this->portNum;i++)
        {
           QCPGraph* line = portToGraphMap.value(i+1);
           if(!line || !line->visible())
               continue;
           bool ok;
           QCPRange tempRange = line->getKeyRange(ok);
           if(ok)
           {
               if(tempRange.lower < xrange.lower)
                   xrange.lower = tempRange.lower;
               if(tempRange.upper > xrange.upper)
                   xrange.upper = tempRange.upper;
           }
        }

        xrange.lower = xrange.upper - size;
        if(xrange.lower<0)
            xrange.lower = 0;

        for(int i=0;i<this->portNum;i++)
        {
           QCPGraph* line = portToGraphMap.value(i+1);
           if(!line || !line->visible())
               continue;
           bool ok;
           QCPRange tempRange = line->getValueRange(ok,QCP::sdBoth,xrange);
           if(ok)
           {
               if(tempRange.lower < yrange.lower)
                   yrange.lower = tempRange.lower;
               if(tempRange.upper > yrange.upper)
                   yrange.upper = tempRange.upper;
           }
        }
    }


    double adjust =  yrange.size() * 0.05 + 0.001;
    yrange.lower -= adjust;
    yrange.upper += adjust;


    customPlot->yAxis->setRange(yrange);
    customPlot->xAxis->setRange(xrange);

    customPlot->replot();
}

void QCPChartWidget::setWaveInfo(const WaveInfo &waveInfo)
{
    this->waveinfo = waveInfo;
    if(waveInfo.useHandle)
    {
        applyHandleTimeSectionToLines(waveInfo);
    }
//    if(waveInfo.waveTimeSection.)
}

WaveInfo QCPChartWidget::getWaveInfo()
{
    return this->waveinfo;
}

void QCPChartWidget::applyHandleTimeSectionToLines(const WaveInfo &waveInfo)
{
    //conver time value to time index;

    if(portToGraphMap.values().isEmpty())
        return;

    qDebug()<<"applyHandleTimeSectionToLines"
           <<waveInfo.useHandle
           <<waveInfo.waveCount
           <<waveInfo.waveNameMap
           <<waveInfo.waveTimeSection;

    QCPGraphWave* waveLine = portToGraphMap.first();
    QVector<QPointF> tempRet = getPointData(waveLine);
    if(tempRet.isEmpty())
        return;

    qDebug()<<"tempRet"<<tempRet;

    QList<QCPDataRange> waveSaveRange;
    int nowIndex = 0;
    int startIndex = 0;
    int endIndex = tempRet.size()-1;

    QMap<int ,QPointF> hanleSection = waveInfo.waveTimeSection;


    foreach (QPointF timePoint, hanleSection.values()) {

        qDebug()<<"tempPoint"<<timePoint;
        startIndex = nowIndex;
        endIndex = tempRet.size()-1;

        bool findStart = false;

        for(int i = nowIndex ; i < tempRet.size(); i++)
        {
            QPointF currentP = tempRet.at(i);
            if(!findStart && currentP.x() >= timePoint.x())
            {
                startIndex = i;
                findStart = true;
            }

            if(findStart && currentP.x() >= timePoint.y())
            {
                endIndex = i;
                nowIndex = i;
                qDebug()<<"find start and end point"<<startIndex<<endIndex;
                waveSaveRange<<QCPDataRange(startIndex,endIndex);
                break;
            }
        }

        if(waveSaveRange.size() == waveInfo.waveCount)
            break;
    }

    if(waveSaveRange.size() < waveInfo.waveCount && startIndex < endIndex-2)
    {
        waveSaveRange<<QCPDataRange(startIndex,endIndex);
    }

    foreach (QCPDataRange range, waveSaveRange) {
        qDebug()<<"handle data range is"<<range.begin()<<range.end();
    }



    foreach (QCPGraphWave* waveLine, this->portToGraphMap.values()) {
        waveLine->setSaveWaveSegments(waveSaveRange);
    }
}

QMap<QString, AxisLabelInfo> QCPChartWidget::getAxisLabelInfos()
{
    QMap<QString, AxisLabelInfo> ret;
    AxisLabelInfo xAxis;
    AxisLabelInfo yAxis;
    AxisLabelInfo rAxis;
    xAxis.range = QPointF( customPlot->xAxis->range().lower ,
                           customPlot->xAxis->range().upper);
    xAxis.axisTitle = customPlot->xAxis->label();
    xAxis.bigTickCount = customPlot->xAxis->ticker()->tickCount();
    xAxis.axisLabelTypeList = getTimeLabelTypeTranslateList();
    xAxis.currentLabelType = getTimeLabelTypeTranslate(timeType);


    yAxis.axisTitle = customPlot->yAxis->label();
    yAxis.range = QPointF( customPlot->yAxis->range().lower ,
                           customPlot->yAxis->range().upper);
    yAxis.bigTickCount = customPlot->yAxis->ticker()->tickCount();
    yAxis.axisLabelTypeList = getTemperatureLabelTypeTranslateList();
    yAxis.currentLabelType = getTemperatureLabelTypeTranslate(tempType);


    rAxis.axisTitle = customPlot->axisRect()->axis(QCPAxis::atBottom,1)->label();
    rAxis.range = QPointF( customPlot->axisRect()->axis(QCPAxis::atBottom,1)->range().lower ,
                           customPlot->axisRect()->axis(QCPAxis::atBottom,1)->range().upper);
    rAxis.bigTickCount = customPlot->axisRect()->axis(QCPAxis::atBottom,1)->ticker()->tickCount();
    rAxis.axisLabelTypeList = getDistanceLabelTypeTranslateList();
    rAxis.currentLabelType = getDistanceLabelTypeTranslate(disType);


    ret.insert("X",xAxis);
    ret.insert("Y",yAxis);
    ret.insert("D",rAxis);

    return ret;
}

void QCPChartWidget::setAxisLabelInfo(const QMap<QString, AxisLabelInfo> &info)
{
    AxisLabelInfo xAxis = info.value("X");
    customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    customPlot->xAxis->ticker()->setTickCount(xAxis.bigTickCount);
    customPlot->xAxis->setRange(xAxis.range.x(),xAxis.range.y());
    customPlot->xAxis->setLabel(xAxis.axisTitle);

    AxisLabelInfo yAxis = info.value("Y");
    customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    customPlot->yAxis->ticker()->setTickCount(yAxis.bigTickCount);
    customPlot->yAxis->setRange(yAxis.range.x(),yAxis.range.y());
    customPlot->xAxis->setLabel(yAxis.axisTitle);

    AxisLabelInfo rAxis = info.value("D");
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->ticker()->setTickCount(rAxis.bigTickCount);
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setRange(rAxis.range.x(),rAxis.range.y());
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setLabel(rAxis.axisTitle);

    customPlot->replot(QCustomPlot::rpImmediateRefresh);
}

void QCPChartWidget::addOneSampleData(const QByteArray &onePieceData,
                                      const double &time)
{
    this->curveData += onePieceData;
    this->tempTime = time;
    auto dataList = RSMUsb::parserFileDataToRecordList(portNum,onePieceData);
    qDebug()<<"record count num is"<<dataList.count();
    foreach (QList<double> portValueList, dataList) {
        addOneSampleData(portValueList,tempTime);
         this->tempTime += this->sampleInteral;
    }

    reFit();
}

void QCPChartWidget::addOneSampleData(const QList<qreal> &portDataList,
                                      const double &time)
{

    const QStringList colorList = {
            "#00007f","#0000ff","#007f00","#007f7f","#007fff","#00ff00",
 "#7f0000","#7f007f","#7f00ff","#7f7f00","#7f7f7f","#7f7fff","#7fff7f",
 "#ff0000","#ff007f","#ff00ff","#ff7f00","#ff7f7f","#ff7fff","#ffff7f"

        };

        for(int i=0; i< portNum && i< portDataList.size();i++)
        {
            QCPGraphWave* portGraph = portToGraphMap.value(i+1);
            if(!portGraph)
            {
                portGraph = new QCPGraphWave(customPlot->xAxis, customPlot->yAxis);
                portGraph->setName(QLatin1String("Graph ")+QString::number(customPlot->graphCount()));
                portToGraphMap.insert(i+1,portGraph);

                portGraph->setInWaveMode(drawWave);

                portGraph->setPen(QPen(colorList[i]));
                portGraph->setName(tr("Port %1").arg(i+1));
                portGraph->rescaleAxes();

                colorLengend->setPortColor(i,portGraph->pen().color().name());
                colorLengend->setPortVisualObject(i,portGraph);
            }

            portGraph = portToGraphMap.value(i+1,0);
            portGraph->addData(time,portDataList.value(i,0));
        }


}

void QCPChartWidget::changeTimeLabelType(int type)
{
    this->timeType = type;

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    QSharedPointer<QCPAxisTickerOffsetDateTime> dateTimeTicker (new QCPAxisTickerOffsetDateTime);
    switch (type) {
    case TimeLabel_S:
        timeTicker->setTimeFormat("%s");
        customPlot->xAxis->setTicker(timeTicker);
        break;
    case TimeLabel_MS:
        timeTicker->setTimeFormat("%m:%s");
        customPlot->xAxis->setTicker(timeTicker);
        break;
    case TimeLabel_HMS:
        timeTicker->setTimeFormat("%h:%m:%s");
        customPlot->xAxis->setTicker(timeTicker);
        break;
    case TimeLabel_Time:
        dateTimeTicker->setDateTimeFormat("Thh:mm:ss");
//        dateTimeTicker->setDateTimeSpec(Qt::LocalTime);
        qDebug()<<"startDateTime"<<this->startDateTime;
        dateTimeTicker->setOffset(this->startDateTime);
        customPlot->xAxis->setTicker(dateTimeTicker);
        break;
    case TimeLabel_DateTime:
        dateTimeTicker->setDateTimeFormat("yyyy/MM/dd hh:mm:ss");
//        dateTimeTicker->setDateTimeSpec(Qt::LocalTime);
        dateTimeTicker->setTickOrigin(this->startDateTime);
        customPlot->xAxis->setTicker(dateTimeTicker);
        break;
    default:
        timeTicker->setTimeFormat("%s");
        break;
    }

//    xLabelUnit->setText(getTimeLabelTypeToString(type).split("_").last());

    customPlot->xAxis->setLabel(
                tr("时间:")
                +getTimeLabelTypeToString(timeType).split("_").last());

    replot();

    emit this->timeTypeChanged(this->timeType);
}

void QCPChartWidget::changeTempLabelType(int type)
{
    qDebug()<<"QCPChartWidget::changeTempLabelType"<<QCPChartWidget::getTemperatureLabelTypeToString(type);

    this->tempType = type;

    QSharedPointer<QCPAxisTicker> normalTicker(new QCPAxisTicker);
    QSharedPointer<QCPAxisTickerScaleAndOffset> scaleAndOffsetTicker(new QCPAxisTickerScaleAndOffset);
    switch (type) {
    case TemperatureLabel_C:
        customPlot->yAxis->setTicker(normalTicker);
        break;
    case TemperatureLabel_F:
        scaleAndOffsetTicker->setScaleRatio(1.8);
        scaleAndOffsetTicker->setOffset(32);
        customPlot->yAxis->setTicker(scaleAndOffsetTicker);
        break;
    case TemperatureLabel_K:
        scaleAndOffsetTicker->setScaleRatio(1);
        scaleAndOffsetTicker->setOffset(273.15);
        customPlot->yAxis->setTicker(scaleAndOffsetTicker);
        break;
    default:
        break;
    }

//    yLabelUnit->setText(getTemperatureLabelTypeToString(type).split("_").last());

    customPlot->yAxis->setLabel(
                tr("温度:")
                +getTemperatureLabelTypeToString(tempType).split("_").last());
    replot();

    emit this->tempTypeChanged(this->tempType);
}

void QCPChartWidget::changeDistanceLabelType(int type)
{
    qDebug()<<"QCPChartWidget::changeTempLabelType"<<QCPChartWidget::getDistanceLabelTypeToString(type);

    this->disType = type;

    QSharedPointer<QCPAxisTicker> normalTicker(new QCPAxisTicker);
    QSharedPointer<QCPAxisTickerScaleAndOffset> scaleAndOffsetTicker(new QCPAxisTickerScaleAndOffset);
    switch (type) {
    case DistanceLabel_M:
        customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->setTicker(normalTicker);
        break;
    case DistanceLabel_Cm:
        scaleAndOffsetTicker->setScaleRatio(100);
        customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->setTicker(scaleAndOffsetTicker);
        break;
    case DistanceLabel_Inch:
        scaleAndOffsetTicker->setScaleRatio(39.3700787);
        customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->setTicker(scaleAndOffsetTicker);
        break;
    case DistanceLabel_Feet:
        scaleAndOffsetTicker->setScaleRatio(3.2808399);
        customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->setTicker(scaleAndOffsetTicker);
        break;
    default:
        break;
    }

//    rLabelUnit->setText(getDistanceLabelTypeToString(type).split("_").last());

    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setLabel(
                tr("距离:")
                +getDistanceLabelTypeToString(disType).split("_").last());

    replot();
}

void QCPChartWidget::replot()
{
    customPlot->replot();
}

void QCPChartWidget::moveToTime(double time)
{
    if(!autoScroll)
        return;

    qreal y = HLine->point1->coords().y();

    HLine->point1->setCoords(0.5 * time, y);
    HLine->point2->setCoords(  2 * time, y);

    VLine->point1->setCoords(time, 0.5 * y);
    VLine->point2->setCoords(time,   2 * y);

    mCenterTracer->position->setCoords(time, y);

    qreal rPixel = customPlot->axisRect()->axis(QCPAxis::atBottom, 0)->coordToPixel(time);
    qreal r = customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->pixelToCoord(rPixel);

    //refresh  axis values
    xAxisTracer->updatePosition(time,1);
    xAxisTracer->setText(customPlot->xAxis->ticker()->getTickLabel(time,customPlot->locale(),'d',1));


    yAxisTracer->updatePosition(0,y);
    yAxisTracer->setText(customPlot->yAxis->ticker()->getTickLabel(y,customPlot->locale(),'d',1));

    rAxisTracer->updatePosition(time,1);
    rAxisTracer->setText(customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->ticker()->getTickLabel(r,customPlot->locale(),'f',1));


    //refresh port values
    foreach (QCPGraphWave* graph, portToGraphMap.values()) {
        double value = 100.0;
        auto iter = graph->data()->findBegin(time);
        value = iter->mainValue();
        graph->setName(QString("%1:%2").arg(portToGraphMap.key(graph))
                       .arg(QString::number(value,'f',1)));
    }
    customPlot->replot();
}

void QCPChartWidget::turnAutoScrollOn()
{
    autoScroll = true;
}

void QCPChartWidget::onMouseMove(QMouseEvent *event)
{
    QPoint mousePoint = event->pos();

    qreal x = customPlot->xAxis->pixelToCoord(mousePoint.x());
    qreal y = customPlot->yAxis->pixelToCoord(mousePoint.y());
    qreal r = customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->pixelToCoord(mousePoint.x());
//    qDebug()<<"chart mouse move"
//           <<"mouse pos"<<mousePoint
//            <<"axis pos"<<QPoint(x,y);

    //refresh indicate lines
    if(!customPlot->xAxis->range().contains(x)
       ||!customPlot->yAxis->range().contains(y) )
        return;


    autoScrollTimer.stop();
    autoScroll = false;

    HLine->point1->setCoords(0.5 * x, y);
    HLine->point2->setCoords(  2 * x, y);

    VLine->point1->setCoords(x, 0.5 * y);
    VLine->point2->setCoords(x,   2 * y);

    mCenterTracer->position->setCoords(x, y);

    //refresh  axis values
    xAxisTracer->updatePosition(x,1);
    xAxisTracer->setText(customPlot->xAxis->ticker()->getTickLabel(x,customPlot->locale(),'d',1));


    yAxisTracer->updatePosition(0,y);
    yAxisTracer->setText(customPlot->yAxis->ticker()->getTickLabel(y,customPlot->locale(),'d',1));

    rAxisTracer->updatePosition(x,1);
    rAxisTracer->setText(customPlot->axisRect()->axis(QCPAxis::atBottom, 1)->ticker()->getTickLabel(r,customPlot->locale(),'f',1));

//    xAxisTracer->setVisible(false);
//    yAxisTracer->setVisible(false);
//    rAxisTracer->setVisible(false);
    //refresh port values
    foreach (QCPGraphWave* graph, portToGraphMap.values()) {
        double value = 100.0;
        auto iter = graph->data()->findBegin(x);
        value = iter->mainValue();
        graph->setName(QString("%1:%2").arg(portToGraphMap.key(graph))
                       .arg(QString::number(value,'f',1)));
    }

    emit this->currentTimeChanged(x);

    customPlot->replot(QCustomPlot::rpQueuedReplot);

    autoScrollTimer.start(2000);
}

void QCPChartWidget::onTimeRangeChanged(QCPRange range)
{
    range.lower *= velocity;
    range.upper *= velocity;
    customPlot->axisRect()->axis(QCPAxis::atBottom,1)->setRange(range);
}

void QCPChartWidget::onAxisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart parts, QMouseEvent *event)
{
    qDebug()<<"QCPChartWidget::onAxisDoubleClick"<<axis<<parts<<event;

    if(axis == customPlot->xAxis)
    {
        changeTimeLabelType((this->timeType+1)%4);
    }
    else if(axis == customPlot->yAxis)
    {
        changeTempLabelType((this->tempType+1)%3);
    }
    else if(axis == customPlot->axisRect()->axis(QCPAxis::atBottom,1))
    {
        changeDistanceLabelType((this->disType+1)%4);
    }
}

void QCPChartWidget::onLineVisibleChanged(QCPGraph *line, bool vi)
{
    foreach (QCPGraph* wave, lineToWaveListMap.value(line)) {
        QColor c = line->pen().color();
        c.setAlpha(vi?20:0);
        wave->setBrush(QBrush(c));
        wave->setVisible(vi);


//        if(vi && !)
//        {
//            customPlot->legend->addItem(graphToLengendItemMap.value(line));
//        }
//        else if(!vi && customPlot->legend->itemWithPlottable(wave)){
//            customPlot->legend->removeItem(graphToLengendItemMap.value(line));
//        }
    }

    if(customPlot->legend->itemWithPlottable(line))
        customPlot->legend->itemWithPlottable(line)->setVisible(vi);

}

bool QCPChartWidget::getDrawWave() const
{
    return drawWave;
}

void QCPChartWidget::setDrawWave(bool value)
{
    drawWave = value;

    foreach (QCPGraphWave* waveLine, portToGraphMap.values()) {
        waveLine->setInWaveMode(value);
    }

    this->replot();
}

QList<QPointF> QCPChartWidget::getWaveRangeList() const
{
    return waveRangeList;
}

void QCPChartWidget::appendWaveDef(const QPointF &value)
{

    if(!waveinfo.useHandle)
    {
        waveRangeList << value;
        foreach (QCPGraphWave * line, portToGraphMap.values()) {
            line->setWaveRangeList(waveRangeList);
        }

    }

    //redraw wave section
    // add the bracket at the top:
    foreach (QCPAbstractItem* item, m_waveSections) {
        this->customPlot->removeItem(item);
//        delete item;
    }

    int waveCount = this->getWaveRangeList().size();
    if(!waveCount)
        return;
    QVector<QPointF> waveSectionVector;
    waveSectionVector.resize(waveCount);
    for(int i = 0; i < waveCount;i++)
    {
        foreach (int portNo, portToGraphMap.keys()) {
            QVector<QPointF> portWaveData = getPortWaveData(portNo,i);

            if(portWaveData.isEmpty())
                continue;

            waveSectionVector[i].setX(1e10);
            waveSectionVector[i].setY(-1e10);

            double start = portWaveData.first().x();
            double end = portWaveData.last().x();

            if(start < waveSectionVector[i].x())
                waveSectionVector[i].setX(start);
            if(end > waveSectionVector[i].y())
                waveSectionVector[i].setY(end);

        }
    }

    int i = 1;

    waveinfo.waveTimeSection.clear();

    foreach (QPointF point, waveSectionVector) {

        qDebug()<<i<<point;
        if(point.isNull())
            break;
        waveinfo.waveTimeSection[i] = point;

        QCPItemBracket *bracket = new QCPItemBracket(customPlot);
        bracket->left->setTypeY(QCPItemPosition::ptAxisRectRatio);
        bracket->right->setTypeY(QCPItemPosition::ptAxisRectRatio);
        bracket->left->setCoords(point.y(), 0.9);
        bracket->right->setCoords(point.x(), 0.9);
        bracket->setLength(13);
        bracket->setPen(QPen(QColor(0,0,0,50)));

        m_waveSections<<bracket;

        // add the text label at the top:
        QCPItemText *wavePacketText = new QCPItemText(customPlot);
        wavePacketText->position->setParentAnchor(bracket->center);
        wavePacketText->position->setCoords(0, -10); // move 10 pixels to the top from bracket center anchor
        wavePacketText->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        QString name = waveinfo.waveNameMap.value(i);
        if(name.isEmpty())
            name = tr("波段 %1").arg(i);
        wavePacketText->setText(name);
        wavePacketText->setFont(QFont(font().family(), 10));

        m_waveSections<<wavePacketText;

        i++;
    }

    waveinfo.waveCount = waveSectionVector.size();


    this->replot();
}

void QCPChartWidget::setWaveRangeList(const QList<QPointF> &value)
{
    waveRangeList = value;
    if(!waveinfo.useHandle)
    {
        foreach (QCPGraphWave * line, portToGraphMap.values()) {
            line->setWaveRangeList(value);
        }
    }
}

inline QVector<QPointF> getDataFromRange(const QVector<QPointF>& data,const  QCPDataRange& dataRange)
{
    QVector<QPointF> ret;
    for(int i=0;i< data.size();i++)
    {
        if( i >= dataRange.begin() )
        {
            if( i>dataRange.end() )
                break;
            ret<<data.at(i);
        }
    }

    return ret;
}

QVector<QPointF> QCPChartWidget::getPortWaveData(int portNo, int waveNo)
{
    QVector<QPointF> ret;
    QCPGraphWave* line = portToGraphMap.value(portNo);
    if(line)
    {
        QVector<QPointF> tempRet = getPointData(line);
        QList<QCPDataRange> waveRangeList = line->getSaveWaveSegments() ;
        if(waveRangeList.size() <= waveNo)
            return ret;
        QCPDataRange waveRange = waveRangeList.at(waveNo);
        ret = ::getDataFromRange(tempRet,waveRange);
    }
    qDebug()<<"QCPChartWidget::getPortWaveData size"<<ret.size();
    return ret;
}

bool QCPChartWidget::getInFullRange() const
{
    return inFullRange;
}

void QCPChartWidget::setInFullRange(bool value)
{
    inFullRange = value;
}

//void QCPChartWidget::drawWave(const QMap<int, QList<QPointF> > &portWaveEndPointListMap)
//{
//    return;
////    qDebug()<<"portWaveEndPointListMap"<<portWaveEndPointListMap<<portToGraphMap;

//    foreach (QCPGraphWave* dataLine, portToGraphMap.values()) {

//        lineToWaveListMap[dataLine].clear();

//        QColor color = dataLine->pen().color();
//        color.setAlpha(20);

//        int portNum = portToGraphMap.key(dataLine);

//        QList<QPointF> wavePointList = portWaveEndPointListMap[portNum];

//        qDebug()<<"port"<<portNum<<wavePointList;

//        if(wavePointList.size() > 3 || !wavePointList.size())
//            continue;

//        foreach (QPointF point, wavePointList) {
//            QCPGraph * waveGraph1= customPlot->addGraph();

//            waveGraph1->addData(point.x(),0);
//            waveGraph1->addData(point.y(),0);

//            waveGraph1->setBrush(color);
//            waveGraph1->setChannelFillGraph(dataLine);

//            lineToWaveListMap[dataLine]<<waveGraph1;
//        }

//    }

//    customPlot->replot();
//}

/* end of 'src/axis/axistickertime.cpp' */


QString QCPChartWidget::getTimeValueToTypeLabel(double value, int timeType)
{
//    qDebug()<<"QCPChartWidget::getTimeValueToTypeLabel"<<value<<timeType;
    QString ret;
    static QCPAxisTickerTime timeTicker;
    switch (timeType) {
    case QCPChartWidget::TimeLabel_S:
        timeTicker.setTimeFormat("%s");
        ret = timeTicker.getTickLabel(value,QLocale("zh_CN"),'a',3);
        break;
    case QCPChartWidget::TimeLabel_MS:
        timeTicker.setTimeFormat("%m:%s");
        ret = timeTicker.getTickLabel(value,QLocale("zh_CN"),'a',3);
        break;
    case QCPChartWidget::TimeLabel_HMS:
        timeTicker.setTimeFormat("%h:%m:%s");
        ret = timeTicker.getTickLabel(value,QLocale("zh_CN"),'a',3);
        break;
    case QCPChartWidget::TimeLabel_Time:
        ret = QDateTime::fromTime_t(value).toString("Thh:mm:ss");
        break;
    default:
        break;
    }

//    qDebug()<<"QCPChartWidget::getTimeValueToTypeLabel result is"<<ret;

    return ret;
}


QByteArray QCPChartWidget::getCurveData() const
{
    return curveData;
}

void QCPChartWidget::addLine(const LineInfo &line)
{
    QCPItemStraightLine* sline = new QCPItemStraightLine(customPlot);
    QCPItemText* sText = new QCPItemText(customPlot);
    switch (line.type) {
    case TopTest::HLine:
        sline->point1->setCoords(-100, line.value);
        sline->point2->setCoords( 100, line.value);
        sText->setLayer("addLayout");
        sText->setColor(Qt::red);
        sText->setText(QString::number(line.value,'f',1));
        sText->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
        sText->position->setCoords(0.95,line.value);
        break;
    case TopTest::VLine:
        sline->point1->setCoords(line.value, -100);
        sline->point2->setCoords(line.value, 100);
        sText->setLayer("addLayout");
        sText->setColor(Qt::red);
        sText->setText(QString::number(line.value,'f',1));
        sText->position->setTypeY(QCPItemPosition::ptAxisRectRatio);
        sText->position->setCoords(line.value,0.05);
        break;
    case TopTest::TwoPointLine:

        break;
    }

    m_HVLines<<sline;
    m_HVLines<<sText;
    customPlot->replot(QCustomPlot::rpImmediateRefresh);
}

void QCPChartWidget::clearAllLine()
{
    foreach (QCPAbstractItem* sline, m_HVLines) {
        customPlot->removeItem(sline);
    }

    m_HVLines.clear();

    customPlot->replot(QCustomPlot::rpImmediateRefresh);

}

double QCPChartWidget::getVelocity() const
{
    return velocity;
}

void QCPChartWidget::setVelocity(double value)
{
    velocity = value;
}

void QCPChartWidget::changeToolTipEnable()
{
    ToolTipEnable = !ToolTipEnable;

    customPlot->legend->setVisible(ToolTipEnable);
//    HLine->setVisible(ToolTipEnable);
//    VLine->setVisible(ToolTipEnable);
//    Pos->setVisible(ToolTipEnable);
//    xAxisTracer->setVisible(ToolTipEnable);
//    yAxisTracer->setVisible(ToolTipEnable);
//    rAxisTracer->setVisible(ToolTipEnable);

    customPlot->replot();
}

void QCPChartWidget::changeTimeFitType()
{
    inFullRange = !inFullRange;
    customPlot->replot();
}

void QCPChartWidget::reDrawTempSection(const TopTest::TempSection &tempSection)
{
    ///remove old section lines
    foreach (QCPAbstractItem* line, m_tempSectionSeries) {
        if(line)
            customPlot->removeItem(line);
    }
    m_tempSectionSeries.clear();


    ///draw new lines

    if(tempSection.sections.isEmpty())
        return;
    QList<double> timeList;

    timeList<<tempSection.sections.first().startTime;
    foreach (TempSectionItem item, tempSection.sections) {
        timeList<<item.endTime;
        QCPItemLine* upTempLine = new QCPItemLine(customPlot);
        upTempLine->start->setCoords( item.startTime,item.upValue);
        upTempLine->end->setCoords( item.endTime,item.upValue);
        m_tempSectionSeries<<upTempLine;

        qDebug()<<"tempSection.isDoubleSection"<<tempSection.isDoubleSection;
        if(!tempSection.isDoubleSection)
        {
            qDebug()<<"draw down line";
            QCPItemLine* downTempLine = new QCPItemLine(customPlot);
            downTempLine->start->setCoords( item.startTime,item.downValue);
            downTempLine->end->setCoords( item.endTime,item.downValue);
            m_tempSectionSeries<<downTempLine;
        }
    }

    foreach (double time, timeList) {
        QCPItemStraightLine * timeLine = new QCPItemStraightLine(customPlot);
        timeLine->point1->setTypeY(QCPItemPosition::ptAxisRectRatio);
        timeLine->point2->setTypeY(QCPItemPosition::ptAxisRectRatio);
        timeLine->point1->setCoords( time, 0);
        timeLine->point2->setCoords( time, 1);
        m_tempSectionSeries<<timeLine;
    }

    foreach (QCPAbstractItem* line, m_tempSectionSeries) {

        line->setVisible(showTempSection);
    }

    customPlot->replot();
}

void QCPChartWidget::changeTempSectionVisiable()
{
    showTempSection = !showTempSection;
    foreach (QCPAbstractItem* line, m_tempSectionSeries) {

        line->setVisible(showTempSection);
    }

    customPlot->replot();
}

QString QCPChartWidget::getSourceDataString()
{
    QString str;

    if(portToGraphMap.isEmpty())
        return "No data";

    QCPGraph* firstLine = portToGraphMap.values().first();
    if(!firstLine || !firstLine->dataCount())
        return "No point";

    QList<QVector<QPointF>> portDataVectorList;
    foreach (int portNo, portToGraphMap.keys()) {
        portDataVectorList<<getPortPointFVector(portNo);
    }

    QDateTime printDateTime = startDateTime;

    if(printDateTime.isNull())
        printDateTime = QDateTime::currentDateTime();

    qDebug()<<"sampleTime"<<sampleInteral<<"testDateTime"<<printDateTime;

    for(int j =0 ; j< firstLine->dataCount(); j++)
    {
        str += printDateTime.date().toString("yyyy/MM/dd")
                +","+printDateTime.time().toString("hh:mm:ss");
        for(int i = 0; i< portNum ; i++)
        {
            QPointF dataPoint = portDataVectorList.at(i).at(j);
            str += "," + QString::number(dataPoint.y());
        }

        str += "\n";
        printDateTime = printDateTime.addMSecs(sampleInteral*1000.0);
    }

    return str;
}

int QCPChartWidget::getTimeType() const
{
    return timeType;
}

void QCPChartWidget::setTimeType(const TimeLabelType &value)
{
    timeType = value;
}

QDateTime QCPChartWidget::getStartDateTime() const
{
    return startDateTime;
}

double QCPChartWidget::getSampleInteral() const
{
    return sampleInteral;
}

int QCPChartWidget::getPortNum() const
{
    return portNum;
}

FormColorLengend *QCPChartWidget::getColorLengend() const
{
    return colorLengend;
}

void QCPChartWidget::setColorLengend(FormColorLengend *value)
{
    colorLengend = value;

    connect(colorLengend,SIGNAL(needReplot()),
            this,SLOT(replot()));
    connect(colorLengend,SIGNAL(lineVisibleChanged(QCPGraph*,bool)),
            this,SLOT(onLineVisibleChanged(QCPGraph*,bool)));
}

QMap<int, QPair<QString, QColor> > QCPChartWidget::getPortInfoMap()
{
    return colorLengend->getPortInfoMap();
}

void QCPChartWidget::setPortInfos(const QMap<int, QPair<QString, QColor> > &portInfoMap)
{
    foreach (int port, portInfoMap.keys()) {
        QPair<QString, QColor> info = portInfoMap[port];

        QCPGraph* portSerires =  portToGraphMap[port];
        if(!portSerires)
        {
            portSerires = customPlot->addGraph();
//            colorLengend->setPortVisualObject(port-1,portSerires);
        }

        portSerires->setPen(QPen(info.second));
        foreach (QCPGraph* wave, lineToWaveListMap.value(portSerires)) {
            QColor c = info.second;
            c.setAlpha(wave->brush().color().alpha());
            wave->setBrush(QBrush(c));
        }

        //apply legend
        colorLengend->setPortName(port-1, info.first);
        colorLengend->setPortColor(port-1, portSerires->pen().color().name());
    }
}

QCPAxisTickerScaleAndOffset::QCPAxisTickerScaleAndOffset()
{

}

double QCPAxisTickerScaleAndOffset::getScaleRatio() const
{
    return scaleRatio;
}

void QCPAxisTickerScaleAndOffset::setScaleRatio(double value)
{
    scaleRatio = value;
}

double QCPAxisTickerScaleAndOffset::getOffset() const
{
    return offset;
}

void QCPAxisTickerScaleAndOffset::setOffset(double value)
{
    offset = value;
}

double QCPAxisTickerScaleAndOffset::getTickStep(const QCPRange &range)
{
    double tickStep = range.size()/*/scaleRatio*//(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
//    double tickStep = range.size()/scaleRatio/(double)(mTickCount+1e-10); // mTickCount ticks on average, the small addition is to prevent jitter on exact integers
    tickStep = cleanMantissa(tickStep * scaleRatio);
//    qDebug()<<"scale step is" <<tickStep <<"tickStep is "<<tickStep /scaleRatio;
    return tickStep /scaleRatio;
}

QString QCPAxisTickerScaleAndOffset::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    return QCPAxisTicker::getTickLabel(tick * scaleRatio + offset ,locale, formatChar, precision);
}

QVector<double> QCPAxisTickerScaleAndOffset::createTickVector(double tickStep, const QCPRange &range)
{
//    return QCPAxisTicker::createTickVector(tickStep,range);

    QVector<double> result;
    // Generate tick positions according to tickStep:
    qint64 firstStep = floor((range.lower-mTickOrigin + offset)/tickStep); // do not use qFloor here, or we'll lose 64 bit precision
    qint64 lastStep = ceil((range.upper-mTickOrigin + offset)/tickStep); // do not use qCeil here, or we'll lose 64 bit precision
    int tickcount = lastStep-firstStep+1;
    if (tickcount < 0) tickcount = 0;
    result.resize(tickcount);
    for (int i=0; i<tickcount; ++i)
      result[i] = -offset/scaleRatio + mTickOrigin + (firstStep+i)*tickStep;
//    qDebug()<<"firstStep and last step is"<<firstStep<<lastStep;
    return result;
}


//int QCPAxisTickerScaleAndOffset::getSubTickCount(double tickStep)
//{
//  return QCPAxisTicker::getSubTickCount(tickStep/mPiValue);
//}

//QString QCPAxisTickerScaleAndOffset::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
//{

//}

QCPAxisTickerOffsetDateTime::QCPAxisTickerOffsetDateTime()
{

}

void QCPAxisTickerOffsetDateTime::setOffset(const QDateTime &dateTime)
{
    setOffset(dateTime.toSecsSinceEpoch());
}

qint64 QCPAxisTickerOffsetDateTime::getOffset() const
{
    return offset;
}

void QCPAxisTickerOffsetDateTime::setOffset(const qint64 &value)
{
    offset = value;
}

//double QCPAxisTickerOffsetDateTime::getTickStep(const QCPRange &range)
//{

//}

QString QCPAxisTickerOffsetDateTime::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    Q_UNUSED(precision)
    Q_UNUSED(formatChar)
    return QCPAxisTickerDateTime::getTickLabel(tick+offset,locale,formatChar,precision);
}

//QVector<double> QCPAxisTickerOffsetDateTime::createTickVector(double tickStep, const QCPRange &range)
//{
//    QVector<double> result = QCPAxisTicker::createTickVector(tickStep, range);
//    if (!result.isEmpty())
//    {
//      if (mDateStrategy == dsUniformTimeInDay)
//      {
//        QDateTime uniformDateTime = keyToDateTime(mTickOrigin); // the time of this datetime will be set for all other ticks, if possible
//        QDateTime tickDateTime;
//        for (int i=0; i<result.size(); ++i)
//        {
//          tickDateTime = keyToDateTime(result.at(i));
//          tickDateTime.setTime(uniformDateTime.time());
//          result[i] = dateTimeToKey(tickDateTime);
//        }
//      } else if (mDateStrategy == dsUniformDayInMonth)
//      {
//        QDateTime uniformDateTime = keyToDateTime(mTickOrigin); // this day (in month) and time will be set for all other ticks, if possible
//        QDateTime tickDateTime;
//        for (int i=0; i<result.size(); ++i)
//        {
//          tickDateTime = keyToDateTime(result.at(i));
//          tickDateTime.setTime(uniformDateTime.time());
//          int thisUniformDay = uniformDateTime.date().day() <= tickDateTime.date().daysInMonth() ? uniformDateTime.date().day() : tickDateTime.date().daysInMonth(); // don't exceed month (e.g. try to set day 31 in February)
//          if (thisUniformDay-tickDateTime.date().day() < -15) // with leap years involved, date month may jump backwards or forwards, and needs to be corrected before setting day
//            tickDateTime = tickDateTime.addMonths(1);
//          else if (thisUniformDay-tickDateTime.date().day() > 15) // with leap years involved, date month may jump backwards or forwards, and needs to be corrected before setting day
//            tickDateTime = tickDateTime.addMonths(-1);
//          tickDateTime.setDate(QDate(tickDateTime.date().year(), tickDateTime.date().month(), thisUniformDay));
//          result[i] = dateTimeToKey(tickDateTime);
//        }
//      }
//    }
//    return result;
//}

QCPGraphWave::QCPGraphWave(QCPAxis *keyAxis, QCPAxis *valueAxis)
    :QCPGraph(keyAxis,valueAxis)
{

//    this->waveRangeList.append(QPointF(-300,-300));
//    this->waveRangeList.append(QPointF(160.0,50.0));

    this->getWaveSegments(this->saveWaveSegments);
}

QCPGraphWave::~QCPGraphWave()
{

}

void filterScatters(QVector<QPointF>* scatters)
{
//     qDebug()<<"before filter scatters size is "<<scatters->size();
     for(int i = 1; i < scatters->size();)
     {
         QPointF p1 = scatters->at(i-1);
         QPointF p2 = scatters->at(i);
         QPointF disPoint = p2 - p1;

//         qDebug()<<disPoint<<disPoint.manhattanLength();
         if(disPoint.manhattanLength() < 15)
         {
//            qDebug()<<"go to remove "<<i;
            scatters->removeAt(i);
         }
         else
             i++;

         if(i >= scatters->size())
             break;
     }

//     qDebug()<<"after filter scatters size is "<<scatters->size();
}


void QCPGraphWave::getLinesWithOutgetOptimized(QVector<QPointF> *lines, const QCPDataRange &dataRange) const
{
  if (!lines) return;

  QVector<QCPGraphData> lineData;
  auto wholeData = this->data();
  for(int i=0;i< wholeData->size();i++)
  {
      if( i >= dataRange.begin() )
      {
          if( i>dataRange.end() )
              break;
          lineData<<*wholeData->at(i);
      }
  }

//  qDebug()<<"lineDataCount"<<lineData.size();


  if (mKeyAxis->rangeReversed() != (mKeyAxis->orientation() == Qt::Vertical)) // make sure key pixels are sorted ascending in lineData (significantly simplifies following processing)
    std::reverse(lineData.begin(), lineData.end());

  switch (mLineStyle)
  {
    case lsNone: lines->clear(); break;
    case lsLine: *lines = dataToLines(lineData); break;
    case lsStepLeft: *lines = dataToStepLeftLines(lineData); break;
    case lsStepRight: *lines = dataToStepRightLines(lineData); break;
    case lsStepCenter: *lines = dataToStepCenterLines(lineData); break;
    case lsImpulse: *lines = dataToImpulseLines(lineData); break;
  }

//  qDebug()<<"lines DataCount"<<lines->size();
}

QList<QPointF> QCPGraphWave::getWaveRangeList() const
{
    return waveRangeList;
}

void QCPGraphWave::setWaveRangeList(const QList<QPointF> &value)
{
    waveRangeList = value;
    this->getWaveSegments(this->saveWaveSegments);
}

QList<QCPDataRange> QCPGraphWave::getSaveWaveSegments() const
{
    return saveWaveSegments;
}

void QCPGraphWave::setSaveWaveSegments(const QList<QCPDataRange> &value)
{
    saveWaveSegments = value;
}

bool QCPGraphWave::getInWaveMode() const
{
    return inWaveMode;
}

void QCPGraphWave::setInWaveMode(bool value)
{
    inWaveMode = value;

    if(value)
    {
        this->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,2));
    }
    else
    {
        this->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    }

}

void QCPGraphWave::draw(QCPPainter *painter)
{
    if(!inWaveMode)
    {
        QCPGraph::draw(painter);
        return;
    }

    if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
    if (mKeyAxis.data()->range().size() <= 0 || mDataContainer->isEmpty()) return;
    if (mLineStyle == lsNone && mScatterStyle.isNone()) return;

    LineStyle lineStyle = lsLine;
    QVector<QPointF> lines, scatters; // line and (if necessary) scatter pixel coordinates will be stored here while iterating over segments
    // loop over and draw segments of unselected/selected data:
    QList<QCPDataRange> selectedSegments, unselectedSegments, allSegments, waveSegments;
    getDataSegments(selectedSegments, unselectedSegments);
    waveSegments = getSaveWaveSegments();
    allSegments << unselectedSegments << selectedSegments;
    for (int i=0; i<allSegments.size(); ++i)
    {
      bool isSelectedSegment = i >= unselectedSegments.size();
      // get line pixel points appropriate to line style:
      QCPDataRange lineDataRange = isSelectedSegment ? allSegments.at(i) : allSegments.at(i).adjusted(-1, 1); // unselected segments extend lines to bordering selected data point (safe to exceed total data bounds in first/last segment, getLines takes care)

      //get wave line and none wave line range list
      QList<QCPDataRange> realWaveRangeList;
      QList<QCPDataRange> noneWaveRangeList;

      for(int j = 0 ; j < waveSegments.size(); j++ )
      {
          QCPDataRange realLineDataRange = lineDataRange.intersection(waveSegments.at(j));
          if(realLineDataRange.isValid() && !realLineDataRange.isEmpty())
              realWaveRangeList<<realLineDataRange;
      }

//      qDebug()<<"real wave range list size is"<<realWaveRangeList.size();

      if(realWaveRangeList.isEmpty())
      {
          noneWaveRangeList<<lineDataRange;
      }
      else
      {
          QCPDataRange lastRange;
          for(int j = 0 ; j < realWaveRangeList.size(); j++ )
          {
              QCPDataRange tempRange;
              if(lastRange.isValid() && !lastRange.isEmpty() )
              {
                   tempRange.setBegin(lastRange.end());
              }
              else
              {
                   tempRange.setBegin(qMax(allSegments.at(i).begin(),0));
              }

              tempRange.setEnd(realWaveRangeList.at(j).begin());

//              qDebug()<<"temp range is"<<tempRange.begin()<<tempRange.end();

              if(tempRange.isValid() && !tempRange.isEmpty())
                  noneWaveRangeList<<tempRange;

              lastRange = realWaveRangeList.at(j);
          }

          if(!noneWaveRangeList.isEmpty())
          {
              if(lastRange.end() < lineDataRange.end())
              {
                  QCPDataRange tempRange(lastRange.end(),allSegments.at(i).end());
//                  qDebug()<<"temp range is"<<tempRange.begin()<<tempRange.end();
                  noneWaveRangeList<<tempRange;
              }
          }

      }

//      qDebug()<<"none wave range list size is"<<noneWaveRangeList.size();

      for(int j =0 ; j < noneWaveRangeList.size(); j++)
      {

          getLinesWithOutgetOptimized(&scatters, noneWaveRangeList.at(j));
          // draw scatters:
          QCPScatterStyle finalScatterStyle = mScatterStyle;
          if (isSelectedSegment && mSelectionDecorator)
            finalScatterStyle = mSelectionDecorator->getFinalScatterStyle(mScatterStyle);
          if (!finalScatterStyle.isNone())
          {
    //        getScatters(&scatters, allSegments.at(i));
            filterScatters(&scatters);
    //        qDebug()<<this->name()<<"scatters count is"<<scatters.size();
            drawScatterPlot(painter, scatters, finalScatterStyle);
          }

      }

      //plot lines in wave with line  and out with scatter
      //so adjust lineDataRange
      for(int j = 0 ; j < realWaveRangeList.size(); j++ )
      {

          QCPDataRange realLineDataRange = realWaveRangeList.at(j);

          getLinesWithOutgetOptimized(&lines, realLineDataRange);

          // check data validity if flag set:
      #ifdef QCUSTOMPLOT_CHECK_DATA
          QCPGraphDataContainer::const_iterator it;
          for (it = mDataContainer->constBegin(); it != mDataContainer->constEnd(); ++it)
          {
            if (QCP::isInvalidData(it->key, it->value))
              qDebug() << Q_FUNC_INFO << "Data point at" << it->key << "invalid." << "Plottable name:" << name();
          }
      #endif

          // draw fill of graph:
          if (isSelectedSegment && mSelectionDecorator)
            mSelectionDecorator->applyBrush(painter);
          else
            painter->setBrush(mBrush);
          painter->setPen(Qt::NoPen);
          drawFill(painter, &lines);

          // draw line:
          if (lineStyle != lsNone)
          {
            if (isSelectedSegment && mSelectionDecorator)
              mSelectionDecorator->applyPen(painter);
            else
              painter->setPen(mPen);
            painter->setBrush(Qt::NoBrush);
            if (lineStyle == lsImpulse)
              drawImpulsePlot(painter, lines);
            else
              drawLinePlot(painter, lines); // also step plots can be drawn as a line plot
          }
      }
    }

    // draw other selection decoration that isn't just line/scatter pens and brushes:
    if (mSelectionDecorator)
      mSelectionDecorator->drawDecoration(painter, selection());
}

#define sortNum 10

#define errorAbs 0.5

int findFirstValueUpAboveY(const QVector<QPointF>& line, double y)
{
//    if(line.size() < sortNum)
//        return -1;

//    bool up = false;
//    QVector<int> sigArray;
//    sigArray.push_back(line.at(i).y() - );

//    sigArray[0] = ;


    bool up = false;
    for(int i = 2 ; i < line.size(); i++)
    {

        double y1 = line.at(i-2).y();
        double y2 = line.at(i-1).y();
        double y3 = line.at(i).y();

        if( (y3-y2) >= 0 && (y2-y1) >= 0 )
            up = true;
        else
            up = false;


        if(up &&  y3 > y)
            return i;
    }
    return -1;
}

int findFirstValueDownBelowY(const QVector<QPointF>& line, double y, double downBelowHeap = 3)
{
    bool down = false;
    double partHeap = -200;
    for(int i = 2 ; i < line.size(); i++)
    {
        double y1 = line.at(i-2).y();
        double y2 = line.at(i-1).y();
        double y3 = line.at(i).y();

        if( y3 > partHeap)
            partHeap = y3;

        if( (y3-y2) < -errorAbs && (y2-y1) < -errorAbs /*&& (partHeap - y3) >= downBelowHeap*/ )
            down = true;
        else
            down = false;


        if(down &&  y3 < y )
            return i;
    }
    return -1;
}

void QCPGraphWave::getWaveSegments(QList<QCPDataRange> &waveSegments)
{
    waveSegments.clear();

    int currentIndex = 0;
    QVector<QPointF> line = getPointData(this);
    qDebug()<<"whole data size"<<line.size();

    foreach (QPointF waveDef, this->waveRangeList) {
        qDebug()<<"begin waveDef"<<"("<<waveDef.x()<<waveDef.y()<<")";
        QVector<QPointF> currentLine = line.mid(currentIndex, this->dataCount() - currentIndex);
        int beginIndex = findFirstValueUpAboveY(currentLine,waveDef.x());
        if(beginIndex == -1)
        {
            qDebug()<<"can't find begin index";
            break;
        }
        beginIndex += currentIndex;
        currentIndex = beginIndex;

        currentLine = line.mid(currentIndex, this->dataCount() - currentIndex);
        int endIndex = findFirstValueDownBelowY(currentLine,waveDef.y());
        if(endIndex == -1)
        {
            qDebug()<<"can't find end index";
            endIndex = currentLine.size()-1;
        }

        endIndex += currentIndex;
        currentIndex = endIndex;

        QCPDataRange waveSegment;
        waveSegment.setBegin(beginIndex);
        waveSegment.setEnd(endIndex);
        waveSegments<<waveSegment;

        qDebug()<<"find wave segment"<<beginIndex<<endIndex;

        if(currentIndex == line.size() -1 )
        {
            qDebug()<<"whole line is find, break now";
            break;
        }
    }

    qDebug()<<this->name()
            <<"current wave range list count is"<<this->waveRangeList.size()
            <<"current wave segments count is"<<waveSegments.size();
}
