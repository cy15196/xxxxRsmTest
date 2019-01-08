#pragma execution_character_set("utf-8")

#include "formdevicetable.h"

#include <QHeaderView>

FormDeviceTable::FormDeviceTable(QWidget *parent)
    :QTableWidget(parent)
{
    this->setRowCount(5);
    this->setVerticalHeaderLabels(
                    {
                    tr("序号"),
                    tr("温区名称"),
                    tr("温区长度"),
                    tr("上温区"),
                    tr("下温区")
                    });
    this->horizontalHeader()->setVisible(false);
}

FormDeviceTable::~FormDeviceTable()
{

}

QMap<QString, QVariant> FormDeviceTable::saveTableToMap()
{
    QMap<QString, QVariant> ret;

    for(int j=0; j<this->columnCount(); j++)
    {
        QMap<QString,QVariant> columnMap;
        for(int i=0; i<this->rowCount();i++)
        {
            QTableWidgetItem* item =  this->item(i,j);
            if(item)
                columnMap.insert(QString::number(i),item->data(Qt::DisplayRole));
        }
        ret.insert(QString::number(j),columnMap);
    }

    return ret;
}

void FormDeviceTable::loadDataToTable(const QMap<QString, QVariant> &data, bool singleSection)
{
    this->setColumnCount(data.keys().count());
    foreach (QString colStr, data.keys()) {
        int col = colStr.toInt();
        QMap<QString,QVariant> columnMap = data.value(colStr).toMap();
        foreach (QString rowStr, columnMap.keys()) {
            int row = rowStr.toInt();
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setData(Qt::DisplayRole,columnMap.value(rowStr));
            item->setFlags(Qt::ItemIsSelectable);
            this->setItem(row,col,item);
        }
    }

    if(singleSection)
    {
        this->setVerticalHeaderItem(
                    3,new QTableWidgetItem( tr("温区温度") ));
        this->setRowHeight(4,0);
    }
    else
    {
        this->setVerticalHeaderItem(
                    3, new QTableWidgetItem(tr("上温区")) );
        this->setRowHeight(4,30);
    }

}

int FormDeviceTable::getAreaCount() const
{
    return areaCount;
}

void FormDeviceTable::setAreaCount(int value)
{
    this->setAreaCount(value);
    areaCount = value;
}
