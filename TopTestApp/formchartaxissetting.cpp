#include "formchartaxissetting.h"

#include "dialogchartaxissetting.h"
#include <QVBoxLayout>

FormChartAxisSetting::FormChartAxisSetting(QWidget *parent)
    : DialogBase(parent)
{
    this->setLayout(new QVBoxLayout);

    xAxisDC = new DialogChartAxisSetting(tr("Time"), this);
    yAxisDC = new DialogChartAxisSetting(tr("Temperature"), this);
    dAxisDC = new DialogChartAxisSetting(tr("Distance"), this);

    this->layout()->addWidget(xAxisDC);
    this->layout()->addWidget(yAxisDC);
    this->layout()->addWidget(dAxisDC);
}

FormChartAxisSetting::~FormChartAxisSetting()
{

}

void FormChartAxisSetting::setAxisLableInfos(const QMap<QString,TopTest::AxisLabelInfo>& labelInfoMap)
{
    xAxisDC->setAxisInfo(labelInfoMap["X"]);
    yAxisDC->setAxisInfo(labelInfoMap["Y"]);
    dAxisDC->setAxisInfo(labelInfoMap["D"]);
}

void FormChartAxisSetting::onButtonClicked(int button)
{
    QMap<QString,TopTest::AxisLabelInfo> labelInfoMap;
    labelInfoMap["X"] = xAxisDC->getAxisInfo();
    labelInfoMap["Y"] = yAxisDC->getAxisInfo();
    labelInfoMap["D"] = dAxisDC->getAxisInfo();
    switch (button) {
    case QDialogButtonBox::Ok:
    case QDialogButtonBox::Yes:
    {
        emit axisLabelInfoChanged(labelInfoMap);
    }
        break;
    default:
        break;
    }
}
