#include "formcustermplotchart.h"
#include "ui_formcustermplotchart.h"

FormCustermPlotChart::FormCustermPlotChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCustermPlotChart)
{
    ui->setupUi(this);
}

FormCustermPlotChart::~FormCustermPlotChart()
{
    delete ui;
}
