#pragma execution_character_set("utf-8")

#include "formdevicecontent.h"
#include "ui_formdevicecontent.h"

#include "clabso_ft_dpi.h"
#include "toptestcore.h"

#include <QDebug>
#include <QMessageBox>

FormDeviceContent::FormDeviceContent(QWidget *parent) :
    FileSettingWidget(parent),
    ui(new Ui::FormDeviceContent)
{
    ui->setupUi(this);
    init();
}

FormDeviceContent::~FormDeviceContent()
{
    delete ui;
}

QString FormDeviceContent::getFileTypeDisplayText()
{
    return tr("设备文件设置");
}

QString FormDeviceContent::getFileTypeSettingKeyName()
{
    return "Device";
}

void FormDeviceContent::loadSettigRecord(const QVariant &recordName)
{
    QMap<QString,QVariant> recordInfoMap = readDataFromSettingFile(recordName.toString()).toMap();

    if(recordInfoMap.isEmpty())
    {
        qDebug()<<"no such record";
        return;
    }

    if(recordInfoMap.value("Name") != recordName)
    {
        qDebug()<<"record is not valid";
        return;
    }

    ui->lineEdit_name->setText(recordInfoMap["Name"].toString());
    loadWidgetContentMap(recordInfoMap);

    adjustColNo();

    onSingleRangChanged(ui->radioButton_3->isChecked());

//    ui->label_pic->setPixmap(recordInfoMap["Pic"].toString());
}

void FormDeviceContent::saveCurrentSettingRecord()
{
    if(!judgeSave())
    {
        emit this->contentChanged();
        return;
    }


    QMap<QString,QVariant> recordInfoMap = saveWidgetContentToMap();
    //save content
    QString recordName = ui->lineEdit_name->text();
    recordInfoMap.insert("Name",recordName);
//    recordInfoMap.insert("Pic",ui->label_pic->pixmap());
    qDebug()<<getFileTypeSettingKeyName()<<recordName<<recordInfoMap;
    //save record
    writeDataToSettingFile(recordName,recordInfoMap);
}

void FormDeviceContent::setCurrentRecordName(const QString &label)
{
    ui->lineEdit_name->setText(label);
}

QString FormDeviceContent::getCurrentRecordName()
{
    return ui->lineEdit_name->text();
}

void FormDeviceContent::clearContent()
{
    foreach (QLineEdit* edit, this->findChildren<QLineEdit*>()) {
        edit->clear();
    }


    ui->spinBox_num->setValue(5);
    ui->doubleSpinBox_velocity->setValue(1.00);
    ui->doubleSpinBox_startTemp->setValue(40);
    ui->doubleSpinBox_endTemp->setValue(50);
    ui->spinBox_length->setValue(10);

    ui->radioButton_2->setChecked(true);
    ui->radioButton_3->setChecked(true);

    ui->tableWidget_device->clearContents();

    onSingleRangChanged(ui->radioButton_3->isChecked());

//    foreach (QRadioButton* edit, this->findChildren<QRadioButton*>()) {
//        edit->
//    }

    //    ui->tableWidget_device->clear();
}

void FormDeviceContent::onSingleRangChanged(bool isSingle)
{
    ui->label_7->setVisible(!isSingle);
    ui->radioButton->setVisible(!isSingle);
    ui->radioButton_2->setVisible(!isSingle);

    if(isSingle)
    {
        ui->tableWidget_device->setVerticalHeaderItem(
                    3,new QTableWidgetItem( tr("温区温度") ));
        ui->tableWidget_device->setRowHeight(4,0);
    }
    else
    {
        ui->tableWidget_device->setVerticalHeaderItem(
                    3, new QTableWidgetItem(tr("上温区")) );
        ui->tableWidget_device->setRowHeight(4,30);
    }

//    ui->tableWidget_device->set
}

void FormDeviceContent::init()
{
    ui->spinBox_num->setValue(5);
    ui->doubleSpinBox_velocity->setValue(1.00);
    ui->doubleSpinBox_startTemp->setValue(40.0);
    ui->doubleSpinBox_endTemp->setValue(50.0);
    ui->spinBox_length->setValue(10);

    ui->radioButton_2->setChecked(true);
    ui->radioButton_3->setChecked(true);

    ui->label_7->setVisible(false);
    ui->radioButton->setVisible(false);
    ui->radioButton_2->setVisible(false);



    ui->tableWidget_device->setFixedHeight(
           ClabsoFTDpi::convertSize(30) * ui->tableWidget_device->rowCount() + 30);

    ui->tableWidget_device->setRowCount(5);
    ui->tableWidget_device->setColumnCount(5);

    foreach (QLineEdit* edit, this->findChildren<QLineEdit*>()) {
        connect(edit,SIGNAL(textChanged(QString)),
                this,SIGNAL(contentChanged()));
    }

    foreach (QRadioButton* edit, this->findChildren<QRadioButton*>()) {
        connect(edit,SIGNAL(clicked(bool)),
                this,SIGNAL(contentChanged()));
    }

    connect(ui->tableWidget_device,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SIGNAL(contentChanged()));

    connect(ui->radioButton_3,SIGNAL(toggled(bool)),
            SLOT(onSingleRangChanged(bool)));

}

void FormDeviceContent::adjustColNo()
{
    for(int i =0 ; i< ui->tableWidget_device->columnCount();i++)
    {
        QTableWidgetItem* noItem = new QTableWidgetItem;
        noItem->setData(Qt::DisplayRole,i+1);
        noItem->setFlags(Qt::NoItemFlags);
        ui->tableWidget_device->setItem(0,i,noItem);
    }
}

bool FormDeviceContent::judgeSave()
{
    double len = ui->spinBox_length->value();
    double sum = 0.0;
    for(int i = 0; i < ui->tableWidget_device->columnCount(); i++)
    {
        QTableWidgetItem* item = ui->tableWidget_device->item(2,i);
        if(item)
        {
            sum += item->data(Qt::DisplayRole).toDouble();
        }

    }

    qDebug()<<"sum is"<<sum<< "len is"<<len;
    if(sum != len)
    {
        QMessageBox::critical(this,tr("错误"),
                              tr("总长度不符!"));
        return false;
    }

    return true;
}

void FormDeviceContent::showEvent(QShowEvent *event)
{
    this->setFocus();
}

void FormDeviceContent::on_pushButton_removeCol_clicked()
{
    int col = ui->tableWidget_device->currentColumn();
    ui->tableWidget_device->removeColumn(col);

    adjustColNo();
}

void FormDeviceContent::on_pushButton_addCol_clicked()
{
    int i = ui->tableWidget_device->currentColumn();
    ui->tableWidget_device->insertColumn(i+1);

    i = i+1;

    QTableWidgetItem * noItem = new QTableWidgetItem;
    noItem->setData(Qt::DisplayRole,i+1);
    noItem->setFlags(Qt::NoItemFlags);
    ui->tableWidget_device->setItem(0,i,noItem);

    QTableWidgetItem * nameItem = new QTableWidgetItem;
    nameItem->setData(Qt::DisplayRole,tr("温区")+QString::number(i+1));
    ui->tableWidget_device->setItem(1,i,nameItem);

    QTableWidgetItem * lengthItem = new QTableWidgetItem;
    lengthItem->setData(Qt::DisplayRole,2.00);
    ui->tableWidget_device->setItem(2,i,lengthItem);

    QTableWidgetItem * upItem = new QTableWidgetItem;
    upItem->setData(Qt::DisplayRole,100.0);
    ui->tableWidget_device->setItem(3,i,upItem);

    QTableWidgetItem * downItem = new QTableWidgetItem;
    downItem->setData(Qt::DisplayRole,100.0);
    ui->tableWidget_device->setItem(4,i,downItem);

    adjustColNo();
}

void FormDeviceContent::on_pushButton_clear_clicked()
{
    while(ui->tableWidget_device->columnCount())
        ui->tableWidget_device->removeColumn(0);
}

void FormDeviceContent::on_pushButton_generate_clicked()
{
    on_pushButton_clear_clicked();

    int num = ui->spinBox_num->value();
    double length = ui->spinBox_length->value();
//    double start = ui->doubleSpinBox_startTemp->value();
//    double end  = ui->doubleSpinBox_endTemp->value();
    double step  = (length) / num;



    ui->tableWidget_device->setColumnCount(num);
    for(int i = 0; i< ui->tableWidget_device->columnCount(); i++)
    {
        QTableWidgetItem * noItem = new QTableWidgetItem;
        noItem->setData(Qt::DisplayRole,i+1);
        noItem->setFlags(Qt::NoItemFlags);
        ui->tableWidget_device->setItem(0,i,noItem);

        QTableWidgetItem * nameItem = new QTableWidgetItem;
        nameItem->setData(Qt::DisplayRole,tr("温区")+QString::number(i+1));
        ui->tableWidget_device->setItem(1,i,nameItem);

        QTableWidgetItem * lengthItem = new QTableWidgetItem;
        lengthItem->setData(Qt::DisplayRole,step);
        ui->tableWidget_device->setItem(2,i,lengthItem);

        QTableWidgetItem * upItem = new QTableWidgetItem;
        upItem->setData(Qt::DisplayRole,100.0);
        ui->tableWidget_device->setItem(3,i,upItem);

        QTableWidgetItem * downItem = new QTableWidgetItem;
        downItem->setData(Qt::DisplayRole,100.0);
        ui->tableWidget_device->setItem(4,i,downItem);
    }
}

void FormDeviceContent::on_tableWidget_device_itemChanged(QTableWidgetItem *item)
{
    if(!item)
        return;

    switch (item->row()) {

    case 3:
    case 4:
    {
        if(ui->radioButton_4->isChecked() && ui->radioButton_2->isChecked())
        {
            int col = item->column();
            QVariant value = item->data(Qt::DisplayRole);
            QTableWidgetItem* toItem = 0;
            if(item->row() == 3)
            {
                toItem = ui->tableWidget_device->item(4,col);
            }
            else if(item->row() == 4)
            {
                toItem = ui->tableWidget_device->item(3,col);
            }

            if(toItem)
                toItem->setData(Qt::DisplayRole,value);
        }

    }
        break;
    default:
        break;
    }





}
