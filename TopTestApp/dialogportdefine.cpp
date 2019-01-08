#include "dialogportdefine.h"
#include "ui_dialogportdefine.h"

#include "color_item_delegate.h"
#include "clabso_ft_dpi.h"

#include <QDebug>
#include <QPushButton>

DialogPortDefine::DialogPortDefine(QWidget *parent) :
    DialogBase(parent),
    ui(new Ui::DialogPortDefine)
{
    ui->setupUi(this);
    ui->tableWidget->setItemDelegateForRow(1,new Fips::ColorItemDelegate);
    ui->tableWidget_2->setItemDelegateForRow(1,new Fips::ColorItemDelegate);

    ui->tableWidget->setFixedSize(ClabsoFTDpi::convertSize(QSize(542,88)));
    ui->tableWidget_2->setFixedSize(ClabsoFTDpi::convertSize(QSize(542,88)));
}

DialogPortDefine::~DialogPortDefine()
{
    delete ui;
}

void DialogPortDefine::setCurrentPortInfo(const QMap<int, QPair<QString, QColor> > &portInfo)
{

    qDebug()<<"setCurrentPortInfo "<< portInfo;

    if(portInfo.count() <=8 )
    {
        ui->tableWidget_2->setColumnCount(0);
        ui->tableWidget_2->setVisible(false);
        ui->tableWidget->setColumnCount(1 + portInfo.count());
    }
    else
    {
        ui->tableWidget_2->setColumnCount(1 + portInfo.count() -8);
    }



    QTableWidget* tableWidget;
    foreach (int portNo, portInfo.keys()) {

        if(portNo <9)
            tableWidget = ui->tableWidget;
        else
            tableWidget = ui->tableWidget_2;

        QPair<QString,QColor> info = portInfo.value(portNo);
        QTableWidgetItem * nameItem = new QTableWidgetItem;
        nameItem->setData(Qt::DisplayRole,info.first);
        tableWidget->setItem(0,(portNo) % 9 + portNo/9 ,nameItem);

        QTableWidgetItem * colorItem = new QTableWidgetItem;
        colorItem->setData(Qt::DisplayRole,info.second.name());
        tableWidget->setItem(1,(portNo) % 9 + portNo/9,colorItem);
    }

    oldPortInfo = portInfo;
}

QMap<int, QPair<QString, QColor> > DialogPortDefine::getCurrentPortInfo()
{
    QMap<int, QPair<QString, QColor> > ret;
    for(int i= 1; i< ui->tableWidget->columnCount(); i++)
    {
        QPair<QString, QColor> info;
        info.first = ui->tableWidget->item(0,i)->data(Qt::DisplayRole).toString();
        info.second = QColor(ui->tableWidget->item(1,i)->data(Qt::DisplayRole).toString());
        ret.insert(i,info);
    }

    for(int i=1; i< ui->tableWidget_2->columnCount(); i++)
    {
        QPair<QString, QColor> info;
        info.first = ui->tableWidget_2->item(0,i)->data(Qt::DisplayRole).toString();
        info.second = QColor(ui->tableWidget_2->item(1,i)->data(Qt::DisplayRole).toString());
        ret.insert(i+8,info);
    }

    return ret;
}

void DialogPortDefine::onButtonClicked(int role)
{
    switch(role)
    {
    case QDialogButtonBox::Ok:
        emit this->portInfoChanged(getCurrentPortInfo());
        break;
    default:
        break;
    }
}
