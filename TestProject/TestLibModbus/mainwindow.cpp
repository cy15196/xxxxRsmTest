#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

#include "modbus.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(200, 60);
    this->setWindowTitle("libmodbus slave test");

    pbRtu=new QPushButton(tr("以modbus rtu方式读取地址1中前20个寄存器值"), this);
    pbRtu->resize(200, 30);

    pbTcp=new QPushButton(tr("以modbus tcp方式读取地址1中前20个寄存器值"), this);
    pbTcp->resize(200, 30);

    QWidget *w=new QWidget();

    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(pbRtu);
    layout->addWidget(pbTcp);

    w->setLayout(layout);

    this->setCentralWidget(w);

    connect(pbRtu, SIGNAL(clicked()), this, SLOT(doRtuQuery()));
    connect(pbTcp, SIGNAL(clicked()), this, SLOT(doTcpQuery()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::doRtuQuery()
{
    modbus_t *mb;
    uint16_t tab_reg[32]={0};

    qDebug()<<"modbus_new_rtu"<<int(mb = modbus_new_rtu("COM4", 500000, 'N', 8, 1));   //相同的端口只能同时打开一个
    qDebug()<<"modbus_set_slave"<<modbus_set_slave(mb, 0);  //设置modbus从机地址
    qDebug()<<"connect modbus "<<modbus_connect(mb);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒
    uint32_t t_sec=0,t_usec = 1000000;

    modbus_set_response_timeout(mb, t_sec , t_usec);

    int regs=modbus_read_registers(mb, 0, 20, tab_reg);

    QMessageBox::about(NULL, tr("报告"), tr("Rtu读取寄存器的个数:%1").arg(regs));

    qDebug()<<QString::number(tab_reg[0],16);

    modbus_close(mb);
    modbus_free(mb);
}

void MainWindow::doTcpQuery()
{
    modbus_t *mb;
    uint16_t tab_reg[32]={0};

    mb = modbus_new_tcp("127.0.0.1", 5101);  //由于是tcp client连接，在同一个程序中相同的端口可以连接多次。
    modbus_set_slave(mb, 1);  //从机地址

    modbus_connect(mb);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒，注意：经测试，如果没有成功建立tcp连接，则该设置无效。

    uint32_t t_sec=0,t_usec = 1000000;

    modbus_set_response_timeout(mb, t_sec , t_usec);


    int regs=modbus_read_registers(mb, 0, 20, tab_reg);

    QMessageBox::about(NULL, tr("报告"), tr("Tcp读取寄存器的个数:%1").arg(regs));
    modbus_close(mb);
    modbus_free(mb);
}
