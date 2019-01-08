#include "dialogdatadownload.h"
#include "ui_dialogdatadownload.h"

#include <QCheckBox>
#include "toptestusb.h"
#include "dataiomanager.h"
#include <QDir>

void initRowTable(QTableWidget* rowTable)
{
    rowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rowTable->setColumnCount(5);
    rowTable->setRowCount(4);
    rowTable->horizontalHeader()->setStretchLastSection(true);
    rowTable->verticalHeader()->setStretchLastSection(true);

    rowTable->setColumnWidth(0,80);

//    rowTable->setColumnWidth(2,90);
//    rowTable->setColumnWidth(3,90);
//    rowTable->setColumnWidth(4,90);

    for(int i=0;i<rowTable->rowCount();i++)
    {
        QTableWidgetItem * item = new QTableWidgetItem;
        item->setCheckState(Qt::Checked);
        item->setText(QString::number(i));
        rowTable->setItem(i,0,item);
    }

//    rowTable->horizontalHeader()->setVisible(false);
    rowTable->verticalHeader()->setVisible(false);
    rowTable->setHorizontalHeaderLabels(QStringList()
//        <<QObject::tr(QString::fromStdWString(L"启用").toUtf8().data())
        <<QObject::tr(QString::fromStdWString(L"序号").toUtf8().data())
        <<QObject::tr(QString::fromStdWString(L"测试时间").toUtf8().data())
        <<QObject::tr(QString::fromStdWString(L"采样频率").toUtf8().data())
        <<QObject::tr(QString::fromStdWString(L"数据量").toUtf8().data())
        <<QObject::tr(QString::fromStdWString(L"通道数").toUtf8().data()));



//    for(int i=0;i<rowTable->rowCount();i++)
//        for(int j=0;j<rowTable->columnCount();j++)
//        {
//            QTableWidgetItem* item = rowTable->item(i,j);
//            if(item)
//                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        }
}

void initPortTable(QTableWidget* portTable)
{
    portTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    portTable->setColumnCount(16);
    portTable->setRowCount(1);
    portTable->horizontalHeader()->setStretchLastSection(true);
    portTable->verticalHeader()->setStretchLastSection(true);

    portTable->verticalHeader()->setVisible(false);

    for(int i=0;i<16;i++)
    {
        portTable->setColumnWidth(i,30);
        QTableWidgetItem * item = new QTableWidgetItem;
        item->setCheckState(Qt::Checked);
        portTable->setItem(0,i,item);
    }
}


DialogDataDownload::DialogDataDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDataDownload)
{
    ui->setupUi(this);

    initRowTable(ui->tableWidget_DataList);
    initPortTable(ui->tableWidget_PortChoose);

    refreshData();

}

DialogDataDownload::~DialogDataDownload()
{
    delete ui;
}

void DialogDataDownload::refreshData()
{
    for(int i=0;i<4;i++)
    {
        int dataCount = TopTestUsb::instance()->ReadRowSampleLength(i);
        double sampleTime = TopTestUsb::instance()->ReadSampleTime();
        int portNum = 16;
        double testTime = sampleTime*dataCount;

        ui->tableWidget_DataList->setItem(i,1,new QTableWidgetItem(QString::number(testTime)));
        ui->tableWidget_DataList->setItem(i,2,new QTableWidgetItem(QString::number(sampleTime)));
        ui->tableWidget_DataList->setItem(i,3,new QTableWidgetItem(QString::number(dataCount)));
        ui->tableWidget_DataList->setItem(i,4,new QTableWidgetItem(QString::number(portNum)));

    }
}

void DialogDataDownload::accept()
{
    ///读取设备上的数据
    int activeRaw = 1;
//    foreach (int rowNo, activeRaw) {
       QMap<int, QList<double>> rawDataMap = TopTestUsb::instance()->ReadRowSampleData(activeRaw);
//    }
    ///保存成曲线文件
    TopTestFileDataMap fileDataMap;
    foreach (int portNo, rawDataMap.keys()) {
        fileDataMap.insert("Port"+QString::number(portNo),rawDataMap[portNo]);
    }

    double sampleTime = 0.05;
    int dataCount = fileDataMap.value("Port1").count();

    QList<double> timeList;
    for(int i=0;i<dataCount;i++)
    {
        timeList.append(i*sampleTime);
    }

    fileDataMap.insert("Time",timeList);

    DataIOManager::saveDataToFile(fileDataMap,QDir::tempPath()+"/test.tpd");

}
