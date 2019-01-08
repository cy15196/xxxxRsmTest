#include "mychartview.h"

#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QtCharts/QValueAxis>

#include <QLayout>

#include "chartvhline.h"
#include "rulerslidergraphicitem.h"

#include "toptestcore.h"

#include "clabsostepaxis.h"


#include <QDebug>

MyChartView::MyChartView(QtCharts::QChart *chart, QWidget *parent)
    : m_coordX(0),
      m_coordY(0),
      m_chart(chart),
      m_tooltip(0),
      m_rulerSlider(0),
//      m_line(0),
      mode(0),
      showTooptips(false),
      m_UnitX(0),
      m_UnitY(0),
      QtCharts::QChartView(chart,parent)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_chart->setAcceptHoverEvents(true);
    setRenderHint(QPainter::Antialiasing);


    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()* 0.75 - 50, 20);
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()* 0.75 + 50, 20);
    m_coordY->setText("Y: ");

//    m_UnitX = new QGraphicsSimpleTextItem(m_chart);
//    m_UnitX->setPos(600, 20);
//    m_UnitX->setText(TopTestCore::instance()->getCurrentTimeUnitStr());
//    m_UnitY = new QGraphicsSimpleTextItem(m_chart);
//    m_UnitY->setPos(20, 20);
//    m_UnitY->setText(TopTestCore::instance()->getCurrentTemputureUnitStr());

    m_rulerSlider = new RulerSliderGraphicItem(m_chart);
    m_rulerSlider->setPrecision(2);
    m_rulerSlider->setPos(0, m_chart->size().height());
    m_rulerSlider->setWholeSize(m_chart->size());

    m_rulerSlider->setRange(0, 200);
    m_rulerSlider->setValue(0);
    m_rulerSlider->setShortStep(5);
    m_rulerSlider->setLongStep(20);
    m_rulerSlider->setSliderColorTop(QColor(24, 188, 155));
    m_rulerSlider->setSliderColorBottom(QColor(0, 0, 255));
    m_rulerSlider->setBgColorStart(QColor("#f0f5f8"));
    m_rulerSlider->setBgColorEnd(QColor("#f0f5f8"));
    m_rulerSlider->setLineColor(QColor(0,0,0));

    m_HLine = new QLineSeries(m_chart);
    m_chart->addSeries(m_HLine);

    m_VLine = new QLineSeries(m_chart);
    m_chart->addSeries(m_VLine);

    QPen portPen =  m_HLine->pen();
    portPen.setWidthF(1);
    portPen.setColor(QColor(Qt::black));
    m_HLine->setPen(portPen);
    m_VLine->setPen(portPen);

    timeRange = QPointF(0,1);
    dataRange = QPointF(0,1);
    lastPos = QPointF(0.5,0.5);

    updateHVLines();

    m_chart->createDefaultAxes();
    yValueAxis = dynamic_cast<QValueAxis*> (m_chart->axisY());
//    xValueAxis = new ClabsoStepAxis(m_chart);
//    m_chart->setAxisX(xValueAxis);
    xValueAxis = dynamic_cast<QValueAxis*> (m_chart->axisX());


    m_HLine->setVisible(false);
    m_VLine->setVisible(false);

    xValueAxis->setRange(0,1);
    yValueAxis->setRange(0,1);

    connect(xValueAxis,SIGNAL(rangeChanged(qreal,qreal)),
            this,SLOT(updateRulerSlider()));
//    connect(yValueAxis,SIGNAL(rangeChanged(qreal,qreal)),
//            this,SLOT(updateRulerSlider()));

//    this->m_rulerSlider->hide();

//    this->setMouseTracking(true);
}

void MyChartView::keepCallout()
{
    qDebug()<<"MyChartView::keepCallout";

    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void MyChartView::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (showTooptips && state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

void MyChartView::onTooltipEnableChanged(bool enable)
{

    showTooptips = enable;

    m_HLine->setVisible(enable);
    m_VLine->setVisible(enable);

    if(!enable)
    {
        foreach (Callout* tooltip, m_callouts) {
            tooltip->hide();
            delete tooltip;
        }
    }
//    mode |= ShowTooltip;
}

void MyChartView::keepLine(bool h)
{
    qDebug()<<"MyChartView::keepLine"<<h;

    QLineSeries* newLine = new QLineSeries(m_chart);
    m_chart->addSeries(newLine);
    newLine->attachAxis(m_chart->axisX());
    newLine->attachAxis(m_chart->axisY());
    QGraphicsSimpleTextItem* labelItem = new QGraphicsSimpleTextItem(m_chart);

    if(h)
    {
        newLine->append(m_HLine->points());
        labelItem->setPos( m_chart->mapToPosition(newLine->points().first()) - QPointF(50,0));
        labelItem->setText(QString("%1").arg(lastPos.y(),5));
    }
    else
    {
        newLine->append(m_VLine->points() );
        labelItem->setPos( m_chart->mapToPosition(newLine->points().first()) + QPointF(0,8));
        labelItem->setText(QString("%1").arg(lastPos.x(),5));
    }

    newLine->setPen(m_HLine->pen());

    qDebug()<<newLine->points();
    m_saveLabelLines.insert(labelItem,newLine);

//    m_chart->createDefaultAxes();
    m_chart->axisX()->setRange(timeRange.x(),timeRange.y());
    m_chart->axisY()->setRange(dataRange.x(),dataRange.y());
}

void MyChartView::addLine(double value, bool h)
{
    QLineSeries* newLine = new QLineSeries(m_chart);
    m_chart->addSeries(newLine);
    QGraphicsSimpleTextItem* labelItem = new QGraphicsSimpleTextItem(m_chart);
    newLine->attachAxis(m_chart->axisX());
    newLine->attachAxis(m_chart->axisY());
    if(h)
    {
        newLine->append(m_HLine->points().first().x(),value);
        newLine->append(m_HLine->points().last().x(),value);
        labelItem->setPos( m_chart->mapToPosition(newLine->points().last()) - QPointF(8,0));
        labelItem->setText(QString("%1").arg(value,5));
    }
    else
    {
        newLine->append(value,m_VLine->points().first().y());
        newLine->append(value,m_VLine->points().last().y());
        labelItem->setPos( m_chart->mapToPosition(newLine->points().last()) - QPointF(30,15));
        labelItem->setText(QString("%1").arg(value,5));
    }

    newLine->setPen(m_HLine->pen());

    qDebug()<<newLine->points();
    m_saveLabelLines.insert(labelItem,newLine);

//    m_chart->createDefaultAxes();
    m_chart->axisX()->setRange(timeRange.x(),timeRange.y());
    m_chart->axisY()->setRange(dataRange.x(),dataRange.y());
}

void MyChartView::removeAllLine()
{
    foreach (QGraphicsSimpleTextItem* label, m_saveLabelLines.keys()) {
        m_chart->removeSeries(m_saveLabelLines[label]);
        delete label;
    }

    m_saveLabelLines.clear();
}

void MyChartView::setAxisLabelInfo(const QMap<QString,TopTest::AxisLabelInfo>& infos)
{


    TopTest::AxisLabelInfo xAxisinfo = infos["X"];
    TopTest::AxisLabelInfo yAxisinfo = infos["Y"];
    TopTest::AxisLabelInfo dAxisinfo = infos["D"];

    qDebug()<<"MyChartView::setAxisLabelInfo";
    if(xValueAxis)
    {
        qDebug()<<"MyChartView::xValueAxis"<<xAxisinfo.bigTickCount;
        xValueAxis->setLinePen(QPen(QColor(Qt::black)));
        xValueAxis->setGridLineColor(QColor(Qt::darkGray));
        xValueAxis->setRange(xAxisinfo.range.x(),xAxisinfo.range.y());
//        xValueAxis->setTickStep(500);
//        xValueAxis->setMinorTickStep(100);
        xValueAxis->setTickCount(xAxisinfo.bigTickCount);
        xValueAxis->setMinorTickCount(xAxisinfo.smallTickCountPerBigStep);
        xValueAxis->setLabelFormat(xAxisinfo.labelFormat);//"%.2f"
        xValueAxis->setGridLineVisible(xAxisinfo.showGridLines);
        xValueAxis->setTitleText(xAxisinfo.axisTitle+QString("(%1)").arg(TopTestCore::instance()->getCurrentTimeUnitStr()));
        xValueAxis->setTitleVisible(xAxisinfo.showAxisTitle);
    }

    if(yValueAxis)
    {
        qDebug()<<"MyChartView::yValueAxis";
        yValueAxis->setLinePen(QPen(QColor(Qt::black)));
        yValueAxis->setGridLineColor(QColor(Qt::darkGray));
        yValueAxis->setRange(yAxisinfo.range.x(),yAxisinfo.range.y());
//        yValueAxis->setTickStep(500);
//        yValueAxis->setMinorTickStep(yAxisinfo.smallTickCountPerBigStep);
        yValueAxis->setTickCount(yAxisinfo.bigTickCount);
        yValueAxis->setMinorTickCount(yAxisinfo.smallTickCountPerBigStep);
        yValueAxis->setLabelFormat(yAxisinfo.labelFormat);//"%.2f"
        yValueAxis->setGridLineVisible(yAxisinfo.showGridLines);
        yValueAxis->setTitleText(yAxisinfo.axisTitle+QString("(%1)").arg(TopTestCore::instance()->getCurrentTemputureUnitStr()));
        yValueAxis->setTitleVisible(yAxisinfo.showAxisTitle);
    }

    if(m_rulerSlider)
    {
        double value = m_rulerSlider->getValue();
        m_rulerSlider->setRange(dAxisinfo.range.x(),dAxisinfo.range.y());
        double rangeWidth = m_rulerSlider->getMaxValue() - m_rulerSlider->getMinValue();
        m_rulerSlider->setLongStep(rangeWidth / dAxisinfo.bigTickCount);
        m_rulerSlider->setShortStep(m_rulerSlider->getLongStep()/ dAxisinfo.smallTickCountPerBigStep);
        m_rulerSlider->setValue(value);
//        m_rulerSlider->setLabelFormat(yAxisinfo.labelFormat);//"%.2f"
//        m_rulerSlider->setGridLineVisible(yAxisinfo.showGridLines);
//        m_rulerSlider->setTitleText(yAxisinfo.axisTitle);
//        m_rulerSlider->setTitleVisible(yAxisinfo.showAxisTitle);
    }


}

//void MyChartView::saveHLine()
//{
//    qDebug()<<"MyChartView::saveHLine";

//    if (m_line == 0)
//    {
//        m_line = new ChartVHLine(m_chart);
//        m_chart->addSeries(m_line);
//        m_line->attachAxis(m_chart->axisX());
//        m_line->attachAxis(m_chart->axisY());
//    }


//    m_line->setVertical(false);
//    m_line->setAnchor(lastPos);
//    m_line->updateGeometry();
//}

//void MyChartView::saveVLine()
//{
//    qDebug()<<"MyChartView::saveVLine";

//    if (m_line == 0)
//    {
//        m_line = new ChartVHLine(m_chart,true);
//        m_chart->addSeries(m_line);
//    }

//    m_line->setVertical(true);

//    m_line->setAnchor(lastPos);
//    m_line->updateGeometry();
//}

void MyChartView::updateHVLines()
{
//    if(mode != ShowTooltip)
//    {
//        m_HLine->hide();
//        m_VLine->hide();
//        return;
//    }
//    else
//    {
//        m_HLine->show();
//        m_VLine->show();
//    }
//    qDebug()<<"MyChartView::updateHVLines()";

    m_HLine->clear();
    m_HLine->append(timeRange.x(),lastPos.y() );
    m_HLine->append(timeRange.y(),lastPos.y() );

    m_VLine->clear();
    m_VLine->append(lastPos.x(), dataRange.x() );
    m_VLine->append(lastPos.x(), dataRange.y() );

}

QList<QLineSeries *> MyChartView::drawTimeLines(const QList<double> &timeList)
{
    QList<QLineSeries *> ret;
    foreach (double time, timeList) {
        QLineSeries* newLine = new QLineSeries(m_chart);
        newLine->append(time,dataRange.x());
        newLine->append(time,dataRange.y());
        ret<<newLine;
    }
    return ret;
}

void MyChartView::updateRulerSlider()
{

    QPointF leftPot = m_chart->mapToPosition( {xValueAxis->min(),0} );
    QPointF rightPot= m_chart->mapToPosition( {xValueAxis->max(),0} );

    m_rulerSlider->setWholeSize(QSizeF( ( rightPot - leftPot ).x() + 40 ,50));
    m_rulerSlider->setPos( leftPot.x() - 20 , m_chart->size().height() - 50 );
    m_rulerSlider->update();
}

void MyChartView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()*0.75 - 50, 20);
         m_coordY->setPos(m_chart->size().width()*0.75 + 50, 20);

//         m_UnitX->setPos(m_chart->size().width() - 20 ,
//                         m_chart->size().height() - 20);
//         m_UnitY->setPos(20, 20);

         updateRulerSlider();

         const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void MyChartView::mousePressEvent(QMouseEvent *event)
{
    pressPos = event->pos();
    if(event->button() == Qt::MiddleButton)
    {
        this->setCursor(QCursor(Qt::OpenHandCursor));
    }

//    m_chart->mapFromScene( mapToGlobal(pressPos)

//    if(m_rulerSlider->contains(pressPos))
//    {
//        qDebug()<<"go into ruler slider"<<pressPos;
//    }

    QtCharts::QChartView::mousePressEvent(event);
}

double MyChartView::convertAxisXValueToDistanceValue(double xValue)
{
    double min = xValueAxis->min();
    double max = xValueAxis->max();
    double rMin = m_rulerSlider->getMinValue();
    double rMax = m_rulerSlider->getMaxValue();

    double ret = rMin + (xValue - min) * (rMax - rMin)/ (max-min);

    if(ret > rMax)
        ret = rMax;
    else if(ret < rMin)
        ret = rMin;

    return  ret;

}

double MyChartView::convertDistanceValueToAxisXValue(double distanceValue)
{
    double min = xValueAxis->min();
    double max = xValueAxis->max();
    double rMin = m_rulerSlider->getMinValue();
    double rMax = m_rulerSlider->getMaxValue();

    double ret =  min + (distanceValue - rMin) * (max - min) / (rMax-rMin) ;

    if(ret > max)
        ret = max;
    else if(ret < min)
        ret = min;

    return  ret;
}

void MyChartView::mouseMoveEvent(QMouseEvent *event)
{

    lastPos = m_chart->mapToValue(event->pos());

    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));

    m_rulerSlider->setValue(convertAxisXValueToDistanceValue(lastPos.x()));
    updateHVLines();

    QPoint dPos = (event->pos() - pressPos);
    if(this->cursor().shape() == Qt::OpenHandCursor)
    {
        this->chart()->scroll(-dPos.x(),dPos.y());
        pressPos = event->pos();
    }

    emit this->currentTimeChanged(lastPos.x());

    QtCharts::QChartView::mouseMoveEvent(event);
}

void MyChartView::mouseReleaseEvent(QMouseEvent *event)
{
    this->unsetCursor();
//    if(event->button() == Qt::RightButton)
//        event->accept();

    QtCharts::QChartView::mouseReleaseEvent(event);
}

void MyChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    switch (mode) {
    case DrawHLine:
        keepLine(true);
        break;
    case DrawVLine:
        keepLine(false);
        break;
    default:
        break;
    }
    QtCharts::QChartView::mouseDoubleClickEvent(event);
}

void MyChartView::wheelEvent(QWheelEvent *event)
{
//    if(event->angleDelta())
    qDebug()<<event->angleDelta();

    if(event->angleDelta().y()>0)
        this->chart()->zoom(0.95);
    else if(event->angleDelta().y()<0)
        this->chart()->zoom(1.05);

//    updateRulerSlider();

    QtCharts::QChartView::wheelEvent(event);

}

int MyChartView::getMode() const
{
    return mode;
}

void MyChartView::setMode(int value)
{
    mode = value;
}

QMap<QString, TopTest::AxisLabelInfo> MyChartView::getAxisLabelInfos()
{

    QMap<QString, TopTest::AxisLabelInfo> ret;

    TopTest::AxisLabelInfo xAxisinfo ;
    TopTest::AxisLabelInfo yAxisinfo ;
    TopTest::AxisLabelInfo dAxisinfo ;

    qDebug()<<"MyChartView::getAxisLabelInfos";
    if(xValueAxis)
    {
        xAxisinfo.range = {xValueAxis->min(),xValueAxis->max()};
        xAxisinfo.bigTickCount = xValueAxis->tickCount();
        xAxisinfo.smallTickCountPerBigStep = xValueAxis->minorTickCount();
//        xAxisinfo.bigTickCount = xValueAxis->tickStep();
//        xAxisinfo.smallTickCountPerBigStep = xValueAxis->minorTickStep();
        xAxisinfo.labelFormat = xValueAxis->labelFormat();
        xAxisinfo.showGridLines = xValueAxis->isGridLineVisible();
        QString xTitle = xValueAxis->titleText();
        if(xTitle.contains("(") && xTitle.contains(")"))
            xTitle = xTitle.left(xTitle.indexOf("("));
        xAxisinfo.axisTitle = xTitle;
        xAxisinfo.showAxisTitle = xValueAxis->isTitleVisible();
    }

    if(yValueAxis)
    {
        qDebug()<<"MyChartView::yValueAxis";
        yAxisinfo.range = {yValueAxis->min(),yValueAxis->max()};
//        yAxisinfo.bigTickCount = yValueAxis->tickStep();
//        yAxisinfo.smallTickCountPerBigStep = yValueAxis->minorTickStep();
        yAxisinfo.bigTickCount = yValueAxis->tickCount();
        yAxisinfo.smallTickCountPerBigStep = yValueAxis->minorTickCount();
        yAxisinfo.labelFormat = yValueAxis->labelFormat();
        yAxisinfo.showGridLines = yValueAxis->isGridLineVisible();
        QString xTitle = yValueAxis->titleText();
        if(xTitle.contains("(") && xTitle.contains(")"))
            xTitle = xTitle.left(xTitle.indexOf("("));
        yAxisinfo.axisTitle = xTitle;
        yAxisinfo.showAxisTitle = yValueAxis->isTitleVisible();
    }

    if(m_rulerSlider)
    {
        dAxisinfo.range = {m_rulerSlider->getMinValue(),m_rulerSlider->getMaxValue()};
        double rangeWidth = m_rulerSlider->getMaxValue() - m_rulerSlider->getMinValue();
        dAxisinfo.bigTickCount = rangeWidth / m_rulerSlider->getLongStep();
        dAxisinfo.smallTickCountPerBigStep = m_rulerSlider->getLongStep() / m_rulerSlider->getShortStep();
        dAxisinfo.labelFormat = xValueAxis->labelFormat();
        dAxisinfo.showGridLines = false;
        dAxisinfo.axisTitle = "Distance";//m_rulerSlider->titleText();
        dAxisinfo.showAxisTitle = false;//m_rulerSlider->isTitleVisible();
    }

    ret["X"] = xAxisinfo;
    ret["Y"] = yAxisinfo;
    ret["D"] = dAxisinfo;

    return ret;
}

void MyChartView::setSliderValue(const double &value)
{
    m_rulerSlider->setValue(value);
}

void MyChartView::setSliderRange(const QPointF &range)
{
    m_rulerSlider->setMinValue(range.x());
    m_rulerSlider->setMaxValue(range.y());
}

void MyChartView::hideForGrab(bool hide)
{
    m_coordX->setVisible(!hide);
    m_coordY->setVisible(!hide);
}

QPointF MyChartView::getDataRange() const
{
    return dataRange;
}

void MyChartView::setDataRange(const QPointF &value)
{
    dataRange = value;
}

QPointF MyChartView::getTimeRange() const
{
    return timeRange;
}

void MyChartView::setTimeRange(const QPointF &value)
{
    timeRange = value;
}
