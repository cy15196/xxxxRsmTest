#include "dialogprogresshandle.h"
#include "ui_dialogprogresshandle.h"

#include <QFileDialog>

DialogProgressHandle::DialogProgressHandle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgressHandle)
{
    ui->setupUi(this);

    connect(ui->listWidget_ProgressType,SIGNAL(currentRowChanged(int)),
            ui->widget_HeaderTable,SLOT(changeToType(int)));

    connect(ui->listWidget_ProgressType,SIGNAL(currentRowChanged(int)),
            ui->widget_DataTable,SLOT(changeToType(int)));

    connect(ui->widget_HeaderTable,SIGNAL(changedToType(int)),
            this,SLOT(on_changedToType(int)));

    connect(ui->widget_DataTable,SIGNAL(tableEdited(int)),
            this,SLOT(onTableDataEdited(int)));

    connect(ui->widget_DataTable,SIGNAL(dataSaved()),
            this,SLOT(onDataSaved()));

    connect(ui->toolButton_Save,SIGNAL(clicked(bool)),
            ui->widget_DataTable,SLOT(saveTypeData()));

    ui->comboBox_TimeUnit->addItem(tr(QString::fromStdWString(L"分钟").toUtf8().data()),TopTest::min);
    ui->comboBox_TimeUnit->addItem(tr(QString::fromStdWString(L"秒").toUtf8().data()),TopTest::s);

    initPortChoose();
}

DialogProgressHandle::~DialogProgressHandle()
{
    delete ui;
}

void DialogProgressHandle::on_changedToType(int type)
{

}

void DialogProgressHandle::onTableDataEdited(int type)
{
    QListWidgetItem* item =
        ui->listWidget_ProgressType->item(type);
    if(!item->text().endsWith("*"))
        item->setText(item->text()+"*");
}

void DialogProgressHandle::onDataSaved()
{
    for(int i=0;i<ui->listWidget_ProgressType->count();i++)
    {
        QListWidgetItem* item =
            ui->listWidget_ProgressType->item(i);
        if(item->text().endsWith("*"))
            item->setText(item->text().left(item->text().length()-1));
    }
}

void DialogProgressHandle::initPortChoose()
{
    ui->tableWidget_PortChoose->setSpan(0,0,2,1);
    ui->tableWidget_PortChoose->setSpan(0,1,2,1);
    ui->tableWidget_PortChoose->setColumnWidth(0,100);
    ui->tableWidget_PortChoose->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_PortChoose->verticalHeader()->setStretchLastSection(true);

    for(int i=1;i<17;i++)
        ui->tableWidget_PortChoose->setColumnWidth(i,32);

    for(int i=0;i<ui->tableWidget_PortChoose->rowCount();i++)
        for(int j=0;j<ui->tableWidget_PortChoose->columnCount();j++)
        {
            QTableWidgetItem* item = ui->tableWidget_PortChoose->item(i,j);
            if(item)
                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
}


void DialogProgressHandle::on_toolButton_Pic_clicked()
{
    QString pictureFilePath = QFileDialog::getOpenFileName(
                this,"Choose Picture File",
                QDir::homePath()+"/Pictures",
                "Picture files (*.png *.jpg *.jpeg)");
    if(!pictureFilePath.isEmpty())
    {
        QPixmap pixmap(pictureFilePath);
        pixmap = pixmap.scaledToHeight(ui->toolButton_Pic->height());
        ui->toolButton_Pic->setIcon(QIcon(pixmap));
        ui->toolButton_Pic->setIconSize(ui->toolButton_Pic->size());
    }
}
