#include "dialogopenfile.h"
#include "ui_dialogopenfile.h"

#include "toptestsettingmanager.h"
#include "clabso_ft_dpi.h"

#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>

DialogOpenFile::DialogOpenFile(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogOpenFile)
{
    ui->setupUi(this);

    this->setMinimumSize(ClabsoFTDpi::convertSize(QSize(750,400)));

    init();

    connect(ui->tableWidget_files->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(record_sortbyclounm(int)));
}

DialogOpenFile::~DialogOpenFile()
{
    delete ui;
}

int DialogOpenFile::standerButtons()
{
    return QDialogButtonBox::Ok;
}

void DialogOpenFile::onButtonClicked(int button)
{

}


void DialogOpenFile::on_listWidget_product_currentRowChanged(int currentRow)
{
    qDebug()<<"on_listWidget_product_currentRowChanged"<<currentRow;
    QListWidgetItem * item = ui->listWidget_product->item(currentRow);
    if(!item)
        return;

    while(ui->tableWidget_files->rowCount())
        ui->tableWidget_files->removeRow(0);

    QString currentPath = item->text();
    QDir dir(workDir + "/" + currentPath);
    foreach (QFileInfo file, dir.entryInfoList({"*.rsm"},QDir::Files)) {

        int row = ui->tableWidget_files->rowCount();
        ui->tableWidget_files->insertRow(row);

        QTableWidgetItem* nameItem = new QTableWidgetItem;
        nameItem->setData(Qt::DisplayRole,file.completeBaseName());
        nameItem->setData(Qt::UserRole,file.absoluteFilePath());
        ui->tableWidget_files->setItem(row,0,nameItem);

        QTableWidgetItem* createTimeItem = new QTableWidgetItem;
        createTimeItem->setData(Qt::DisplayRole,file.created());
        ui->tableWidget_files->setItem(row,1,createTimeItem);

        QTableWidgetItem* editTimeItem = new QTableWidgetItem;
        editTimeItem->setData(Qt::DisplayRole,file.lastModified());
        ui->tableWidget_files->setItem(row,2,editTimeItem);

        QTableWidgetItem* sizeItem = new QTableWidgetItem;
        sizeItem->setData(Qt::DisplayRole,file.size());
        ui->tableWidget_files->setItem(row,3,sizeItem);

    }


}

void DialogOpenFile::record_sortbyclounm(int column)
{
    ui->tableWidget_files->sortByColumn(column);
}

void DialogOpenFile::init()
{
    ui->tableWidget_files->setColumnWidth(0,200);
    ui->tableWidget_files->setEditTriggers(QTableWidget::NoEditTriggers);

    //work dir
    TopTestSettingManager::instance()->loadFromSystemSettings();
    workDir = TopTestSettingManager::instance()->getSystemSettingKeyValue("WorkDir:").toString();
    ui->lineEdit_workDir->setText(workDir);

    //product dir list
    QDir dir(workDir);
    if(!dir.exists())
        return;
    ui->listWidget_product->addItems(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    if(ui->listWidget_product->count())
        ui->listWidget_product->setCurrentRow(0);

    //maybe current product

}

void DialogOpenFile::on_tableWidget_files_itemDoubleClicked(QTableWidgetItem *item)
{
    if(!item)
        return;

    int row = item->row();

    QTableWidgetItem* nameItem = ui->tableWidget_files->item(row,0);
    if(!nameItem)
        return;
    qDebug()<<"row double clicked"<<row<<nameItem->data(Qt::UserRole).toString();
    emit this->needToOpenFile(nameItem->data(Qt::UserRole).toString());
}

void DialogOpenFile::on_pushButton_DirectOpen_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"),
                                 "D:/","RSM files (*.rsm);;");
    if(!filePath.isEmpty() && QFile::exists(filePath))
    {
         emit this->needToOpenFile(filePath);
    }
}
