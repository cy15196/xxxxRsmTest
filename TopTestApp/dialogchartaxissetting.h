#ifndef DIALOGCHARTAXISSETTING_H
#define DIALOGCHARTAXISSETTING_H

#include <QWidget>

#include "topteststructdata.h"

namespace Ui {
class DialogChartAxisSetting;
}

class DialogChartAxisSetting : public QWidget
{
    Q_OBJECT

public:
    explicit DialogChartAxisSetting(const QString& axisTypeString, QWidget *parent = 0);
    ~DialogChartAxisSetting();


    TopTest::AxisLabelInfo getAxisInfo();
    void setAxisInfo(const TopTest::AxisLabelInfo &value);

private slots:
    void on_doubleSpinBox_rangeSmall_valueChanged(double arg1);

    void on_doubleSpinBox_rangeBig_valueChanged(double arg1);

private:
    Ui::DialogChartAxisSetting *ui;

    TopTest::AxisLabelInfo axisInfo;
};

#endif // DIALOGCHARTAXISSETTING_H
