#ifndef TOPTESTSTRUCTDATA_H
#define TOPTESTSTRUCTDATA_H

#include "toptestcore_global.h"
#include "toptestenum.h"
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include <QPointF>

namespace TopTest {

//struct SerialUserData
//{
//    QList<double> portVolValues;
//};
//struct SerialConfigureData
//{
//    QMap<TopTest::RsmCMD, QVariant> dataMap;
//};

struct TempSectionItem
{
    QString name;
    double startTime = 0;
    double endTime = 0;
    double upValue = 0;
    double downValue = 0 ;

};

struct TempSection{
   bool isDoubleSection = false;
   QList<TempSectionItem> sections;
};

struct WaveInfo
{
   int waveCount = 0;
   QMap<int,QString> waveNameMap;
   QMap<int,QPointF> waveTimeSection;
   bool useHandle = false;
};

struct SerialData
{
    QMap<TopTest::RsmCMD, QVariant> userData;
    QMap<TopTest::RsmCMD, QVariant> configData;
    QList<double> portTempValues;
    QList<double> portVolValues;
    QList<int> portADValues;
    QList<int> portWholeAdjustValues;
    QList<int> portZeroAdjustValues;
};

struct RecordInfoData{

    void loadMap(const QVariantMap& map)
    {
        fileName = map.value("FileName").toString();
        dateTime = map.value("DateTime").toDateTime();
        Comment = map.value("Comment").toString();
        User = map.value("User").toString();
        Company = map.value("Company").toString();
        Spy = map.value("Spy").toString();
        saveFilePath = map.value("SaveFilePath").toString();

        Progress = map.value("Progress").toString();
        Product = map.value("Product").toString();
        Device = map.value("Device").toString();
        LineVelocity = map.value("LineVelocity").toDouble();

        Line = map.value("Line").toString();


    }

    QVariantMap toVariantMap()
    {
        QVariantMap map;
        map.insert("FileName",fileName);
        map.insert("DateTime",dateTime);
        map.insert("Comment",Comment);
        map.insert("User",User);
        map.insert("Company",Company);
        map.insert("Spy",Spy);
        map.insert("Progress",Progress);
        map.insert("Product",Product);
        map.insert("Device",Device);
        map.insert("LineVelocity",LineVelocity);
        map.insert("Line",Line);
        map.insert("SaveFilePath",saveFilePath);
        return map;
    }

    QString fileName;
    QString Line;
    QDateTime dateTime;
    int portNum;
    double sampleTime;
    int dataCount;
    QString Comment;
    QString User;
    QString Spy;
    QString Company;

    QString Progress;
    QString Product;
    QString Device;
    double LineVelocity;

    QString saveFilePath;
};

struct InnerFileSettingData
{
    QMap<int, QList<QPair<QList<int>, QMap<QString, double> > > > processData;
    QMap<QString,QVariant> typeDeviceMap;
    QMap<QString,QVariant> productMap;
};

struct FileData
{
    RecordInfoData info;
    QByteArray pointData;
    InnerFileSettingData innerFileSetting;
};

struct AxisLabelInfo{
    QPointF range = QPointF(0,1);
    int smallTickCountPerBigStep = 5;
//    double bigStep = 100.0;
    int bigTickCount = 10;
    QString labelFormat = "%6g";
    bool showAxisTitle = true;
    QString axisTitle = "Axis";
    bool showGridLines = true;
    QStringList axisLabelTypeList;
    QString currentLabelType;
};


enum LineType
{
    HLine,
    VLine,
    TwoPointLine
};

struct LineInfo{
    LineType type;
    double value;
    QPair<QPointF,QPointF> pointPair;
};



struct ProjectInfo
{
    ProjectInfo()
    {
        type = ProjectType::DataAnalysis;
        isValid = false;
    }

    QString path;
    QString name;
    ProjectType type;
    bool isValid;
    QMap<QString,QVariant> projectMap;
};

struct ProgressInfoItem
{
    int portNo;
    int algoType;
    QPair<QVariant,QVariant> refRange;
    QPair<double,double> realRange;
    double pwiValue;
};

}

#endif // TOPTESTSTRUCTDATA_H
