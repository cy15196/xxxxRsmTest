#ifndef CHARTVHLINE_H
#define CHARTVHLINE_H

#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>
#include <QLineSeries>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ChartVHLine: public QLineSeries
{
public:
    ChartVHLine(QChart *parent,bool vertical = false);

//    void setText(const QString &text);
    void setAnchor(QPointF point);
    void updateGeometry();

//    QRectF boundingRect() const;
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

    bool getVertical() const;
    void setVertical(bool value);

protected:
    //    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

    bool vertical;//true vline , false hline
    QPointF m_anchor;
    QChart *m_chart;
};

#endif // CHARTVHLINE_H
