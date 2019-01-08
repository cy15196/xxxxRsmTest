#include "formstatetime.h"
#include "ui_formstatetime.h"

FormStateTime::FormStateTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStateTime)
{
    ui->setupUi(this);

    ui->tableWidget_Header->horizontalHeader()->setVisible(false);
    ui->tableWidget_Header->verticalHeader()->setVisible(false);
    ui->tableWidget_Header->setColumnCount(7);
    ui->tableWidget_Header->insertRow(0);
    ui->tableWidget_Header->insertRow(1);
    ui->tableWidget_Header->setSpan(0,0,2,1);
    ui->tableWidget_Header->setItem(0,0,new QTableWidgetItem(tr("分析项目")));
    ui->tableWidget_Header->setSpan(0,1,2,1);
    ui->tableWidget_Header->setItem(0,1,new QTableWidgetItem(tr("序号")));
    ui->tableWidget_Header->setSpan(0,2,1,2);
    ui->tableWidget_Header->setItem(0,2,new QTableWidgetItem(tr("温度")));
    ui->tableWidget_Header->setItem(1,2,new QTableWidgetItem(tr("下限")));
    ui->tableWidget_Header->setItem(1,3,new QTableWidgetItem(tr("上限")));
    ui->tableWidget_Header->setSpan(0,4,1,2);
    ui->tableWidget_Header->setItem(0,4,new QTableWidgetItem(tr("参照标准")));
    ui->tableWidget_Header->setItem(1,4,new QTableWidgetItem(tr("下限")));
    ui->tableWidget_Header->setItem(1,5,new QTableWidgetItem(tr("上限")));
    ui->tableWidget_Header->setSpan(0,6,2,1);
    ui->tableWidget_Header->setItem(0,6,new QTableWidgetItem(tr("警报")));

    reset();
}

FormStateTime::~FormStateTime()
{
    delete ui;
}

void FormStateTime::reset()
{
    ui->tableWidget_Data->clear();


//    ui->tableWidget->setItem(0);
}
