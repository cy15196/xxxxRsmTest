#include "clabsostepaxis.h"

#include "private/clabsostepaxis_p.h"
#include <private/chartclabsostepaxisx_p.h>
#include <private/chartclabsostepaxisy_p.h>
#include <private/abstractdomain_p.h>
#include <private/polarchartvalueaxisangular_p.h>
#include <private/polarchartvalueaxisradial_p.h>
#include <private/chartdataset_p.h>
#include <private/chartpresenter_p.h>
#include <private/charttheme_p.h>
#include <private/charthelpers_p.h>

QT_CHARTS_BEGIN_NAMESPACE

ClabsoStepAxis::ClabsoStepAxis(QObject *parent)
    :QAbstractAxis(*new ClabsoStepAxisPrivate(this),parent)
{

}

ClabsoStepAxis::~ClabsoStepAxis()
{
    Q_D(ClabsoStepAxis);
    if (d->m_chart)
        d->m_chart->removeAxis(this);
}

QAbstractAxis::AxisType ClabsoStepAxis::type() const
{
    return (QAbstractAxis::AxisType)ClabsoStepAxisType;
}

void ClabsoStepAxis::setMin(qreal min)
{
    Q_D(ClabsoStepAxis);
    setRange(min, qMax(d->m_max, min));
}

qreal ClabsoStepAxis::min() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_min;
}

void ClabsoStepAxis::setMax(qreal max)
{
    Q_D(ClabsoStepAxis);
    setRange(qMin(d->m_min, max), max);
}

qreal ClabsoStepAxis::max() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_max;
}

void ClabsoStepAxis::setRange(qreal min, qreal max)
{
    Q_D(ClabsoStepAxis);
    d->setRange(min,max);
}

void ClabsoStepAxis::setTickStep(qreal step)
{
    Q_D(ClabsoStepAxis);
    if (d->m_tickStep != step && step >= 0) {
        d->m_tickStep = step;
        emit tickStepChanged(step);
    }
}

qreal ClabsoStepAxis::tickStep() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_tickStep;
}

void ClabsoStepAxis::setTickCount(int Count)
{
    Q_D(ClabsoStepAxis);
    if (d->m_tickCount != Count && Count >= 0) {
        d->m_tickCount = Count;
    }
}

int ClabsoStepAxis::tickCount() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_tickCount;
}

void ClabsoStepAxis::setMinorTickStep(qreal step)
{
    Q_D(ClabsoStepAxis);
    if (d->m_minorTickStep != step && step >= 0) {
        d->m_minorTickStep = step;
        emit minorTickStepChanged(step);
    }
}

qreal ClabsoStepAxis::minorTickStep() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_minorTickStep;
}

void ClabsoStepAxis::setLabelFormat(const QString &format)
{
    Q_D(ClabsoStepAxis);
    d->m_format = format;
    emit labelFormatChanged(format);
}

QString ClabsoStepAxis::labelFormat() const
{
    Q_D(const ClabsoStepAxis);
    return d->m_format;
}

ClabsoStepAxis::ClabsoStepAxis(ClabsoStepAxisPrivate &d, QObject *parent)
    : QAbstractAxis(d, parent)
{

}

void ClabsoStepAxis::applyNiceNumbers()
{
//    Q_D(ClabsoStepAxis);
//    if(d->m_applying) return;
//    qreal min = d->m_min;
//    qreal max = d->m_max;
//    int ticks = d->m_tickStep;
//    AbstractDomain::looseNiceNumbers(min,max,ticks);
//    d->m_applying=true;
//    d->setRange(min,max);
//    setTickStep(ticks);
//    d->m_applying=false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClabsoStepAxisPrivate::ClabsoStepAxisPrivate(ClabsoStepAxis *q)
    : QAbstractAxisPrivate(q),
      m_min(0),
      m_max(0),
      m_tickStep(500),
      m_tickCount(0),
      m_minorTickStep(100),
      m_format(),
      m_applying(false)
{

}

ClabsoStepAxisPrivate::~ClabsoStepAxisPrivate()
{

}

void ClabsoStepAxisPrivate::setMin(const QVariant &min)
{
    Q_Q(ClabsoStepAxis);
    bool ok;
    qreal value = min.toReal(&ok);
    if (ok)
        q->setMin(value);
}

void ClabsoStepAxisPrivate::setMax(const QVariant &max)
{
    Q_Q(ClabsoStepAxis);
    bool ok;
    qreal value = max.toReal(&ok);
    if (ok)
        q->setMax(value);
}

void ClabsoStepAxisPrivate::setRange(const QVariant &min, const QVariant &max)
{
    Q_Q(ClabsoStepAxis);
    bool ok1;
    bool ok2;
    qreal value1 = min.toReal(&ok1);
    qreal value2 = max.toReal(&ok2);
    if (ok1 && ok2)
        q->setRange(value1, value2);
}

void ClabsoStepAxisPrivate::setRange(qreal min, qreal max)
{
    Q_Q(ClabsoStepAxis);
    bool changed = false;

    if (min > max)
        return;

    if (!isValidValue(min, max)) {
        qWarning() << "Attempting to set invalid range for value axis: ["
                   << min << " - " << max << "]";
        return;
    }

    if (m_min != min) {
        m_min = min;
        changed = true;
        emit q->minChanged(min);
    }

    if (m_max != max) {
        m_max = max;
        changed = true;
        emit q->maxChanged(max);
    }

    if (changed) {
        emit rangeChanged(min,max);
        emit q->rangeChanged(min, max);
    }
}

void ClabsoStepAxisPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(ClabsoStepAxis);
    ChartAxisElement *axis(0);

    if (m_chart->chartType() == QChart::ChartTypeCartesian) {
        if (orientation() == Qt::Vertical)
            axis = new ChartClabsoStepAxisY(q,parent);
        if (orientation() == Qt::Horizontal)
            axis = new ChartClabsoStepAxisX(q,parent);
    }

//    if (m_chart->chartType() == QChart::ChartTypePolar) {
//        if (orientation() == Qt::Vertical)
//            axis = new PolarChartValueAxisRadial(q, parent);
//        if (orientation() == Qt::Horizontal)
//            axis = new PolarChartValueAxisAngular(q, parent);
//    }

    m_item.reset(axis);
    QAbstractAxisPrivate::initializeGraphics(parent);
}


void ClabsoStepAxisPrivate::initializeDomain(AbstractDomain *domain)
{
    if (orientation() == Qt::Vertical) {
        if (!qFuzzyIsNull(m_max - m_min))
            domain->setRangeY(m_min, m_max);
        else
            setRange(domain->minY(), domain->maxY());
    }
    if (orientation() == Qt::Horizontal) {
        if (!qFuzzyIsNull(m_max - m_min))
            domain->setRangeX(m_min, m_max);
        else
            setRange(domain->minX(), domain->maxX());
    }
}

#include "moc_clabsostepaxis.cpp"
#include "moc_clabsostepaxis_p.cpp"

QT_CHARTS_END_NAMESPACE
