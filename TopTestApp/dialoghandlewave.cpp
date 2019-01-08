#include "dialoghandlewave.h"
#include "ui_dialoghandlewave.h"

#include "topteststructdata.h"

DialogHandleWave::DialogHandleWave(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogHandleWave)
{
    ui->setupUi(this);
    ui->tableWidget->setEnabled(false);
    ui->checkBox->setCheckable(true);
}

DialogHandleWave::~DialogHandleWave()
{
    delete ui;
}

void DialogHandleWave::setWaveInfo(const TopTest::WaveInfo &waveInfo)
{

    connect(ui->checkBox,SIGNAL(toggled(bool)),
            ui->tableWidget,SLOT(setEnabled(bool)));


    ui->checkBox->setChecked(waveInfo.useHandle);


    for(int i = 0; i < waveInfo.waveCount; i++)
    {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem* nameItem = new QTableWidgetItem;
        nameItem->setData(Qt::DisplayRole, waveInfo.waveNameMap[i+1]);
        nameItem->setData(Qt::DisplayRole,tr("波段 %1").arg(i+1));
        nameItem->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(i,0,nameItem);

        QPointF timePoint = waveInfo.waveTimeSection[i+1];
        QTableWidgetItem* startItem = new QTableWidgetItem;
        startItem->setData(Qt::DisplayRole, timePoint.x());
        ui->tableWidget->setItem(i,1,startItem);

        QTableWidgetItem* endItem = new QTableWidgetItem;
        endItem->setData(Qt::DisplayRole, timePoint.y());
        ui->tableWidget->setItem(i,2,endItem);
    }

}

void DialogHandleWave::onButtonClicked(int role)
{
    if(role == QDialogButtonBox::Ok)
    {
        TopTest::WaveInfo waveInfo;
        waveInfo.useHandle = ui->checkBox->isChecked();
        waveInfo.waveCount = ui->tableWidget->rowCount();
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
            QString name = ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toString();
            double start =  ui->tableWidget->item(i,1)->data(Qt::DisplayRole).toDouble();
            double end =  ui->tableWidget->item(i,2)->data(Qt::DisplayRole).toDouble();

            waveInfo.waveNameMap[i+1] = name;
            waveInfo.waveTimeSection[i+1] = QPointF(start,end);
        }
        emit this->waveInfoChanged(waveInfo);
    }
}
