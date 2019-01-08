#pragma execution_character_set("utf-8")

#include "formheadertable.h"
#include "ui_formheadertable.h"

#include <QDebug>

#include "toptestenum.h"
#include "color_list_item_delegate.h"

#include "clabso_ft_dpi.h"

FormHeaderTable::FormHeaderTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormHeaderTable)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    changeToType(TopTest::MaxTemputure);

    ui->tableWidget->setFixedWidth(ClabsoFTDpi::convertSize(800));

}

FormHeaderTable::~FormHeaderTable()
{
    delete ui;
}

void FormHeaderTable::changeToType(int type)
{
    ui->tableWidget->clear();

    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    while(ui->tableWidget->columnCount())
        ui->tableWidget->removeColumn(0);

    switch (type) {
    case TopTest::MaxTemputure:
    case TopTest::MaxIncreaseTime:
    case TopTest::MaxDecreaseTime:
    case TopTest::MaxPortMaxDeltaValue:
    {
        ui->tableWidget->setColumnCount(6);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,200);
        ui->tableWidget->setColumnWidth(3,200);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
//        ui->tableWidget->setSpan(0,2,2,2);
//        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("Max 温度 (C)")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("参照标准 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,4,2,1);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,5,2,1);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(4,40);
        ui->tableWidget->setColumnWidth(5,80);
//        ui->tableWidget->setColumnWidth(6,80);
    }
        break;
    case TopTest::IncreaseTime:
//    case TopTest::RemainTime:
    case TopTest::TestValuedTime:
    case TopTest::TemputureBetweenTime:
    {
        ui->tableWidget->setColumnCount(8);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,4,1,2);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("参照标准 (C)")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,5,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,7,2,1);
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,8,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(6,40);
        ui->tableWidget->setColumnWidth(7,80);
//        ui->tableWidget->setColumnWidth(8,80);
    }
        break;
    case TopTest::AboveTime:
//    case TopTest::ReturnTime:
    {
        ui->tableWidget->setColumnCount(7);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,200);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,2,1);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
//        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("下限")));
//        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,3,1,2);
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("参照标准 (S)")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,5,2,1);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(5,40);
        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(7,80);
    }
        break;
    case TopTest::DecreaseTime:
    {
        ui->tableWidget->setColumnCount(8);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setSpan(0,4,1,2);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("参照标准 (S)")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,5,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,7,2,1);
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,8,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(6,40);
        ui->tableWidget->setColumnWidth(7,80);
    }
        break;
    case TopTest::IncreaseRatio:
    {
        ui->tableWidget->setColumnCount(9);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,80);
        ui->tableWidget->setColumnWidth(3,80);
        ui->tableWidget->setColumnWidth(4,80);
        ui->tableWidget->setColumnWidth(5,80);
        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(5,80);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,4,2,1);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("斜率计算时间 (S)")));
        ui->tableWidget->setSpan(0,5,1,2);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("参照标准 (C/S)")));
        ui->tableWidget->setItem(1,5,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,6,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,7,2,1);
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,8,2,1);
        ui->tableWidget->setItem(0,8,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,9,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(7,40);
        ui->tableWidget->setColumnWidth(8,80);
//        ui->tableWidget->setColumnWidth(9,80);
    }
        break;
    case TopTest::DecreaseRatio:
    {
        ui->tableWidget->setColumnCount(9);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,80);
        ui->tableWidget->setColumnWidth(3,80);
        ui->tableWidget->setColumnWidth(4,80);
        ui->tableWidget->setColumnWidth(5,80);
        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(5,80);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setSpan(0,4,2,1);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("斜率计算时间 (S)")));
        ui->tableWidget->setSpan(0,5,1,2);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("参照标准 (C/S)")));
        ui->tableWidget->setItem(1,5,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,6,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,7,2,1);
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,8,2,1);
        ui->tableWidget->setItem(0,8,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,9,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(7,40);
        ui->tableWidget->setColumnWidth(8,80);
//        ui->tableWidget->setColumnWidth(9,80);
    }
        break;
    case TopTest::MaxIncreaseRatio:
    case TopTest::MaxDecreaseRatio:
    {
        ui->tableWidget->setColumnCount(7);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,200);
//        ui->tableWidget->setColumnWidth(3,200);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,2,1);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("斜率计算时间 (S)")));
//        ui->tableWidget->setSpan(0,2,2,2);
//        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("Max 温度")));
        ui->tableWidget->setSpan(0,3,1,2);
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("参照标准 (C/S)")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,5,2,1);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(5,40);
        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(6,80);
    }
        break;
    case TopTest::AverageRatio:
    {
        ui->tableWidget->setColumnCount(8);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,80);
        ui->tableWidget->setColumnWidth(3,80);
        ui->tableWidget->setColumnWidth(4,80);
        ui->tableWidget->setColumnWidth(5,80);
//        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(5,80);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,1,2);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("温度 (C)")));
        ui->tableWidget->setItem(1,2,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("上限")));
//        ui->tableWidget->setSpan(0,4,2,1);
//        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("斜率计算时间 (S)")));
        ui->tableWidget->setSpan(0,4,1,2);
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("参照标准 (C/S)")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,5,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,7,2,1);
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,9,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(6,40);
        ui->tableWidget->setColumnWidth(7,80);
//        ui->tableWidget->setColumnWidth(9,80);
    }
        break;
//    case TopTest::MaxIncreaseTime:
//    {
//        ui->tableWidget->setColumnCount(7);
////        ui->tableWidget->setColumnWidth(0,60);
//        ui->tableWidget->setColumnWidth(1,40);
//        ui->tableWidget->setColumnWidth(2,200);
//        ui->tableWidget->insertRow(0);
//        ui->tableWidget->insertRow(1);
//        ui->tableWidget->setSpan(0,0,2,1);
//        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
//        ui->tableWidget->setSpan(0,1,2,1);
//        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
//        ui->tableWidget->setSpan(0,2,2,1);
//        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("Max 温度")));
//        ui->tableWidget->setSpan(0,3,1,2);
//        ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("参照标准")));
//        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
//        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("上限")));
//        ui->tableWidget->setSpan(0,5,2,1);
//        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("警报")));
//        ui->tableWidget->setSpan(0,6,2,1);
//        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("热电偶选择")));
////        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("波形选择")));
//        ui->tableWidget->setColumnWidth(5,40);
//        ui->tableWidget->setColumnWidth(6,80);
////        ui->tableWidget->setColumnWidth(7,80);
//    }
//        break;
    case TopTest::DistanceToMaxTimeSection:
    {
        ui->tableWidget->setColumnCount(7);
//        ui->tableWidget->setColumnWidth(0,60);
        ui->tableWidget->setColumnWidth(1,40);
        ui->tableWidget->setColumnWidth(2,200);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->insertRow(1);
        ui->tableWidget->setSpan(0,0,2,1);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
        ui->tableWidget->setSpan(0,1,2,1);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
        ui->tableWidget->setSpan(0,2,2,1);
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("Max ± 温度 (C)")));
        ui->tableWidget->setSpan(0,3,1,2);
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("参照标准 (S)")));
        ui->tableWidget->setItem(1,3,new QTableWidgetItem(tr("下限")));
        ui->tableWidget->setItem(1,4,new QTableWidgetItem(tr("上限")));
        ui->tableWidget->setSpan(0,5,2,1);
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("警报")));
        ui->tableWidget->setSpan(0,6,2,1);
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setItem(0,7,new QTableWidgetItem(tr("波形选择")));
        ui->tableWidget->setColumnWidth(5,40);
        ui->tableWidget->setColumnWidth(6,80);
//        ui->tableWidget->setColumnWidth(7,80);
    }
        break;
//    case TopTest::PinNameAndColor:
//    {
//        ui->tableWidget->setColumnCount(6);
////        ui->tableWidget->setColumnWidth(0,60);
//        ui->tableWidget->setColumnWidth(1,40);
//        ui->tableWidget->setColumnWidth(2,200);
//        ui->tableWidget->setColumnWidth(3,200);
//        ui->tableWidget->insertRow(0);
//        ui->tableWidget->insertRow(1);
//        ui->tableWidget->setSpan(0,0,2,1);
//        ui->tableWidget->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
//        ui->tableWidget->setSpan(0,1,2,1);
//        ui->tableWidget->setItem(0,1,new QTableWidgetItem(tr("序号")));
//        ui->tableWidget->setSpan(0,2,2,1);
//        ui->tableWidget->setItem(0,2,new QTableWidgetItem(tr("通道名称")));
//        ui->tableWidget->setSpan(0,3,2,1);
//        ui->tableWidget->setItem(0,3,new QTableWidgetItem(tr("通道颜色")));
//        ui->tableWidget->setSpan(0,4,2,1);
//        ui->tableWidget->setItem(0,4,new QTableWidgetItem(tr("警报")));
//        ui->tableWidget->setItem(0,5,new QTableWidgetItem(tr("热电偶选择")));
//        ui->tableWidget->setSpan(0,6,2,1);
//        ui->tableWidget->setItem(0,6,new QTableWidgetItem(tr("波形选择")));
//        ui->tableWidget->setColumnWidth(5,40);
//        ui->tableWidget->setColumnWidth(5,80);
////        ui->tableWidget->setColumnWidth(6,80);
//    }
//        break;
    default:
        break;
    }

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            QTableWidgetItem* item = ui->tableWidget->item(i,j);
            if(item)
                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }

        ui->tableWidget->setRowHeight(i,ClabsoFTDpi::convertSize(30));
    }
    ui->tableWidget->setMaximumHeight(ui->tableWidget->rowCount() *
                                      ClabsoFTDpi::convertSize(30) +
                                      ClabsoFTDpi::convertSize(1));


    ui->tableWidget->setColumnWidth(0,ClabsoFTDpi::convertSize(120));

    emit this->changedToType(type);
}

void FormHeaderTable::changeEvent(QEvent *event)
{
    qDebug()<<"on header table change event";

    if (event->type() == QEvent::LanguageChange)
    {
        changeToType(TopTest::MaxTemputure);
    }else
         QWidget::changeEvent(event);
}
