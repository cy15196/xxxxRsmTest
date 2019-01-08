#include "qcplabeltracer.h"

#include "qcustomplot.h"

QCPLabelTracer::QCPLabelTracer(QCustomPlot *_plot, TracerType _type, QObject *parent) : plot(_plot),
    type(_type),
    QObject(parent)
{
    if (plot)
    {
        tracer = new QCPItemTracer(plot);
        tracer->setStyle(QCPItemTracer::tsNone);
        tracer->setPen(QPen(Qt::red));
        tracer->setBrush(Qt::red);
        tracer->setSize(10);

        label = new QCPItemText(plot);
        label->setLayer("overlay");
        label->setClipToAxisRect(false);
        label->setPadding(QMargins(2, 2, 2, 2));
        label->setBrush(QBrush(QColor(244, 244, 244, 255)));
        label->setPen(QPen(Qt::blue));
        label->position->setParentAnchor(tracer->position);
        label->setFont(QFont("宋体", 10));

        arrow = new QCPItemLine(plot);
        arrow->setLayer("overlay");
        arrow->setClipToAxisRect(false);
        arrow->setHead(QCPLineEnding::esSpikeArrow);

        switch (type) {
        case XAxisTracer:
        {
//            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptAxisRectRatio);

            label->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(-20, 0);//偏移量
            break;
        }
        case RAxisTracer:
        {
//            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptAxisRectRatio);

            label->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(-5, 0);//偏移量
            break;
        }
        case YAxisTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
//            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

            label->setPositionAlignment(Qt::AlignRight|Qt::AlignHCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(-20, 0);//偏移量
            break;
        }
        case DataTracer:
        {
            tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
            tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

            label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);

            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(25, 0);
            break;
        }

        case AxisUnit:
        {
            tracer->position->setType(QCPItemPosition::ptAxisRectRatio);

            label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
            label->setBrush(QBrush(QColor(244, 244, 244, 0)));
            label->setPen(QPen(QColor(244, 244, 244, 0)));
//            label->setPadding();

            arrow->setVisible(false);
            arrow->end->setParentAnchor(tracer->position);
            arrow->start->setParentAnchor(arrow->end);
            arrow->start->setCoords(25, 0);
            break;
        }

        default:
            break;
        }

        setVisible(false);
    }
}

QCPLabelTracer::~QCPLabelTracer()
{
    if (tracer)
        plot->removeItem(tracer);
    if (label)
        plot->removeItem(label);
}

void QCPLabelTracer::setPen(const QPen &pen)
{
    tracer->setPen(pen);
    arrow->setPen(pen);
}

void QCPLabelTracer::setBrush(const QBrush &brush)
{
    tracer->setBrush(brush);
}

void QCPLabelTracer::setLabelPen(const QPen &pen)
{
    label->setPen(pen);
}

void QCPLabelTracer::setText(const QString &text)
{
    label->setText(text);
}

void QCPLabelTracer::setVisible(bool visible)
{
    tracer->setVisible(visible);
    label->setVisible(visible);

    arrow->setVisible(visible);
}

void QCPLabelTracer::setAxisType(const AxisLabelType &value)
{
    axisType = value;
}

void QCPLabelTracer::updatePosition(double xValue, double yValue)
{
    if (!visible)
    {
        setVisible(true);
        visible = true;
    }
    if (yValue > plot->yAxis->range().upper)
        yValue = plot->yAxis->range().upper;
    switch (type) {
    case XAxisTracer:
    {
        tracer->position->setCoords(xValue, 1);
        arrow->start->setCoords(0, -15);
        arrow->end->setCoords(0, 0);
        label->position->setCoords(0, 1);

        break;
    }
    case RAxisTracer:
    {
        tracer->position->setCoords(xValue, 1);
        arrow->start->setCoords(0, -20);
        arrow->end->setCoords(0, 40);
        label->position->setCoords(0, 40);

        break;
    }
    case YAxisTracer:
    {
        tracer->position->setCoords(0, yValue);
        arrow->start->setCoords(15, 0);
        arrow->end->setCoords(0, 0);
        label->position->setCoords(-18, -8);
        break;
    }
    case DataTracer:
    {
        tracer->position->setCoords(xValue, yValue);
        label->position->setCoords(0, 0);
        break;
    }

    case AxisUnit:
    {
        arrow->setVisible(false);
        tracer->position->setCoords(xValue, yValue);
        switch (axisType) {
        case XAxis:
        {
            label->position->setCoords(50, 25);
        }
            break;
        case YAxis:
        {
            label->position->setCoords(-60, -50);
            label->setRotation(-90);
        }
            break;
        case RAxis:
        {
            label->position->setCoords(50, 85);
        }
            break;
        default:
            break;
        }
        break;
    }

    default:
        break;
    }
}
