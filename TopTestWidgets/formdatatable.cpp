#include "formdatatable.h"
#include "ui_formdatatable.h"

#include "toptestenum.h"
#include "toptestcore.h"

#include <QResizeEvent>
#include <QSettings>
#include <QTableWidget>
#include <QHeaderView>
#include <QItemEditorFactory>
#include <QDoubleSpinBox>
#include <QDebug>

#include "color_list_item_delegate.h"
#include "spin_item_delegate.h"
#include "port_choose_item_delegate.h"
#include "templateedititemdelegate.h"

#include "toptestsettingmanager.h"

//inline QSettings progressSettingHandle()
//{
//    return QSettings(TopTestCore::instance()->getProgressFileSavePath(),QSettings::IniFormat);
//}

#define varToString(x) #x

FormDataTable::FormDataTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDataTable)
{
    ui->setupUi(this);

    TopTestSettingManager::instance()->loadFromSystemSettings();
    bool ok = false;
    int num = TopTestSettingManager::instance()->getSystemSettingKeyValue("GlobalPortNum:").toInt(&ok);
    if(ok && num >= 1 && num<= 16)
    {
        portNum = num;
    }

//    QItemEditorFactory *factory = new QItemEditorFactory;
////       const_cast<QItemEditorFactory *>( QItemEditorFactory::defaultFactory());
//    QItemEditorCreatorBase *portListCreator =
//        new QStandardItemEditorCreator<MultiComboBox>();
//    factory->registerEditor(QVariant::String, portListCreator);

//    QItemEditorCreatorBase *doubleCreator =
//        new QStandardItemEditorCreator<DoubleSpin>();
//    factory->registerEditor(QVariant::Double, doubleCreator);


//    QItemEditorCreatorBase *boolCreator =
//        new QStandardItemEditorCreator<BoolComboBox>();
//    factory->registerEditor(QVariant::Bool, boolCreator);
//    QItemEditorFactory::setDefaultFactory(factory);


    connect(ui->toolButton_Add,SIGNAL(clicked(bool)),
            this,SLOT(addData()));

    connect(ui->toolButton_Del,SIGNAL(clicked(bool)),
            this,SLOT(removeData()));

    currentTable = 0;

    changeToType(TopTest::MaxTemputure);
}

FormDataTable::~FormDataTable()
{
    delete ui;
}

void FormDataTable::changeToType(int type)
{
    QTableWidget* tableWidget = typeTableMap.value(type);

    if(!tableWidget)
    {
        tableWidget = new QTableWidget(ui->stackedWidget);
        typeTableMap.insert(type,tableWidget);
        ui->stackedWidget->addWidget(tableWidget);
        currentTable = tableWidget;
        initTableWidget(tableWidget,type);
        loadTypeData(type);
        currentTable->setItemDelegateForColumn(
                    currentTable->columnCount()-1,
                    new PortChooseItemDelegate);

        connect(tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),
                this,SLOT(onDataTableWidgetChanged(QTableWidgetItem*)));

    }
    else
        currentTable = tableWidget;

    ui->stackedWidget->setCurrentWidget(tableWidget);

    emit this->changedToType(type);
}

void FormDataTable::loadTypeData(int type)
{
    this->blockSignals(true);

    QSettings s(TopTestCore::instance()->getProgressFileSavePath(),QSettings::IniFormat);
    s.beginGroup(QString::number(type));
    int rowNum = 0;

    QTableWidget* tableWidget = typeTableMap.value(type);
    if(!tableWidget)
        return;

    for(int i=0;i<s.allKeys().count();i++)
    {
        tableWidget->insertRow(i);

        for(int j=0;j<tableWidget->columnCount();j++)
        {
            QTableWidgetItem* emptyItem = new QTableWidgetItem("");
            emptyItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            tableWidget->setItem(i,j,emptyItem);
        }

        QTableWidgetItem* item = tableWidget->item(i,0);
        item->setData(Qt::DisplayRole,i+1);
//        item->setText(QString::number(i+1));
    }

//    for(int i=0;i<tableWidget->rowCount();i++) {
//        QStringList values = s.value(QString::number(i)).toStringList();
//        for(int j=1;j<values.size() && j<tableWidget->columnCount();j++)
//        {
//            QTableWidgetItem* item = tableWidget->item(i,j);
//            item->setText(values.at(j));
//        }
//        rowNum++;
//    }
    s.endGroup();

    this->blockSignals(true);
}

void FormDataTable::saveTypeData()
{
    QSettings s(TopTestCore::instance()->getProgressFileSavePath(),QSettings::IniFormat);

    foreach (int type, typeTableMap.keys())
    {
        s.beginGroup(QString::number(type));

        QTableWidget* tableWidget = typeTableMap.value(type);
        if(tableWidget)
        {
            s.clear();

            for(int i=0;i<tableWidget->rowCount();i++)
            {
        //        QString key = tableWidget->item(i,0)->text();
                QStringList values;
                for(int j=1;j<tableWidget->columnCount();j++)
                {
                    values.push_back(tableWidget->item(i,j)->text());
                }

                s.setValue(QString::number(i),values);
            }
        }

        s.endGroup();
    }



    emit this->dataSaved();
}

void FormDataTable::addData()
{
    this->blockSignals(true);
    int rowNum = currentTable->rowCount();
    currentTable->insertRow(rowNum++);
    QTableWidgetItem* rowNumItem = new QTableWidgetItem;
    rowNumItem->setData(Qt::DisplayRole,rowNum);
    rowNumItem->setFlags(Qt::NoItemFlags);
    rowNumItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    currentTable->setItem(rowNum-1,0,rowNumItem);

    int type = typeTableMap.key(currentTable,-1);

    QVariantList defaultValueList;

    switch (type) {
    case TopTest::IncreaseTime:
//    case TopTest::RemainTime:
    case TopTest::TestValuedTime:
    case TopTest::TemputureBetweenTime:
    {
        defaultValueList<<200.00<<600.00<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::AboveTime:
//    case TopTest::ReturnTime:
    {
        defaultValueList<<300.00<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::DecreaseTime:
    {
        defaultValueList<<600.00<<200.00<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::IncreaseRatio:
    {
        defaultValueList<<200.00<<600.00<<20.0<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::DecreaseRatio:
    {
        defaultValueList<<600.00<<200.00<<20.0<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::MaxIncreaseRatio:
    case TopTest::MaxDecreaseRatio:
    {
        defaultValueList<<20.0<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::AverageRatio:
    {
        defaultValueList<<200.00<<600.00<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::MaxIncreaseTime:
    case TopTest::MaxDecreaseTime:
    {
        defaultValueList<<300.00<<800.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::MaxPortMaxDeltaValue:
    {
        defaultValueList<<0<<100<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::DistanceToMaxTimeSection:
    {
        defaultValueList<<50.00<<30.00<<80.00<<false<<getPortNumString(portNum);
    }
        break;
    case TopTest::MaxTemputure:
    {
        defaultValueList<<600.00<<800.00<<false<<getPortNumString(portNum);
    }
        break;
    default:
        break;
    }


    int column = 1;
    foreach (QVariant value, defaultValueList) {
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole, value);
        currentTable->setItem(rowNum-1,column++,item);
    }

    this->blockSignals(false);

    emit this->tableEdited(typeTableMap.key(currentTable));
}

void FormDataTable::removeData()
{
    currentTable->removeRow(currentTable->currentRow());

    ///refresh data number
    for(int i=0;i<currentTable->rowCount();i++)
    {
        currentTable->item(i,0)->setText(QString::number(i+1));
    }

    emit this->tableEdited(typeTableMap.key(currentTable));
}

QMap<QString, QVariant> FormDataTable::saveTableDataToMap()
{
    QMap<QString,QVariant> tableData;
    foreach (int type, typeTableMap.keys())
    {
        QMap<QString,QVariant> typeDataMap;
        QTableWidget* tableWidget = typeTableMap.value(type);
        if(tableWidget)
        {
            for(int i=0;i<tableWidget->rowCount();i++)
            {
                QMap<QString,QVariant> rowMap;
                for(int j=0;j<tableWidget->columnCount();j++)
                {
                    rowMap.insert(QString::number(j),tableWidget->item(i,j)->data(Qt::DisplayRole));
                }
                typeDataMap.insert(QString::number(i),rowMap);
            }
        }

        tableData.insert(QString::number(type),typeDataMap);
    }

    qDebug()<<"FormDataTable::saveTableDataToMap()"<<tableData<<tableData.count();
    return tableData;
}

void FormDataTable::loadMapToTable(const QMap<QString, QVariant> &data)
{
    qDebug()<<"FormDataTable::loadMapToTable"<<data<<data.count();
    this->blockSignals(true);
    foreach (QString noStr, data.keys()) {
        qDebug()<<"begin type"<<noStr;
        bool ok;
        int type = noStr.toInt(&ok);

        this->changeToType(type);

        QTableWidget* tableWidget = typeTableMap.value(type);
        if(!ok || !tableWidget)
        {
            qDebug()<<"skip type"<<type<<ok<<tableWidget;
            continue;
        }

        QMap<QString,QVariant> typeDataMap = data.value(noStr).toMap();
        qDebug()<<"FormDataTable::typeDataMap"<<typeDataMap;
        foreach(QString rowStr, typeDataMap.keys())
        {
            bool ok;
            int i = rowStr.toInt(&ok);
            if(!ok)
                continue;
            tableWidget->insertRow(i);
            for(int j=0;j<tableWidget->columnCount();j++)
            {
                QTableWidgetItem* emptyItem = new QTableWidgetItem;
//                emptyItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                tableWidget->setItem(i,j,emptyItem);
            }

            QTableWidgetItem* rowNumItem = tableWidget->item(i,0);
            if(rowNumItem)
            {
                rowNumItem->setData(Qt::DisplayRole,i+1);
                rowNumItem->setFlags(Qt::NoItemFlags);
                rowNumItem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }

        for(int i=0;i<tableWidget->rowCount();i++) {
            QMap<QString,QVariant> variantMap = typeDataMap.value(QString::number(i)).toMap();
            qDebug()<<"row values is "<<variantMap;
            foreach (QString columnStr, variantMap.keys()) {
               int column = columnStr.toInt();
               QTableWidgetItem* item = tableWidget->item(i,column);
               if(item)
                    item->setData(Qt::DisplayRole,variantMap.value(columnStr));
            }
        }

    }

    this->changeToType(TopTest::IncreaseTime);

    this->blockSignals(false);
}

void FormDataTable::clear()
{
    foreach (QTableWidget* table, typeTableMap.values()) {
        while(table->rowCount())
            table->removeRow(0);
    }
}

QString FormDataTable::getPortNumString(int portNum)
{
    QString ret;
    for(int i = 1; i <= portNum; i++)
    {
        ret += QString("%1;").arg(i);
    }

    ret.chop(1);

    return ret;
}


void FormDataTable::onDataTableWidgetChanged(QTableWidgetItem *item)
{
    QTableWidget* tableWidget = item->tableWidget();

    //check edit value first


    if(tableWidget && typeTableMap.values().contains(tableWidget))
        emit this->tableEdited(typeTableMap.key(tableWidget));
}

void FormDataTable::initTableWidget(QTableWidget *tableWidget, int type)
{


    tableWidget->reset();

    tableWidget->setGeometry(QRect(QPoint(0,0),this->size()));
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
//    QItemEditorFactory::setDefaultFactory(factory);

    while(tableWidget->rowCount())
        tableWidget->removeRow(0);

    while(tableWidget->columnCount())
        tableWidget->removeColumn(0);

    switch (type) {
    case TopTest::IncreaseTime:
//    case TopTest::RemainTime:
    case TopTest::TestValuedTime:
    case TopTest::TemputureBetweenTime:
    {
        tableWidget->setColumnCount(8);
        tableWidget->setColumnWidth(1,40);
    }
        break;
    case TopTest::AboveTime:
//    case TopTest::ReturnTime:
    {
        tableWidget->setColumnCount(7);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,200);
    }
        break;
    case TopTest::DecreaseTime:
    {
        tableWidget->setColumnCount(8);
        tableWidget->setColumnWidth(1,40);
    }
        break;
    case TopTest::IncreaseRatio:
    case TopTest::DecreaseRatio:
    {
        tableWidget->setColumnCount(9);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,80);
        tableWidget->setColumnWidth(3,80);
        tableWidget->setColumnWidth(4,80);
        tableWidget->setColumnWidth(5,80);
        tableWidget->setColumnWidth(6,80);
    }
        break;

    case TopTest::MaxIncreaseRatio:
    case TopTest::MaxDecreaseRatio:
    {
        tableWidget->setColumnCount(7);
//        tableWidget->setColumnWidth(0,60);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,200);
//        tableWidget->setColumnWidth(3,200);
    }
        break;
    case TopTest::AverageRatio:
    {
        tableWidget->setColumnCount(8);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,80);
        tableWidget->setColumnWidth(3,80);
        tableWidget->setColumnWidth(4,80);
        tableWidget->setColumnWidth(5,80);
//        tableWidget->setColumnWidth(6,80);
    }
        break;
//    case TopTest::MaxIncreaseTime:
    case TopTest::DistanceToMaxTimeSection:
    {
        tableWidget->setColumnCount(7);
//        tableWidget->setColumnWidth(0,60);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,200);
    }
        break;
    case TopTest::MaxIncreaseTime:
    case TopTest::MaxDecreaseTime:
    case TopTest::MaxTemputure:
    case TopTest::MaxPortMaxDeltaValue:
    {
        tableWidget->setColumnCount(6);
//        tableWidget->setColumnWidth(0,60);
        tableWidget->setColumnWidth(1,40);
        tableWidget->setColumnWidth(2,200);
        tableWidget->setColumnWidth(3,200);
   }
        break;
//    case TopTest::PinNameAndColor:
//    {
//        tableWidget->setColumnCount(6);
////        tableWidget->setColumnWidth(0,60);
//        tableWidget->setColumnWidth(1,40);
//        tableWidget->setColumnWidth(2,200);
//        tableWidget->setColumnWidth(3,200);

//        Fips::ColorListItemDelegate * delegate = new Fips::ColorListItemDelegate(tableWidget);
//        tableWidget->setItemDelegateForColumn(3,delegate);
//    }
//        break;
    default:
        break;
    }
//    tableWidget->setItemDelegateForColumn(
//                tableWidget->columnCount()-1,
//                new PortChooseItemDelegate);
    tableWidget->setColumnWidth(tableWidget->columnCount()-2,40);

    tableWidget->removeColumn(0);
}

void FormDataTable::resizeEvent(QResizeEvent *event)
{
    ui->stackedWidget->setGeometry(QRect(QPoint(0,0),this->size()));

    ui->gridWidget->setGeometry(QRect(QPoint(this->size().width()* 0.75,
                                             this->size().height()* 0.60),
                                QSize(this->size().width()*0.25,
                                      this->size().height()* 0.40)));
}
