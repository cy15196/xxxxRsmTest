#pragma execution_character_set("utf-8")

#include "formresulttable.h"
#include "ui_formresulttable.h"

#include "toptestenum.h"

#include <QTextStream>
#include <QTime>

#include "qcpchartwidget.h"


//inline QString getAlgoString(int algoType)
//{
//    switch(algoType)
//    {
//        case TopTest::MaxTemputure:
//        {
//            return tr("峰值温度");
//        }
//            break;
//        case TopTest::IncreaseTime:
//        {
//            return tr("上升时间");
//        }
//            break;
//        case TopTest::IncreaseRatio:
//        {
//            return tr("上升斜率");
//        }
//            break;
//        case TopTest::AboveTime:
//        {
//            return tr("以上时间");
//        }
//            break;
//        default:
//            break;
//    }

//    return "";
//}

FormResultTable::FormResultTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormResultTable)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
//    test();
}

FormResultTable::~FormResultTable()
{
    delete ui;
}

void FormResultTable::setPortInfo(const QMap<int, QPair<QString, QColor> > &portInfo)
{
    ui->tableWidget->clear();
    removeAllColumn();
    currentPortInfo = portInfo;
    ui->tableWidget->setRowCount(5 +  portInfo.keys().count());
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setColumnWidth(0,40);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setItem(
                0,0,new QTableWidgetItem(tr("序号")));
    ui->tableWidget->setItem(
                0,1,new QTableWidgetItem(tr("通道名称")));

    ui->tableWidget->setSpan(0,0,2,1);
    ui->tableWidget->setSpan(0,1,2,1);

    foreach (int port, portInfo.keys()) {
        ui->tableWidget->setItem(
                    port+1,0,new QTableWidgetItem(QString::number(port)));
        ui->tableWidget->setItem(
                    port+1,1,new QTableWidgetItem(portInfo[port].first));
    }

    int termRow = portInfo.keys().count()+2;
    {
        QTableWidgetItem* pwiItem =
                new QTableWidgetItem(tr("PWI"));
        pwiItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem( termRow,0,pwiItem);
        ui->tableWidget->setSpan(termRow,0,1,2);
        termRow++;
    }

//    {
//        QTableWidgetItem* pwiItem =
//                new QTableWidgetItem(tr("分析条件"));
//        pwiItem->setTextAlignment(Qt::AlignCenter);
//        ui->tableWidget->setItem( termRow,0,pwiItem);
//        ui->tableWidget->setSpan( termRow,0,2,2);
//        termRow+=2;
//    }

    {
        QTableWidgetItem* pwiItem =
                new QTableWidgetItem(tr("参照标准"));
        pwiItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem( termRow,0,pwiItem);
        ui->tableWidget->setSpan( termRow,0,2,2);
        termRow+=2;
    }

    fillResult(currentProgressResult);
}

void FormResultTable::removeAllColumn(int remianNum)
{
    while(ui->tableWidget->columnCount() >remianNum)
    {
        ui->tableWidget->removeColumn(ui->tableWidget->columnCount()-1);
    }
}

void FormResultTable::fillResult(const QMap<int, QList<QMap<int, QVariant> > > &progressResult)
{

//    ui->tableWidget->horizontalHeader()->setS;
    removeAllColumn(2);
    foreach (int algoType, progressResult.keys()) {
        int typeColCount = 0;
        foreach (auto oneResult, progressResult.value(algoType)) {
            ui->tableWidget->insertColumn(ui->tableWidget->columnCount());

            for(int row =0 ;row <ui->tableWidget->rowCount();row++)
            {
                QTableWidgetItem* item = new QTableWidgetItem("-");
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(row,ui->tableWidget->columnCount()-1,item);
            }

            foreach (int rowKey, oneResult.keys()) {
                ///step over the empty result
                QString valueStr = oneResult[rowKey].toString();
                if(valueStr.isEmpty())
                    continue;

                QTableWidgetItem* item = new QTableWidgetItem;
                item->setTextAlignment(Qt::AlignCenter);
                item->setData(Qt::DisplayRole,oneResult[rowKey]);
                //keep it in user data
                item->setData(Qt::UserRole,oneResult[rowKey]);
                ui->tableWidget->setItem(rowKey,ui->tableWidget->columnCount()-1,item);
            }

            typeColCount++;
        }
        ui->tableWidget->setSpan(0,ui->tableWidget->columnCount()-typeColCount,1,typeColCount);
        QTableWidgetItem* typeLabelItem = new QTableWidgetItem;
        typeLabelItem->setData(Qt::DisplayRole,TopTest::getProgressTypeTranslate(algoType));
        typeLabelItem->setData(Qt::UserRole,algoType);
        typeLabelItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(
                    0,
                    ui->tableWidget->columnCount()-typeColCount,
                    typeLabelItem);
    }

    currentProgressResult = progressResult;
}

void FormResultTable::test()
{
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(18);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnWidth(0,30);
    ui->tableWidget->setSpan(0,0,1,2);
    ui->tableWidget->setSpan(1,0,1,2);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setStretchLastSection(true);

    QTableWidgetItem* item0 = new QTableWidgetItem(tr("PWI"));
    item0->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0,0,item0);

    QTableWidgetItem* item1 = new QTableWidgetItem(QString::fromStdWString(L"RefStander(s)"));
    item1->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(1,0,item1);

    for(int i=2;i<18;i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i-1));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item);
    }


}

QTableWidget *FormResultTable::getTableWidget()
{
    return ui->tableWidget;
}

void FormResultTable::onTimeTypeChanged(int timeType)
{
    ///the result only has time spec length
    if(timeType >= QCPChartWidget::TimeLabel_Time)
        timeType = QCPChartWidget::TimeLabel_HMS;

    qDebug()<<"FormResultTable::onTimeTypeChanged";
    if(currentTimeType == timeType)
        return;

    qDebug()<<"form"
           <<QCPChartWidget::getTimeLabelTypeToString(currentTimeType)
           <<"to"
           <<QCPChartWidget::getTimeLabelTypeToString(timeType);


    ///test convert function
    static QString currentText = "4652";

    qDebug()<<"before convert the value is"<<currentText;
    QString toValue = convertTimeValue(currentText,timeType);
    qDebug()<<"after convert the value is"<<toValue;


    ///real convert str to now timeType
    for(int j = 2 ; j < ui->tableWidget->columnCount(); j++ )
    {
        QTableWidgetItem* algoTypeItem = ui->tableWidget->item(0,j);
        if(!algoTypeItem)
            continue;
        int algoType = algoTypeItem->data(Qt::UserRole).toInt();
        int spanCount = ui->tableWidget->columnSpan(0,j);
        int nowColumn = j;
        for(int k =0 ; k < spanCount; k++)
        {
            j = nowColumn + k;
            switch (algoType) {
            case TopTest::MaxTemputure:
                break;
            case TopTest::IncreaseTime:
            case TopTest::AboveTime:
            case TopTest::DecreaseTime:
            case TopTest::DistanceToMaxTimeSection:
            case TopTest::TemputureBetweenTime:
            case TopTest::MaxIncreaseTime:
            case TopTest::MaxDecreaseTime:
            case TopTest::TestValuedTime:
            {
                for(int i = 2 ; i < ui->tableWidget->rowCount() - 3 ; i++)
                {
                    QTableWidgetItem* valueItem = ui->tableWidget->item(i,j);
                    if(!valueItem)
                        continue;
                    QString valueStr = valueItem->data(Qt::UserRole).toString();
                    valueItem->setData(Qt::DisplayRole,convertTimeValue(valueStr,timeType));
                }
                QTableWidgetItem* unitItem = ui->tableWidget->item(ui->tableWidget->rowCount() - 1,j);
                if(unitItem)
                {
                    unitItem->setData(Qt::DisplayRole,
                                   QCPChartWidget::getTimeLabelTypeToString(timeType).split("_").last());
                }
            }

                break;


            case TopTest::IncreaseRatio:
            case TopTest::DecreaseRatio:
            case TopTest::MaxIncreaseRatio:
            case TopTest::MaxDecreaseRatio:
            case TopTest::AverageRatio:
            case TopTest::MaxPortMaxDeltaValue:
                break;
            default:
                break;
            }
        }
    }


    currentTimeType = timeType;
}

QString convertTempValue(const QString &currentStr, int toType)
{
    QString ret = currentStr;

    double tempInC = 0.0;
    bool ok = false;
    tempInC = ret.toDouble(&ok);

    if(!ok)
    {
        qDebug()<<"convert tmep in c faild, current value is"<<currentStr;
        if(currentStr.isEmpty())
            ret = "-";
        return ret;
    }

    switch (toType) {
    case QCPChartWidget::TemperatureLabel_C:
        ret = QString::number(tempInC);
        break;
    case QCPChartWidget::TemperatureLabel_F:
        ret = QString::number(1.8 * tempInC + 32);
        break;
    case QCPChartWidget::TemperatureLabel_K:
        ret = QString::number(tempInC + 273.15);
        break;
    default:
        break;
    }

    return ret;

}

void FormResultTable::onTempTypeChanged(int tempType)
{
    ///the result only has time spec length
//    if(tempType >= QCPChartWidget::TimeLabel_Time)
//        tempType = QCPChartWidget::TimeLabel_HMS;

    qDebug()<<"FormResultTable::onTempTypeChanged";
    if(currentTempType == tempType)
        return;

    qDebug()<<"form"
           <<QCPChartWidget::getTemperatureLabelTypeToString(currentTempType)
           <<"to"
           <<QCPChartWidget::getTemperatureLabelTypeToString(tempType);


//    ///test convert function
//    static QString currentText = "4652";

//    qDebug()<<"before convert the value is"<<currentText;
//    QString toValue = convertTimeValue(currentText,timeType);
//    qDebug()<<"after convert the value is"<<toValue;


    ///real convert str to now timeType
    for(int j = 2 ; j < ui->tableWidget->columnCount(); j++ )
    {
        QTableWidgetItem* algoTypeItem = ui->tableWidget->item(0,j);
        if(!algoTypeItem)
            continue;
        int algoType = algoTypeItem->data(Qt::UserRole).toInt();
        int spanCount = ui->tableWidget->columnSpan(0,j);
        int nowColumn = j;
        for(int k =0 ; k < spanCount; k++)
        {
            j = nowColumn + k;
            switch (algoType) {
            case TopTest::MaxTemputure:
            case TopTest::MaxPortMaxDeltaValue:
            {
                for(int i = 2 ; i < ui->tableWidget->rowCount() - 3 ; i++)
                {
                    QTableWidgetItem* valueItem = ui->tableWidget->item(i,j);
                    if(!valueItem)
                        continue;
                    QString valueStr = valueItem->data(Qt::UserRole).toString();
                    valueItem->setData(Qt::DisplayRole,convertTempValue(valueStr,tempType));
                }
                QTableWidgetItem* unitItem = ui->tableWidget->item(ui->tableWidget->rowCount() - 1,j);
                if(unitItem)
                {
                    unitItem->setData(Qt::DisplayRole,
                                   QCPChartWidget::getTemperatureLabelTypeToString(tempType).split("_").last());
                }
            }
                break;
            case TopTest::IncreaseTime:
            case TopTest::AboveTime:
            case TopTest::DecreaseTime:
            case TopTest::DistanceToMaxTimeSection:
            case TopTest::TemputureBetweenTime:
            case TopTest::MaxIncreaseTime:
            case TopTest::MaxDecreaseTime:
            case TopTest::TestValuedTime:
                break;


            case TopTest::IncreaseRatio:
            case TopTest::DecreaseRatio:
            case TopTest::MaxIncreaseRatio:
            case TopTest::MaxDecreaseRatio:
            case TopTest::AverageRatio:
                break;
            default:
                break;
            }
        }
    }


    currentTempType = tempType;
}

QString FormResultTable::convertTimeValue(const QString &currentStr, int toType)
{
    QString ret = currentStr;

    double timeInS = 0.0;
    bool ok = false;
    timeInS = ret.toDouble(&ok);

    if(!ok)
    {
        qDebug()<<"convert time in s faild, current value is"<<currentStr;
        if(currentStr.isEmpty())
            ret = "-";
        return ret;
    }

    ret = QCPChartWidget::getTimeValueToTypeLabel(timeInS,toType);

    return ret;

}
