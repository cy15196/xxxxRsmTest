#ifndef FORMCHARTAXISSETTING_H
#define FORMCHARTAXISSETTING_H

#include <QWidget>
#include "dialogbase.h"
#include "topteststructdata.h"

class DialogChartAxisSetting;

class FormChartAxisSetting : public DialogBase
{
    Q_OBJECT
public:
    explicit FormChartAxisSetting(QWidget *parent = nullptr);
    ~FormChartAxisSetting();

    void setAxisLableInfos(const QMap<QString,TopTest::AxisLabelInfo>& labelInfoMap);

    void onButtonClicked(int button);
signals:

    void axisLabelInfoChanged(const QMap<QString,TopTest::AxisLabelInfo>& labelInfoMap);

public slots:

private:
    DialogChartAxisSetting* xAxisDC;
    DialogChartAxisSetting* yAxisDC;
    DialogChartAxisSetting* dAxisDC;
};

#endif // FORMCHARTAXISSETTING_H
