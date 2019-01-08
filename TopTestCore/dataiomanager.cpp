#include "dataiomanager.h"
#include <QSettings>
#include <QDateTime>
#include <QFile>
#include <iostream>
#include <QTextStream>


DataIOManager::DataIOManager()
{

}

TopTestFileDataMap DataIOManager::readDataFile(const QString &fileName)
{
    TopTestFileDataMap dataMap;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        std::cout<<"readDataFile"<<"open file error";
        return dataMap;
    }

    QTextStream ts(&file);

    QString header = ts.readLine();
    if(!header.startsWith("#Header:"))
    {
        std::cout<<"readDataFile"<<"can't find header at the first line";
        return dataMap;
    }

    QString tableHeader = ts.readLine();
    QStringList arrayLabelList = tableHeader.remove("\t").remove("\n").split(",",QString::SkipEmptyParts);



    int lineNum = 3;
    while(!ts.atEnd())
    {
        QString lineStr = ts.readLine();
        if(lineStr.isEmpty())
            continue;
        QStringList lineDataList = lineStr.remove("\t").remove("\n").split(",",QString::SkipEmptyParts);

        if(lineDataList.size()!=arrayLabelList.size())
        {
            std::cout<<"readDataFile"<<"data count mismatch in line "<<lineNum;
            file.close();
            return dataMap;
        }

        for(int i=0 ;i<arrayLabelList.size();i++)
        {
            QString key = arrayLabelList.at(i);
            double value = QString(lineDataList.at(i)).toDouble();
            dataMap[key].append(value);
        }


        lineNum++;
    }

    file.close();
    return dataMap;


}

void DataIOManager::saveDataToFile(const TopTestFileDataMap &dataMap, const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly))
    {
        std::cout<<"SaveDataToFile"<<"open file error";
        return;
    }

    QList<double> timeList = dataMap.value("Time");

    Q_ASSERT_X(timeList.count() == dataMap.values().first().count(),
               "saveDataToFile","dataArray's length is not equal the time list");

    QTextStream ts(&file);
    ts<<"#Header:The TopTestData file created at "<<QDateTime::currentDateTime().toString()<<"\n";
//    ts<<"#PortNum:"



    ts<<"Time"<<",\t";
    foreach (QString portName, dataMap.keys()) {
        if(portName!="Time")
            ts<<portName+",\t";
    }
    ts<<"\n";

    for(int i=0; i<timeList.count(); i++ )
    {
        ts<<timeList.at(i)<<",\t";
        foreach (QString portName, dataMap.keys()) {
            if(portName!="Time")
                ts<<dataMap.value(portName).at(i)<<",\t";
        }
        ts<<"\n";
    }

    file.close();
}

TopTestChartDataMap DataIOManager::convertFileDataToChartData(const TopTestFileDataMap &dataMap)
{
    TopTestChartDataMap chartDataMap;


    QList<double> timeList = dataMap.value("Time");


    foreach (QString portKey, dataMap.keys()) {
        if(portKey.startsWith("Port"))
        {
            int portNum = QString(portKey).remove("Port").toInt();
            QList<QPointF> portPointList;
            QList<double> portDataList = dataMap.value(portKey);
            for(int i=0;i<timeList.size() && i< portDataList.size();i++)
            {
                portPointList.append( QPointF(timeList.at(i),portDataList.at(i)) );
            }

            chartDataMap.insert(portNum,portPointList);
        }
    }


    return chartDataMap;
}

TopTestFileDataMap DataIOManager::convertChartDataToFileData(const TopTestChartDataMap &chartMap)
{
    TopTestFileDataMap chartFileMap;

    return chartFileMap;
}
