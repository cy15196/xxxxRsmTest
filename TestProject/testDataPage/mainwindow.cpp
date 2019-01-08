#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qcpchartwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCPChartWidget* chart = new QCPChartWidget(this);
    chart->setSampleTimeInteral(0.1);
    chart->addOneSampleData(QList<double>() << 10 << 20 << 30 << 40 << 50 << 60 , 0.1);
    chart->addOneSampleData(QList<double>() << 100 << 200 << 300 << 400 << 500 << 600 , 1100);
//    chart->addOneSampleData(QList<double>() << 10 << 20 << 30 << 40 << 50 << 60 , 0);
//    chart->addOneSampleData(QList<double>() << 10 << 20 << 30 << 40 << 50 << 60 , 0);
    chart->reFit();
    this->setCentralWidget(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}
