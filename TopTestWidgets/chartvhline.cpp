#include "chartvhline.h"

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>

ChartVHLine::ChartVHLine(QChart *parent,bool vertical)
    :QLineSeries(parent),
    m_chart(parent),
    vertical(vertical)
{

}

void ChartVHLine::setAnchor(QPointF point)
{
    m_anchor = point;

    this->clear();
//    QRect rect = m_chart->axisX()->setMin();
    if(!vertical)
    {
        this->append(point+QPointF(-500,0));
        this->append(point+QPointF(500,0));
    }
    else
    {
        this->append(point+QPointF(0,-500));
        this->append(point+QPointF(0,500));
    }

}

void ChartVHLine::updateGeometry()
{
    m_chart->removeSeries(this);
    m_chart->addSeries(this);
}

//QRectF ChartVHLine::boundingRect() const
//{
//    QRectF rect;
//    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));

//    if(!vertical)
//    {
//        rect.setWidth(1000);
//        rect.setHeight(2);
//        rect.setTopLeft(QPointF(anchor.x()-500,anchor.y()-1));
//    }
//    else
//    {
//        rect.setWidth(2);
//        rect.setHeight(1000);
//        rect.setTopLeft(QPointF(anchor.x()-1,anchor.y()-500));
//    }

//    return rect;
//}

//void ChartVHLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    Q_UNUSED(option)
//    Q_UNUSED(widget)
//    QPointF realPos =mapFromParent( m_chart->mapToPosition(m_anchor));
//    if(!vertical)
//        painter->drawLine(realPos+QPointF(-500,0),realPos+QPointF(500,0));
//    else
//        painter->drawLine(realPos+QPointF(0,-500),realPos+QPointF(0,500));
//}

bool ChartVHLine::getVertical() const
{
    return vertical;
}

void ChartVHLine::setVertical(bool value)
{
    vertical = value;
}
