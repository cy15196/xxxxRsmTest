#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "topteststructdata.h"

class Callout;
//class ChartVHLine;

namespace QtCharts {
    class QLineSeries;
    class QValueAxis;
    class QDateTimeAxis;
    class ClabsoStepAxis;
}

class RulerSliderGraphicItem;

class MyChartView : public QtCharts::QChartView
{
    Q_OBJECT
public:
    MyChartView(QtCharts::QChart* chart,QWidget* parent = nullptr);

    enum InteractiveMode
    {
        Normal = 0x00,
        ShowTooltip = 0x01,
        DrawHLine = 0x02,
        DrawVLine = 0x04
    };

    QPointF getTimeRange() const;
    void setTimeRange(const QPointF &value);

    QPointF getDataRange() const;
    void setDataRange(const QPointF &value);

    int getMode() const;
    void setMode(int value);

    QMap<QString,TopTest::AxisLabelInfo> getAxisLabelInfos();

    void setSliderValue(const double& value);
    void setSliderRange(const QPointF& range);

    void hideForGrab(bool hide);

    inline double convertAxisXValueToDistanceValue(double xValue);
    inline double convertDistanceValueToAxisXValue(double distanceValue);

signals:
    void currentTimeChanged(double time);

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
    void onTooltipEnableChanged(bool enable);

    void keepLine(bool h);

    void addLine(double value, bool h);

    void removeAllLine();

    void updateRulerSlider();

    void setAxisLabelInfo(const QMap<QString, TopTest::AxisLabelInfo> &infos);

//    void saveHLine();
//    void saveVLine();

    void updateHVLines();

    QList<QtCharts::QLineSeries*> drawTimeLines(const QList<double> & timeList);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent *event);
private:
    QPoint pressPos;
    QPointF lastPos;

    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;

    QGraphicsSimpleTextItem *m_UnitX;
    QGraphicsSimpleTextItem *m_UnitY;

    RulerSliderGraphicItem *m_rulerSlider;
    QtCharts::QChart *m_chart;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

//    ChartVHLine* m_line;
//    QList<ChartVHLine *> m_HLines;
//    QList<ChartVHLine *> m_VLines;

    QMap<QGraphicsSimpleTextItem*,QtCharts::QLineSeries*> m_saveLabelLines;

    QtCharts::QLineSeries* m_HLine;
    QtCharts::QLineSeries* m_VLine;

    bool showTooptips;
    int mode;

    QPointF timeRange;
    QPointF dataRange;

    QtCharts::QValueAxis* xValueAxis;
    QtCharts::QDateTimeAxis* xDateTimeAxis;
    QtCharts::QValueAxis* yValueAxis;

//    QtCharts::ClabsoStepAxis* xValueAxis;
//    QtCharts::ClabsoStepAxis* yValueAxis;

    bool pressed;
};

#endif // MYCHARTVIEW_H
