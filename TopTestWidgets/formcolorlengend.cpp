#pragma execution_character_set("utf-8")

#include "formcolorlengend.h"
#include "ui_formcolorlengend.h"
#include "line_color_item_delegate.h"
//#include "checktoolbutton_item_delegate.h"

#include <QTableWidgetItem>
#include <QToolButton>
#include <QLineSeries>
#include <QDebug>

#include "clabso_ft_dpi.h"
#include "qcustomplot.h"

FormColorLengend::FormColorLengend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormColorLengend)
{
    ui->setupUi(this);
    ui->tableWidget->clear();
    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);
    lineNum = 0;
    connect(this,SIGNAL(lineNumChanged(int)),
            this,SLOT(onLineNumChanged(int)));

    this->setLineNum(1);
    initTable();
}

FormColorLengend::~FormColorLengend()
{
    delete ui;
}

void FormColorLengend::setPortNum(const int &portNum)
{
//    ui->tableWidget->setRowCount(portNum);
    resetPortToNum(portNum);
}

void FormColorLengend::setPortName(const int &portNo, const QString &name)
{
    QTableWidgetItem* name00 = new QTableWidgetItem;
    name00->setText(name);
    ui->tableWidget->setItem(portNo,1,name00);
}

void FormColorLengend::setPortColor(const int &portNo, const QString &colorName)
{
    QTableWidgetItem* color01 = new QTableWidgetItem;
    color01->setData(Qt::DisplayRole,colorName);
    ui->tableWidget->setItem(portNo,0,color01);
}

QMap<int, QPair<QString, QColor> > FormColorLengend::getPortInfoMap()
{
    QMap<int, QPair<QString, QColor> > ret;

    for(int i =0; i < ui->tableWidget->rowCount() ; i++)
    {
        QPair<QString,QColor> info;
        info.first = ui->tableWidget->item(i,1)->data(Qt::DisplayRole).toString();
        info.second = QColor(ui->tableWidget->item(i,0)->data(Qt::DisplayRole).toString());
        ret.insert(i+1,info);
    }

    return ret;
}

void FormColorLengend::setPortInfos(const QMap<int, QPair<QString, QColor> > &portInfoMap)
{

}

void FormColorLengend::setPortVisualObject(const int &portNo, QCPGraph *line)
{
    QToolButton* button = qobject_cast<QToolButton*>(ui->tableWidget->cellWidget(portNo,2));
    if(!button)
        return;
    buttonToLine[button] = line;
//    connect(button,SIGNAL(toggled(bool)),
//            line,SLOT)
    //    ui->tableWidget->setItem(i,2,visible02);
}

void FormColorLengend::resetPortToNum(const int& portNum, bool reset)
{
    qDebug()<<"FormColorLengend::got to resetPortToNum"<<portNum<<reset;
    if(reset)
    {
        while(ui->tableWidget->rowCount())
            ui->tableWidget->removeRow(0);
        for(int i = 0 ; i < portNum ; i++)
        {
            ui->tableWidget->insertRow(i);

            QTableWidgetItem* name00 = new QTableWidgetItem;
            name00->setText(tr("通道")+QString::number(i+1));
            ui->tableWidget->setItem(i,1,name00);

            QTableWidgetItem* color01 = new QTableWidgetItem;
            color01->setData(Qt::DisplayRole,QColor(static_cast<Qt::GlobalColor>(i+4)).name());
            ui->tableWidget->setItem(i,0,color01);

            QToolButton* button = new QToolButton(ui->tableWidget);
            button->setCheckable(true);
            button->setChecked(true);
            button->setIcon(QIcon(":/images/ic_yanjingzhankai.png"));
            ui->tableWidget->setCellWidget(i,2,button);
            connect(button,SIGNAL(toggled(bool)),
                    SLOT(onButtonCheckStateChanged(bool)));
        }
    }
    else
    {
        int i = ui->tableWidget->rowCount();
        if(portNum == i)
            return;

        if(portNum > ui->tableWidget->rowCount()) // 需要增加行
        {
            for(; i < portNum ; i++)
            {
                ui->tableWidget->insertRow(i);

                QTableWidgetItem* name00 = new QTableWidgetItem;
                name00->setText(tr("通道")+QString::number(i+1));
                ui->tableWidget->setItem(i,1,name00);

                QTableWidgetItem* color01 = new QTableWidgetItem;
                color01->setData(Qt::DisplayRole,QColor(static_cast<Qt::GlobalColor>(i+4)).name());
                ui->tableWidget->setItem(i,0,color01);

                QToolButton* button = new QToolButton(ui->tableWidget);
                button->setCheckable(true);
                button->setChecked(true);
                button->setIcon(QIcon(":/images/ic_yanjingzhankai.png"));
                ui->tableWidget->setCellWidget(i,2,button);
                connect(button,SIGNAL(toggled(bool)),
                        SLOT(onButtonCheckStateChanged(bool)));
            }
        }

        else{   //需要删除行
            for(; i > portNum ; i-- )
            {
                ui->tableWidget->removeRow( i -1 );
            }
        }

    }
}

void FormColorLengend::initTable()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(0,32);
    ui->tableWidget->setColumnWidth(2,25);

    ///first column color name and show/hide
    ui->tableWidget->setItemDelegateForColumn(0,new  Fips::LineColorItemDelegate(ui->tableWidget));
//    ToolButtonItemDelegate* visibleDelegate =  new ToolButtonItemDelegate(ui->tableWidget);
//    visibleDelegate->setCheckable(true);
//    visibleDelegate->setIconPathChecked(":/images/ic_yanjingzhankai.png");
//    visibleDelegate->setIconPathUnchecked(":/images/ic_yanjingguanbi.png");
    //    ui->tableWidget->setItemDelegateForColumn(2,visibleDelegate);
}

int FormColorLengend::getLineNum() const
{
    return lineNum;
}

void FormColorLengend::setLineNum(int value)
{
    lineNum = value;
    emit this->lineNumChanged(value);
}

void FormColorLengend::clearData()
{
    lineNum = 1;
    buttonToLine.clear();
    while(ui->tableWidget->rowCount())
    {
        ui->tableWidget->removeRow(0);
    }
}

void FormColorLengend::onButtonCheckStateChanged(bool checked)
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if(!button)
        return;

    if(checked)
        button->setIcon(QIcon(":/images/ic_yanjingzhankai.png"));
    else
        button->setIcon(QIcon(":/images/ic_yanjingguanbi.png"));

    QCPGraph* line = buttonToLine[button];
    if(line)
    {
        line->setVisible(checked);
        emit this->lineVisibleChanged(line,checked);
        emit this->needReplot();
    }

}

void FormColorLengend::on_tableWidget_currentItemChanged(
        QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if(current)
    {
        QToolButton* button = qobject_cast<QToolButton*>(ui->tableWidget->cellWidget(current->row(),2));
        if(button)
        {
            QCPGraph* line = buttonToLine[button];
            if(line)
            {
                QPen pen = line->pen();
                pen.setWidthF(1.5);
                line->setPen(pen);
            }
        }
    }

    if(previous)
    {
        QToolButton* button = qobject_cast<QToolButton*>(ui->tableWidget->cellWidget(previous->row(),2));
        if(button)
        {
            QCPGraph* line = buttonToLine[button];
            if(line)
            {
                QPen pen = line->pen();
                pen.setWidthF(1);
                line->setPen(pen);
            }
        }
    }

    emit this->needReplot();
}

void FormColorLengend::onLineNumChanged(const int &num)
{
    int rWidth = 20 + 120 * num;
    this->setMaximumWidth(ClabsoFTDpi::convertSize(rWidth));
    this->setMinimumWidth(ClabsoFTDpi::convertSize(rWidth));
}
