#include "formdistanceindicator.h"

#include <QValueAxis>
#include <QChartView>
#include <QChar>
#include <QHBoxLayout>
#include <QLabel>

#include "clabso_ft_dpi.h"

QT_CHARTS_USE_NAMESPACE

class FormDistanceIndicatorPrivate
{
public:
    QValueAxis* disAxis;
};

FormDistanceIndicator::FormDistanceIndicator(QWidget *parent) :
    QWidget(parent),d_ptr(new FormDistanceIndicatorPrivate)
{
    QHBoxLayout* layout = new QHBoxLayout;
    this->setLayout(layout);

    QLabel* textLabel = new QLabel(tr("Distance"),this);
    layout->addWidget(textLabel);

    QChart* chart = new QChart;
    QChartView* view = new QChartView(chart,this);
    layout->addWidget(view);

    chart->createDefaultAxes();
    this->setMaximumHeight(ClabsoFTDpi::convertSize(200.00));

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 6.5);    //设置范围
    axisX->setLabelFormat("%.2f");   //设置刻度的格式
    axisX->setGridLineVisible(true);   //网格线可见
    axisX->setTickCount(10);       //设置多少格
    axisX->setMinorTickCount(5);   //设置每格小刻度线的数目

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 10);
    axisY->setLabelFormat("%u");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(10);
    axisY->setMinorTickCount(1);

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    d_ptr->disAxis = axisX;
}

FormDistanceIndicator::~FormDistanceIndicator()
{
    delete d_ptr;
}

void FormDistanceIndicator::setWholeDiatance(const double &value)
{

}
