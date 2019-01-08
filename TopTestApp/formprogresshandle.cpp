#pragma execution_character_set("utf-8")

#include "formprogresshandle.h"
#include "ui_formprogresshandle.h"

#include <QFileDialog>
#include <QCheckBox>
#include <QTableWidget>
#include <QDebug>
#include <QVariant>
#include <QStackedLayout>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBitArray>
#include <QBuffer>
#include <QFileDialog>
#include <QLineEdit>

#include "formdraglabel.h"
#include "formportdefinepic.h"
#include "rsmcoresignalenum.h"
#include "formdatatable.h"
#include "formdevicetable.h"

#include "toptestenum.h"
#include "clabso_ft_dpi.h"

inline QCheckBox* addCheckBoxToTable(QTableWidget* table, int row, int column)
{

    QWidget* widget = new QWidget();
    widget->setLayout(new QHBoxLayout);
    widget->layout()->setMargin(0);

    QCheckBox* checkBox = new QCheckBox(widget);
    widget->layout()->addWidget(checkBox);
    widget->layout()->setAlignment(checkBox,Qt::AlignCenter);

    table->setCellWidget(row, column, widget);

    return checkBox;
}


FormProgressHandle::FormProgressHandle(QWidget *parent) :
    FileSettingWidget(parent),
    ui(new Ui::DialogProgressHandle)
{
    ui->setupUi(this);

    init();

//    qDebug()<<this->findChildren<QWidget*>();
//    initPortChoose();
}

FormProgressHandle::~FormProgressHandle()
{
    delete ui;
}

void FormProgressHandle::on_changedToType(int type)
{

}

void FormProgressHandle::onTableDataEdited(int type)
{
    QListWidgetItem* item =
        current_ProgressType->item(type);
    if(!item->text().endsWith("*"))
        item->setText(item->text()+"*");
}

void FormProgressHandle::onDataSaved()
{
    for(int i=0;i<current_ProgressType->count();i++)
    {
        QListWidgetItem* item =
            current_ProgressType->item(i);
        if(item->text().endsWith("*"))
            item->setText(item->text().left(item->text().length()-1));
    }
}

//void FormProgressHandle::initPortChoose()
//{
//    ui->tableWidget_PortChoose->setSpan(0,0,2,1);
////    ui->tableWidget_PortChoose->setSpan(0,1,2,1);
//    ui->tableWidget_PortChoose->setColumnWidth(0,100);
//    ui->tableWidget_PortChoose->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_PortChoose->verticalHeader()->setStretchLastSection(true);

//    for(int i=1;i<17;i++)
//    {
//        ui->tableWidget_PortChoose->setColumnWidth(i,32);
//    }

//    for(int i=1;i<18;i++)
//    {
//        addCheckBoxToTable(ui->tableWidget_PortChoose,1,i);
//    }

//    for(int i=0;i<ui->tableWidget_PortChoose->rowCount();i++)
//        for(int j=0;j<ui->tableWidget_PortChoose->columnCount();j++)
//        {
//            QTableWidgetItem* item = ui->tableWidget_PortChoose->item(i,j);
//            if(item)
//                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
//        }
//}


void FormProgressHandle::init()
{
    ui->comboBox_TimeUnit->addItem(tr("秒"),TopTest::s);
//    ui->comboBox_TimeUnit->addItem(tr("分钟"),TopTest::min);
    ui->comboBox_TimeUnit->setEnabled(false);


    ui->listWidget_ProgressType_1->addItems(TopTest::getProgressTypeTranslateList());
    ui->listWidget_ProgressType_2->addItems(TopTest::getProgressTypeTranslateList());
    ui->listWidget_ProgressType_3->addItems(TopTest::getProgressTypeTranslateList());

//    foreach (QString typeStr, TopTest::getProgressTypeTranslateList()) {
//        ui->listWidget_ProgressType_1->addItem(typeStr);
//        ui->listWidget_ProgressType_2->addItem(typeStr);
//        ui->listWidget_ProgressType_3->addItem(typeStr);
//    }

    ui->listWidget_ProgressType_1->setMaximumWidth(ClabsoFTDpi::convertSize(120));
    ui->listWidget_ProgressType_2->setMaximumWidth(ClabsoFTDpi::convertSize(120));
    ui->listWidget_ProgressType_3->setMaximumWidth(ClabsoFTDpi::convertSize(120));

    connect(ui->listWidget_ProgressType_1,SIGNAL(currentRowChanged(int)),
            ui->widget_HeaderTable_1,SLOT(changeToType(int)));
    connect(ui->listWidget_ProgressType_1,SIGNAL(currentRowChanged(int)),
            ui->widget_DataTable_1,SLOT(changeToType(int)));

    connect(ui->listWidget_ProgressType_2,SIGNAL(currentRowChanged(int)),
            ui->widget_HeaderTable_2,SLOT(changeToType(int)));
    connect(ui->listWidget_ProgressType_2,SIGNAL(currentRowChanged(int)),
            ui->widget_DataTable_2,SLOT(changeToType(int)));

    connect(ui->listWidget_ProgressType_3,SIGNAL(currentRowChanged(int)),
            ui->widget_HeaderTable_3,SLOT(changeToType(int)));
    connect(ui->listWidget_ProgressType_3,SIGNAL(currentRowChanged(int)),
            ui->widget_DataTable_3,SLOT(changeToType(int)));

    ui->listWidget_ProgressType_1->setCurrentRow(1);
    ui->listWidget_ProgressType_2->setCurrentRow(1);
    ui->listWidget_ProgressType_3->setCurrentRow(1);

    ui->listWidget_ProgressType_1->setCurrentRow(0);
    ui->listWidget_ProgressType_2->setCurrentRow(0);
    ui->listWidget_ProgressType_3->setCurrentRow(0);

//    ui->listWidget_ProgressType_2->item(1)->setSelected(true);
//    ui->listWidget_ProgressType_3->item(1)->setSelected(true);

//    ui->listWidget_ProgressType_1->item(0)->setSelected(true);
//    ui->listWidget_ProgressType_2->item(0)->setSelected(true);
//    ui->listWidget_ProgressType_3->item(0)->setSelected(true);

    ui->tableWidget_wave->setColumnWidth(0,ClabsoFTDpi::convertSize(80));
    ui->tableWidget_wave->setColumnWidth(1,ClabsoFTDpi::convertSize(80));
    ui->tableWidget_wave->setColumnWidth(2,ClabsoFTDpi::convertSize(80));
    ui->tableWidget_wave->verticalHeader()->setVisible(false);
    ui->tableWidget_wave->insertRow(0);
    ui->tableWidget_wave->insertRow(1);
    ui->tableWidget_wave->insertRow(2);
    ui->tableWidget_wave->setSelectionMode(QTableWidget::SingleSelection);
    ui->tableWidget_wave->setSelectionBehavior(QTableWidget::SelectRows);
    for(int i =0; i<3;i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::DisplayRole,tr("波段") + QString::number(i+1));
        item->setFlags(item->flags() & 0xFFFD);
        ui->tableWidget_wave->setItem(i,0,item);

        QTableWidgetItem* item1 = new QTableWidgetItem;
        item1->setData(Qt::DisplayRole,50.0);
        item1->setFlags(item1->flags() & 0xFFFD);
        ui->tableWidget_wave->setItem(i,1,item1);

        QTableWidgetItem* item2 = new QTableWidgetItem;
        item2->setData(Qt::DisplayRole,50.0);
        item2->setFlags(item2->flags() & 0xFFFD);
        ui->tableWidget_wave->setItem(i,2,item2);

        QTableWidgetItem* item3 = new QTableWidgetItem;
        item3->setData(Qt::DisplayRole,tr("波段") + QString::number(i+1));
        item3->setFlags(item2->flags() & 0xFFFD);
        ui->tableWidget_wave->setItem(i,3,item3);
    }

    ui->tableWidget_wave->item(0,0)->setFlags(ui->tableWidget_wave->item(0,0)->flags() & 0xFFFD);

    ui->tableWidget_wave->selectRow(0);
//    ui->tableWidget_wave->setEnabled(false);





    connect(ui->widget_HeaderTable_1,SIGNAL(changedToType(int)),
            this,SLOT(on_changedToType(int)));
    connect(ui->widget_HeaderTable_2,SIGNAL(changedToType(int)),
            this,SLOT(on_changedToType(int)));
    connect(ui->widget_HeaderTable_3,SIGNAL(changedToType(int)),
            this,SLOT(on_changedToType(int)));

    connect(ui->widget_DataTable_1,SIGNAL(tableEdited(int)),
            this,SLOT(onTableDataEdited(int)));
    connect(ui->widget_DataTable_1,SIGNAL(dataSaved()),
            this,SLOT(onDataSaved()));
    connect(ui->widget_DataTable_1,SIGNAL(tableEdited(int)),
            this,SIGNAL(contentChanged()));

    connect(ui->widget_DataTable_2,SIGNAL(tableEdited(int)),
            this,SLOT(onTableDataEdited(int)));
    connect(ui->widget_DataTable_2,SIGNAL(dataSaved()),
            this,SLOT(onDataSaved()));
    connect(ui->widget_DataTable_2,SIGNAL(tableEdited(int)),
            this,SIGNAL(contentChanged()));

    connect(ui->widget_DataTable_3,SIGNAL(tableEdited(int)),
            this,SLOT(onTableDataEdited(int)));
    connect(ui->widget_DataTable_3,SIGNAL(dataSaved()),
            this,SLOT(onDataSaved()));
    connect(ui->widget_DataTable_3,SIGNAL(tableEdited(int)),
            this,SIGNAL(contentChanged()));
//    connect(ui->toolButton_Save,SIGNAL(clicked(bool)),
//            ui->widget_DataTable,SLOT(saveTypeData()));

    foreach (QLineEdit* edit, this->findChildren<QLineEdit*>()) {
        connect(edit,SIGNAL(textChanged(QString)),
                this,SIGNAL(contentChanged()));
    }

    foreach (QComboBox* combo, this->findChildren<QComboBox*>()) {
        connect(combo,SIGNAL(currentTextChanged(QString)),
                this,SIGNAL(contentChanged()));
    }

//    connect(ui->widget_pic,SIGNAL(contentChanged()),
//            this,SIGNAL(contentChanged()));





//    ui->tableWidget_wave->setMinimumHeight(ClabsoFTDpi::convertSize(110));
    ui->tableWidget_wave->setFixedHeight(ClabsoFTDpi::convertSize(110));
    ui->tableWidget_wave->setFixedWidth(ClabsoFTDpi::convertSize(360));
}

QString FormProgressHandle::getFileTypeDisplayText()
{
    return tr("工艺文件设置");
}

QString FormProgressHandle::getFileTypeSettingKeyName()
{
    return "Progress";
}

void FormProgressHandle::loadSettigRecord(const QVariant &recordName)
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
    ///load wave table
    QMap<QString,QVariant > waveDataMap = recordInfoMap[ui->tableWidget_wave->objectName()].toMap();
    qDebug()<<"load wave data map"<<waveDataMap;
    for(int i = 0; i < ui->tableWidget_wave->rowCount(); i++)
    {
        QMap<QString,QVariant > rowMap = waveDataMap[QString::number(i)].toMap();
        if(rowMap.isEmpty())
            continue;

        for(int j = 0; j < ui->tableWidget_wave->columnCount(); j++)
        {
            QTableWidgetItem* item = ui->tableWidget_wave->item(i,j);
            if(!item)
                continue;
            if( j == 0)
                item->setCheckState((Qt::CheckState)rowMap[QString::number(j)].toInt());
            else
                item->setData(Qt::DisplayRole,rowMap[QString::number(j)]);
        }
    }
    loadWidgetContentMap(recordInfoMap);
//    ui->label_pic->setPixmap(recordInfoMap["Pic"].toString());
}

void FormProgressHandle::saveCurrentSettingRecord()
{
    QMap<QString,QVariant> recordInfoMap = saveWidgetContentToMap();
    emit ui->widget_DataTable_1->dataSaved();
    //save content
    QString recordName = ui->lineEdit_name->text();
    recordInfoMap.insert("Name",recordName);
//    recordInfoMap.insert("Pic",ui->label_pic->pixmap());
    qDebug()<<getFileTypeSettingKeyName()<<recordName<<recordInfoMap;
    //save record
    writeDataToSettingFile(recordName,recordInfoMap);
}
void FormProgressHandle::setCurrentRecordName(const QString &label)
{
    ui->lineEdit_name->setText(label);
}

QString FormProgressHandle::getCurrentRecordName()
{
    return ui->lineEdit_name->text();
}

void FormProgressHandle::clearContent()
{
    foreach (QLineEdit* edit, this->findChildren<QLineEdit*>()) {
        edit->clear();
    }

    ui->widget_DataTable_1->clear();
    ui->widget_DataTable_2->clear();
    ui->widget_DataTable_3->clear();

    for(int i = 0; i < ui->tableWidget_wave->rowCount(); i++)
    {
        for(int j = 0; j < ui->tableWidget_wave->columnCount(); j++)
        {
            QTableWidgetItem* item = ui->tableWidget_wave->item(i,j);
            if(!item)
                continue;
            if( j == 0)
            {
                if(i == 0)
                    item->setCheckState(Qt::Checked);
                else
                    item->setCheckState(Qt::Unchecked);
            }
            else if(j == 3)
                item->setData(Qt::DisplayRole,tr("波段") + QString::number(i+1));
            else {
                item->setData(Qt::DisplayRole,-200.0);
            }

        }
    }

    //    ui->widget_pic->clearContent();
}

QMap<QString, QVariant> FormProgressHandle::saveWidgetContentToMap()
{
    QMap<QString, QVariant> ret = FileSettingWidget::saveWidgetContentToMap();

    ///save wave table state to this map
    QMap<QString,QVariant > waveDataMap;
    for(int i = 0; i < ui->tableWidget_wave->rowCount(); i++)
    {
        QMap<QString,QVariant > rowMap;
        for(int j = 0; j < ui->tableWidget_wave->columnCount(); j++)
        {
            QTableWidgetItem* item = ui->tableWidget_wave->item(i,j);
            if(!item)
                continue;
            if( j == 0)
                rowMap.insert(QString::number(j),item->checkState());
            else
                rowMap.insert(QString::number(j),item->data(Qt::DisplayRole));

        }
        waveDataMap.insert(QString::number(i),rowMap);
    }

    qDebug()<<"save wave data map to system"<<waveDataMap;

    ret.insert(ui->tableWidget_wave->objectName(),waveDataMap);


    return ret;
}

void FormProgressHandle::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->listWidget_ProgressType_1->blockSignals(true);
        ui->listWidget_ProgressType_1->clear();
        ui->listWidget_ProgressType_1->addItems(TopTest::getProgressTypeTranslateList());
        ui->listWidget_ProgressType_1->blockSignals(false);

        ui->listWidget_ProgressType_2->blockSignals(true);
        ui->listWidget_ProgressType_2->clear();
        ui->listWidget_ProgressType_2->addItems(TopTest::getProgressTypeTranslateList());
        ui->listWidget_ProgressType_2->blockSignals(false);

        ui->listWidget_ProgressType_3->blockSignals(true);
        ui->listWidget_ProgressType_3->clear();
        ui->listWidget_ProgressType_3->addItems(TopTest::getProgressTypeTranslateList());
        ui->listWidget_ProgressType_3->blockSignals(false);
    }

    QWidget::changeEvent(event);
}

void FormProgressHandle::on_tableWidget_wave_itemChanged(QTableWidgetItem *item)
{

    if(!item)
        return;

    ui->tableWidget_wave->blockSignals(true);

    if(item->column() == 0)
    {
        qDebug()<<"go to change row editable"<<item->row();
        if(item->checkState() == Qt::Checked)
        {
            setRowTemperatureEditable(item->row(),true);
            switch (item->row()) {
            case 0:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(1,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable |
                                  Qt::ItemIsEnabled);
            }
                break;
            case 1:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(2,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable |
                                  Qt::ItemIsEnabled);

                rowitem = ui->tableWidget_wave->item(0,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::NoItemFlags);
            }
                break;
            case 2:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(1,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::NoItemFlags);
            }
                break;
            default:
                break;
            }
        }
        else if(item->checkState() == Qt::Unchecked)
        {
            setRowTemperatureEditable(item->row(),false);
            switch (item->row()) {
            case 0:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(1,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::NoItemFlags);
            }
                break;
            case 1:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(2,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::NoItemFlags);

                rowitem = ui->tableWidget_wave->item(0,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable |
                                  Qt::ItemIsEnabled);
            }
                break;
            case 2:
            {
                QTableWidgetItem* rowitem = ui->tableWidget_wave->item(1,0);
                if(!rowitem)
                    break;
                rowitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable |
                                  Qt::ItemIsEnabled);
            }
                break;
            default:
                break;
            }
        }
    }

    ui->tableWidget_wave->blockSignals(false);

    emit this->contentChanged();
}

void FormProgressHandle::setRowTemperatureEditable(int rowNo, bool editable)
{
    for(int j = 1; j< ui->tableWidget_wave->columnCount(); j++)
    {
        QTableWidgetItem* item = ui->tableWidget_wave->item(rowNo,j);
        if(item)
        {
            if(editable)
            {
                item->setFlags(item->flags() | Qt::ItemIsEditable);
            }
            else
                item->setFlags(item->flags() & 0xFFFD);
        }
    }
}

void FormProgressHandle::on_tableWidget_wave_itemSelectionChanged()
{
    QList<QTableWidgetItem*> itemList = ui->tableWidget_wave->selectedItems();
    if(itemList.isEmpty())
        return;

    changeToRowWave(itemList.first()->row());
}

void FormProgressHandle::changeToRowWave(int rowNo)
{
    ui->stackedWidget->setCurrentIndex(rowNo);
    switch (rowNo) {
    case 0:
        current_HeaderTable = ui->widget_HeaderTable_1 ;
        current_DataTable = ui->widget_DataTable_1;
        current_ProgressType = ui->listWidget_ProgressType_1;
        break;
    case 1:
        current_HeaderTable = ui->widget_HeaderTable_2 ;
        current_DataTable = ui->widget_DataTable_2;
        current_ProgressType = ui->listWidget_ProgressType_2;
        break;
    case 2:
        current_HeaderTable = ui->widget_HeaderTable_3 ;
        current_DataTable = ui->widget_DataTable_3;
        current_ProgressType = ui->listWidget_ProgressType_3;
        break;
    default:
        current_HeaderTable = ui->widget_HeaderTable_1 ;
        current_DataTable = ui->widget_DataTable_1;
        current_ProgressType = ui->listWidget_ProgressType_1;
        ui->stackedWidget->setCurrentIndex(0);
        break;
    }
}
