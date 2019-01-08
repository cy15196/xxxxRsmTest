#include "dialogchartaxissetting.h"
#include "ui_dialogchartaxissetting.h"

DialogChartAxisSetting::DialogChartAxisSetting(const QString &axisTypeString, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogChartAxisSetting)
{
    ui->setupUi(this);
    ui->groupBox_time->setTitle(axisTypeString);
}

DialogChartAxisSetting::~DialogChartAxisSetting()
{
    delete ui;
}

void DialogChartAxisSetting::on_doubleSpinBox_rangeSmall_valueChanged(double arg1)
{

}

void DialogChartAxisSetting::on_doubleSpinBox_rangeBig_valueChanged(double arg1)
{

}

void DialogChartAxisSetting::setAxisInfo(const TopTest::AxisLabelInfo &value)
{
    axisInfo = value;

    ui->lineEdit_Title->setText(axisInfo.axisTitle);
    ui->checkBoxTitleVisible->setChecked(axisInfo.showAxisTitle);

    ui->doubleSpinBox_rangeSmall->setRange(-1e6,1e6);
    ui->doubleSpinBox_rangeBig->setRange(-1e6,1e6);

    ui->doubleSpinBox_rangeSmall->setValue(axisInfo.range.x());
    ui->doubleSpinBox_rangeBig->setValue(axisInfo.range.y());

    ui->spinBox_tickCount->setRange(4,20);
    ui->spinBox_minorTickCount->setRange(1,5);

    ui->spinBox_tickCount->setValue(axisInfo.bigTickCount);
    ui->spinBox_minorTickCount->setValue(axisInfo.smallTickCountPerBigStep);
}

TopTest::AxisLabelInfo DialogChartAxisSetting::getAxisInfo()
{
    axisInfo.axisTitle = ui->lineEdit_Title->text();
    axisInfo.showAxisTitle = ui->checkBoxTitleVisible->isChecked();

    axisInfo.range = QPointF(ui->doubleSpinBox_rangeSmall->value(),ui->doubleSpinBox_rangeBig->value());
    axisInfo.bigTickCount = ui->spinBox_tickCount->value();
    axisInfo.smallTickCountPerBigStep = ui->spinBox_minorTickCount->value();

    return axisInfo;
}
