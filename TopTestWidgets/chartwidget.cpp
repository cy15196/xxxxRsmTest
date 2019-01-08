/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "chartwidget.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QtCharts/QLegend>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QXYLegendMarker>
#include <QtCore/QtMath>

#include "mychartview.h"
#include "formcolorlengend.h"

#include "callout.h"


//#include "dataiomanager.h"
#include <QDir>
#include <QDataStream>


#include "dataprogresshandlealgorithm.h"
//#include "clabso_ft_io.h"
#include "rsmusb.h"

#include "toptestcore.h"


QT_CHARTS_USE_NAMESPACE

void changePointListRange(const QList<QPointF> & newPointList,QRectF & rangeRect)
{
    foreach (QPointF point , newPointList) {
        if(point.x()>rangeRect.right())
            rangeRect.setRight(point.x());
        else if(point.x()<rangeRect.left())
            rangeRect.setLeft(point.x());

        if(point.y()>rangeRect.bottom())
            rangeRect.setBottom(point.y());
        else if(point.y()<rangeRect.top())
            rangeRect.setTop(point.y());
    }
}

void changeDataRange(double time,
                     const QList<double>& portValueList,
                     QPointF& timeRange,
                     QPointF& dataRange)
{
    if(dataRange.x() == 0 && dataRange.y()==0)
    {
        dataRange.setX(portValueList.first());
        dataRange.setY(portValueList.first());
    }

    foreach (double value, portValueList) {
        if(value < dataRange.x())
            dataRange.setX(value);
        else if(value > dataRange.y())
            dataRange.setY(value);
    }

    timeRange.setY(time);
}

ChartWidget::ChartWidget(QWidget *parent) :
    QWidget(parent)
{
    showTempSection = false;
    // Create chart view with the chart
    m_chart = new QChart();
    m_chartView = new MyChartView(m_chart, this);
//    m_chartView->setRubberBand(QChartView::RectangleRubberBand);

    this->timeRange = QPointF(0,1);
    this->dataRange = QPointF(0,1);
    toolTipEnable = false;

    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView);
    setLayout(m_mainLayout);
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);

    m_chart->legend()->setVisible(false);
    m_chart->legend()->setAlignment(Qt::AlignRight);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    TopTest::AxisLabelInfo xLabelInfo;
    TopTest::AxisLabelInfo yLabelInfo;
    TopTest::AxisLabelInfo dLabelInfo;

    xLabelInfo.axisTitle = "Time";
    xLabelInfo.bigTickCount = 6;
    xLabelInfo.smallTickCountPerBigStep = 5;
    xLabelInfo.range = timeRange;

    yLabelInfo.axisTitle = "Temperature";
    yLabelInfo.bigTickCount = 6;
    yLabelInfo.smallTickCountPerBigStep = 5;
    yLabelInfo.range = dataRange;

    dLabelInfo.axisTitle = "Distance";
    dLabelInfo.bigTickCount = 10;
    dLabelInfo.smallTickCountPerBigStep = 5;
    dLabelInfo.range = QPointF(0,20);

    QMap<QString,TopTest::AxisLabelInfo> labelInfoMap;
    labelInfoMap["X"] = xLabelInfo;
    labelInfoMap["Y"] = yLabelInfo;
    labelInfoMap["D"] = dLabelInfo;


    m_chartView->setAxisLabelInfo(labelInfoMap);

    connect(m_chartView,SIGNAL(currentTimeChanged(double)),
            this,SIGNAL(currentTimeChanged(double)));

//    m_chartView->hide();
}

void ChartWidget::setPortLineSeries(const int &portNum, QLineSeries *series)
{
    portSeriresMap[portNum] = series;
    m_chart->addSeries(series);
    series->attachAxis(m_chart->axisX());
    series->attachAxis(m_chart->axisY());
    series->setName(QString("Port%1").arg(portNum));
//    if (portSeriresMap.count() > 0)
//    {
//        m_chart->createDefaultAxes();
//    }
}

void ChartWidget::addPortTimeData(const int &portNum, const QList<QPointF> &data)
{

//    CsLogs("addPortTimeData port:%1 pointCount:%2",portNum,data.count());

    if(!portSeriresMap[portNum])
        setPortLineSeries(portNum,new QLineSeries);
    portSeriresMap[portNum]->append(data);

//    changePointListRange(data,dataRange);
//    m_chart->axisX()->setRange(dataRange.left(),dataRange.right());
//    m_chart->axisY()->setRange(dataRange.top(),dataRange.bottom());

//    DataProgressHandleAlgorithm::instance()->setInput(data);
//    QMap<QString,double> paramters;
//    paramters.insert("TemperatureMinValue",200.0);
//    paramters.insert("TemperatureMaxValue",750.0);
//    paramters.insert("TemperatureRemainValue",500);
//    DataProgressHandleAlgorithm::instance()->setParameters(paramters);

//    DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::IncreaseTime);
//    DataProgressHandleAlgorithm::instance()->update();

//    DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::AboveTime);
//    DataProgressHandleAlgorithm::instance()->update();

}

//void ChartWidget::addTimeData(const QMap<int, QList<QPointF> > & portDataMap)
//{
//    foreach (int portNum, portDataMap.keys()) {
//        addPortTimeData(portNum,portDataMap.value(portNum));
//    }
//}

void ChartWidget::addSeries()
{
    QLineSeries *series = new QLineSeries();
    m_series.append(series);

    series->setName(QString("line " + QString::number(m_series.count())));

    // Make some sine wave for data
    QList<QPointF> data;
    int offset = m_chart->series().count();
    for (int i = 0; i < 360; i++) {
        qreal x = offset * 20 + i;
        data.append(QPointF(i, qSin(2.0 * 3.141592 * x / 360.0)));
    }

    series->append(data);
//    QPen pen;

//    pen.setStyle(Qt::DashDotLine);
//      pen.setWidth(3);
////      pen.setBrush(Qt::green);
//      pen.setCapStyle(Qt::RoundCap);
//      pen.setJoinStyle(Qt::RoundJoin);

//    series->setPen(pen);
    m_chart->addSeries(series);
    QPen  pen = series->pen();
    pen.setStyle(Qt::DotLine);
//    pen.setDashOffset(5);
    series->setPen(pen);

//    if (m_series.count() == 1)
//        m_chart->createDefaultAxes();
}

void ChartWidget::removeSeries()
{
    // Remove last series from chart
    if (m_series.count() > 0) {
        QLineSeries *series = m_series.last();
        m_chart->removeSeries(series);
        m_series.removeLast();
        delete series;
    }
}

void ChartWidget::connectMarkers()
{
//![1]
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        marker->setShape(QLegend::MarkerShapeFromSeries);
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &ChartWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &ChartWidget::handleMarkerClicked);
    }
//![1]
}

void ChartWidget::disconnectMarkers()
{
//![2]
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &ChartWidget::handleMarkerClicked);
    }
//![2]
}

void ChartWidget::handleMarkerClicked()
{
//![3]
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void ChartWidget::reset()
{
    removeSeries();
}

void ChartWidget::loadDataFile(const QString &dataFile)
{
//    CsLogs("loadDataFile %1",dataFile);

//    TopTestFileDataMap fileDataMap = DataIOManager::readDataFile(dataFile);
//    TopTestChartDataMap chartDataMap = DataIOManager::convertFileDataToChartData(fileDataMap);

//    foreach (int port, chartDataMap.keys()) {
//        addPortTimeData(port,chartDataMap[port]);
    //    }
}

void ChartWidget::setCurveData(const QByteArray &dataArray)
{
    qDebug()<<"add curve data to chart "<<dataArray.length()<<"of port num = "<<this->portNum;
    this->curveData = dataArray;
    double time = 0.0;
    if(sampleTime == 0.0)
         sampleTime = 0.1;

    auto dataList = RSMUsb::parserFileDataToRecordList(portNum,dataArray);
    qDebug()<<"record count num is"<<dataList.count();
    foreach (QList<double> portValueList, dataList) {
        addOneSampleData(time,portValueList);
        time += this->sampleTime;
    }

//    qDebug()<<portSeriresMap.values();

//    m_chart->createDefaultAxes();

    //for test
//    applyProgressToPortData(0);

}

void ChartWidget::addCurveData(const double &sampleTime, const QByteArray &dataArray)
{
    this->curveData += dataArray;
    double time = sampleTime;
    auto dataList = RSMUsb::parserFileDataToRecordList(portNum,dataArray);
    foreach (QList<double> portValueList, dataList) {
        addOneSampleData(time,portValueList);
        time += this->sampleTime;
    }

}

void ChartWidget::addOneSampleData(const double &sampleTime, const QList<double> &portDataList)
{

    qDebug()<<"add sample points at time "<<sampleTime
           <<"with port datalist "<<portDataList
           <<"port num is"<<portNum;

    static QStringList colorList = {
        "#84bef7","#848ff7","#a784f7","#d284f7",
        "#f784e8","#f784a4","#f57b7b","#f4a87a",
        "#f4bf7a","#ece05d","#d5eb5e","#91f47a",
        "#7af4ab","#75ecdd","#66daef","#58bcf0"
    };

    for(int i=0; i< portNum && i< portDataList.size();i++)
    {
        QLineSeries* portSerires =  portSeriresMap[i];
        if(!portSerires)
        {
            portSerires = new QLineSeries(m_chartView);

            if(toolTipEnable)
            {
                connect(portSerires, SIGNAL(clicked(QPointF)),
                        m_chartView, SLOT(keepCallout()));
                connect(portSerires, SIGNAL(hovered(QPointF,bool)),
                        m_chartView, SLOT(tooltip(QPointF,bool)));
            }

            QPen portPen =  portSerires->pen();
            portPen.setWidthF(1);
            portPen.setColor(colorList[i]);
            portSerires->setPen(portPen);

            m_chart->addSeries(portSerires);
            portSerires->attachAxis(m_chart->axisX());
            portSerires->attachAxis(m_chart->axisY());
            portSeriresMap[i] = portSerires;
            colorLengend->setPortColor(i,portSerires->pen().color().name());
//            colorLengend->setPortVisualObject(i,portSerires);
        }
        portSerires->append(sampleTime,portDataList[i]);
    }

    //update
    changeDataRange(sampleTime,portDataList,timeRange,dataRange);

    m_chart->axisX()->setRange(timeRange.x(),timeRange.y());
//    double height = dataRange.y() - dataRange.x();
    m_chart->axisY()->setRange(dataRange.x() /*- qMax( 0.1 * height, 0.05)*/,
                               dataRange.y() /*+ qMax( 0.1 * height, 0.05)*/);

    m_chartView->setTimeRange(timeRange);
    m_chartView->setDataRange(QPointF(dataRange.x() /*- qMax( 0.1 * height, 0.05)*/,
                              dataRange.y() /*+ qMax( 0.1 * height, 0.05)*/));

//    {
//        QValueAxis* axisx = qobject_cast<QValueAxis*> (m_chart->axisX());
//        axisx->applyNiceNumbers();
//        QValueAxis* axisy = qobject_cast<QValueAxis*> (m_chart->axisY());
//        axisy->applyNiceNumbers();
//    }

}

void ChartWidget::onToolTipEnableChanged(bool enable)
{
    if(enable)
    {
        foreach (QLineSeries* line, portSeriresMap.values()) {
            connect(line, SIGNAL(clicked(QPointF)),
                    m_chartView, SLOT(keepCallout()));
            connect(line, SIGNAL(hovered(QPointF,bool)),
                    m_chartView, SLOT(tooltip(QPointF,bool)));
        }
    }else
    {
        foreach (QLineSeries* line, portSeriresMap.values()) {
            disconnect(line, SIGNAL(clicked(QPointF)),
                    m_chartView, SLOT(keepCallout()));
            disconnect(line, SIGNAL(hovered(QPointF,bool)),
                    m_chartView, SLOT(tooltip(QPointF,bool)));
        }
    }
}

void ChartWidget::changeToDrawHLineMode()
{
    if(m_chartView->getMode() != MyChartView::DrawHLine)
        m_chartView->setMode(MyChartView::DrawHLine);
    else
        m_chartView->setMode(MyChartView::Normal);
}

void ChartWidget::changeToDrawVLineMode()
{
    if(m_chartView->getMode() != MyChartView::DrawVLine)
        m_chartView->setMode(MyChartView::DrawVLine);
    else
        m_chartView->setMode(MyChartView::Normal);
}

void ChartWidget::addHLine(double tempValue)
{
    m_chartView->addLine(tempValue,true);
}

void ChartWidget::addVLine(double timeValue)
{
    m_chartView->addLine(timeValue,false);
}

void ChartWidget::setAxisLabelInfo(const QMap<QString,TopTest::AxisLabelInfo>& infos)
{
    m_chartView->setAxisLabelInfo(infos);
}

void ChartWidget::addLine(const TopTest::LineInfo &lineInfo)
{
    switch (lineInfo.type) {
    case TopTest::HLine:
    {
        addHLine(lineInfo.value);
    }
        break;
    case TopTest::VLine:
    {
        addVLine(lineInfo.value);
    }
        break;
    case TopTest::TwoPointLine:
    {
    }
        break;
    default:
        break;
    }
}

void ChartWidget::clearAllLine()
{
    m_chartView->removeAllLine();
}

void ChartWidget::reDrawTempSection(const TempSection &tempSection)
{
    ///remove old section lines
    foreach (QLineSeries* line, m_tempSectionSeries) {
        if(line)
            m_chart->removeSeries(line);
    }
    m_tempSectionSeries.clear();


    ///draw new lines

    if(tempSection.sections.isEmpty())
        return;
    QList<double> timeList;

    timeList<<tempSection.sections.first().startTime;
    foreach (TempSectionItem item, tempSection.sections) {
        timeList<<item.endTime;
        QLineSeries* upTempLine = new QLineSeries(m_chartView);
        upTempLine->append(item.startTime,item.upValue);
        upTempLine->append(item.endTime,item.upValue);
        m_tempSectionSeries<<upTempLine;

        qDebug()<<"tempSection.isDoubleSection"<<tempSection.isDoubleSection;
        if(!tempSection.isDoubleSection)
        {
            qDebug()<<"draw down line";
            QLineSeries* downTempLine = new QLineSeries(m_chartView);
            downTempLine->append(item.startTime,item.downValue);
            downTempLine->append(item.endTime,item.downValue);
            m_tempSectionSeries<<downTempLine;
        }
    }

    m_tempSectionSeries << m_chartView->drawTimeLines(timeList);
    foreach (QLineSeries* line, m_tempSectionSeries) {
        QPen portPen =  line->pen();
        portPen.setWidthF(1.5);
        portPen.setColor(QColor(Qt::black));
        portPen.setStyle(Qt::DashLine);
        line->setPen(portPen);

        m_chart->addSeries(line);
        line->attachAxis(m_chart->axisX());
        line->attachAxis(m_chart->axisY());

        line->setVisible(showTempSection);
    }
}

void ChartWidget::changeTempSectionVisiable()
{
    showTempSection = !showTempSection;
    foreach (QLineSeries* line, m_tempSectionSeries) {
        line->setVisible(showTempSection);
    }
}

QByteArray ChartWidget::getCurveData() const
{
    return curveData;
}

void ChartWidget::hideForGrab(bool hide)
{
    m_chartView->hideForGrab(hide);
}

QDateTime ChartWidget::getTestDateTime() const
{
    return testDateTime;
}

void ChartWidget::setTestDateTime(const QDateTime &value)
{
    testDateTime = value;
}

void ChartWidget::moveSliderToDistance(const double &distance)
{
    m_chartView->setSliderValue(distance);
}

void ChartWidget::setSliderRange(const double &max, const double &min)
{
    m_chartView->setSliderRange(QPointF(min,max));
}

QList<double> ChartWidget::getCurrentPortsTempList(double currentTime)
{
    qDebug()<<"ChartWidget::getCurrentPortsTempList";

    QList<double> ret;
    QMap <int,QVector<QPointF> > portVectorMap;

    foreach (int port, portSeriresMap.keys()) {
        portVectorMap.insert(port,portSeriresMap.value(port)->pointsVector());
    }

    if(portVectorMap.isEmpty())
        return ret;

    int currentPos = 0;

    QVector<QPointF> firstDataList = portVectorMap.values().first();
    for(int i = 0 ; i < firstDataList.count(); i++)
    {
        QPointF point = firstDataList.at(i);
        if(currentTime > point.x())
            continue;

        currentPos = i;
    }

    foreach (int port, portVectorMap.keys()) {
        ret<<portVectorMap.value(port).at(currentPos).y();
    }

    return ret;
}

//QByteArray ChartWidget::saveChartToCurveData()
//{
//    QByteArray curveData;

//    if(portSeriresMap.isEmpty())
//        return curveData;

//    QLineSeries* firstLine = portSeriresMap.values().first();
//    if(!firstLine || firstLine->pointsVector().isEmpty())
//        return curveData;

//    QList<QVector<QPointF>> portDataVectorList;
//    foreach (int portNo, portSeriresMap.keys()) {
//        portDataVectorList<<portSeriresMap.value(portNo)->pointsVector();
//    }

//    for(int j =0 ; j< firstLine->pointsVector().count(); j++)
//    {
//        for(int i = 0; i< portNum ; i++)
//        {
//            QPointF dataPoint = portDataVectorList.at(i).at(j);
//            curveData += "," + QString::number(dataPoint.y());
//        }

//        curveData += "\n";
//    }

//    return curveData;
//}

void ChartWidget::clearData()
{
    foreach (int portNo, portSeriresMap.keys()) {
        m_chart->removeSeries(portSeriresMap[portNo]);
    }
    this->curveData.clear();

    colorLengend->clearData();
}


bool ChartWidget::getToolTipEnable() const
{
    return toolTipEnable;
}

QString ChartWidget::getSourceDataString()
{
    QString str;

    if(portSeriresMap.isEmpty())
        return "No data";

    QLineSeries* firstLine = portSeriresMap.values().first();
    if(!firstLine || firstLine->pointsVector().isEmpty())
        return "No point";

    QList<QVector<QPointF>> portDataVectorList;
    foreach (int portNo, portSeriresMap.keys()) {
        portDataVectorList<<portSeriresMap.value(portNo)->pointsVector();
    }

    QDateTime printDateTime = testDateTime;

    if(printDateTime.isNull())
        printDateTime = QDateTime::currentDateTime();

    qDebug()<<"sampleTime"<<sampleTime<<"testDateTime"<<printDateTime;

    for(int j =0 ; j< firstLine->pointsVector().count(); j++)
    {
        str += printDateTime.date().toString("yyyy/MM/dd")
                +","+printDateTime.time().toString("hh:mm:ss");
        for(int i = 0; i< portNum ; i++)
        {
            QPointF dataPoint = portDataVectorList.at(i).at(j);
            str += "," + QString::number(dataPoint.y());
        }

        str += "\n";
        printDateTime.addMSecs(sampleTime*1000);
    }

    return str;
}

void ChartWidget::setToolTipEnable(bool value)
{
    toolTipEnable = value;
    onToolTipEnableChanged(value);
    m_chartView->onTooltipEnableChanged(value);
}

//void ChartWidget::applyProgressToPortData(const int &portNum)
//{
//    DataProgressHandleAlgorithm::instance()->setInput(portSeriresMap[portNum]->pointsVector());
//    QMap<QString,double> paramters;
//    paramters.insert("TemperatureMinValue",200.0);
//    paramters.insert("TemperatureMaxValue",750.0);
//    paramters.insert("TemperatureRemainValue",500);
//    DataProgressHandleAlgorithm::instance()->setParameters(paramters);

//    DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::IncreaseTime);
//    DataProgressHandleAlgorithm::instance()->update();

//    DataProgressHandleAlgorithm::instance()->setAlgoType(TopTest::AboveTime);
//    DataProgressHandleAlgorithm::instance()->update();
//}

FormColorLengend *ChartWidget::getColorLengend() const
{
    return colorLengend;
}

void ChartWidget::setColorLengend(FormColorLengend *value)
{
    colorLengend = value;
}

QVector<QPointF> ChartWidget::getPortPointFVector(int portNo)
{
    if(portSeriresMap.value(portNo-1,0))
        return portSeriresMap[portNo-1]->pointsVector();
    else
        return QVector<QPointF>();
}

QList<QVector<QPointF>> ChartWidget::getAllPortPointFVector()
{
    QList<QVector<QPointF>> ret;
    foreach (QLineSeries* line, portSeriresMap.values()) {
        ret<<line->pointsVector();
    }

    return ret;

}

QMap<int, QPair<QString, QColor> > ChartWidget::getPortInfoMap()
{
    return colorLengend->getPortInfoMap();
}

void ChartWidget::setPortInfos(const QMap<int, QPair<QString, QColor> > &portInfoMap)
{
    foreach (int port, portInfoMap.keys()) {
        QPair<QString, QColor> info = portInfoMap[port];

        QLineSeries* portSerires =  portSeriresMap[port-1];
        if(!portSerires)
        {
            portSerires = new QLineSeries(this);
//            colorLengend->setPortVisualObject(port-1,portSerires);
        }

        portSerires->setColor(info.second);

        //apply legend
        colorLengend->setPortName(port-1, info.first);
        colorLengend->setPortColor(port-1, portSerires->color().name());
    }
}

QMap<QString, TopTest::AxisLabelInfo> ChartWidget::getAxisLabelInfos()
{
    return m_chartView->getAxisLabelInfos();
}

double ChartWidget::getSampleTime() const
{
    return sampleTime;
}

void ChartWidget::setSampleTime(double value)
{
    sampleTime = value;
}

int ChartWidget::getPortNum() const
{
    return portNum;
}

void ChartWidget::setPortNum(int value)
{
    portNum = value;
    colorLengend->setPortNum(value);
}
