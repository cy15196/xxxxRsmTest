#ifndef QCPLABELTRACER_H
#define QCPLABELTRACER_H

#include "qcustomplot.h"

//class QCustomPlot;
//class QCPItemTracer;
//class QCPItemLine;
//class QCPItemText;

class QCPLabelTracer : public QObject
{
    Q_OBJECT
public:

    enum TracerType
    {
        XAxisTracer,
        YAxisTracer,
        RAxisTracer,
        DataTracer,
        AxisUnit
    };

    enum AxisLabelType
    {
        XAxis,
        YAxis,
        RAxis,
    };

public:

    explicit QCPLabelTracer(QCustomPlot *_plot, TracerType _type, QObject *parent = nullptr);
    ~QCPLabelTracer();

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setText(const QString &text);
    void setLabelPen(const QPen &pen);
    void updatePosition(double xValue, double yValue);
    void setVisible(bool visible);
    void setAxisType(const AxisLabelType &value);

protected:
    QCustomPlot *plot = nullptr;
    QCPItemTracer *tracer = nullptr;// 跟踪的点
    QCPItemText *label = nullptr;   // 显示的数值
    QCPItemLine *arrow = nullptr;   // 箭头

    TracerType type = DataTracer;
    AxisLabelType axisType = XAxis;
    bool visible = false;
};

#endif // QCPLABELTRACER_H
