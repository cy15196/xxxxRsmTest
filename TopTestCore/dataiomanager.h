#ifndef DATAIOMANAGER_H
#define DATAIOMANAGER_H

#include <QMap>
#include <QPair>
#include <QPointF>
#include "toptestcore_global.h"

///time - <port,value>

typedef QMap<QString , QList<double> > TopTestFileDataMap;

typedef QMap<int,QList<QPointF> > TopTestChartDataMap;

//class SwitchablePortData
//{
//    SwitchablePortData(const int & portNum = 8);
//    void setPortOpenState(int portIndex, bool opened);
//private:
//    int portNum;
//};

class TOPTESTCORESHARED_EXPORT DataIOManager
{
public:
    DataIOManager();

    static TopTestFileDataMap readDataFile(const QString& fileName);
    static void saveDataToFile(const TopTestFileDataMap& dataMap, const QString& fileName);

    static TopTestChartDataMap convertFileDataToChartData(const TopTestFileDataMap& dataMap);
    static TopTestFileDataMap convertChartDataToFileData(const TopTestChartDataMap& chartMap);
private:

};

#endif // DATAIOMANAGER_H
