#ifndef CLABSOSTEPAXIS_H
#define CLABSOSTEPAXIS_H

#include <QtCharts/QAbstractAxis>

QT_CHARTS_BEGIN_NAMESPACE

#define ClabsoStepAxisType 0x20

class ClabsoStepAxisPrivate;

class ClabsoStepAxis: public QAbstractAxis
{
    Q_OBJECT
    Q_PROPERTY(qreal tickStep READ tickStep WRITE setTickStep NOTIFY tickStepChanged)
    Q_PROPERTY(qreal min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(qreal max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(qreal minorTickStep READ minorTickStep WRITE setMinorTickStep NOTIFY minorTickStepChanged)


public:
    explicit ClabsoStepAxis(QObject *parent = Q_NULLPTR);
    ~ClabsoStepAxis();

public:
    AxisType type() const;

    //range handling
    void setMin(qreal min);
    qreal min() const;
    void setMax(qreal max);
    qreal max() const;
    void setRange(qreal min, qreal max);

    //ticks handling
    void setTickStep(qreal step);
    qreal tickStep() const;
    void setTickCount(int Count);
    int tickCount() const;
    void setMinorTickStep(qreal step);
    qreal minorTickStep() const;

    void setLabelFormat(const QString &format);
    QString labelFormat() const;

protected:
    ClabsoStepAxis(ClabsoStepAxisPrivate &d, QObject *parent = Q_NULLPTR);


public Q_SLOTS:
    void applyNiceNumbers();

Q_SIGNALS:
    void minChanged(qreal min);
    void maxChanged(qreal max);
    void rangeChanged(qreal min, qreal max);
    void tickStepChanged(qreal tickCount);
    void minorTickStepChanged(qreal tickCount);
    void labelFormatChanged(const QString &format);

private:
    Q_DECLARE_PRIVATE(ClabsoStepAxis)
    Q_DISABLE_COPY(ClabsoStepAxis)

};

QT_CHARTS_END_NAMESPACE

#endif // CLABSOSTEPAXIS_H
