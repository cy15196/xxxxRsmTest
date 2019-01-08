#ifndef FORMCUSTERMPLOTCHART_H
#define FORMCUSTERMPLOTCHART_H

#include <QWidget>

namespace Ui {
class FormCustermPlotChart;
}

class FormCustermPlotChart : public QWidget
{
    Q_OBJECT

public:
    explicit FormCustermPlotChart(QWidget *parent = 0);
    ~FormCustermPlotChart();

private:
    Ui::FormCustermPlotChart *ui;
};

#endif // FORMCUSTERMPLOTCHART_H
