#include "dataprogresshandlealgorithm.h"

#include "toptestenum.h"

#include <QDebug>

DataProgressHandleAlgorithm* instanceHandle = 0;

//QMap<QString,double>(*progressAlgorithm ) (const QVector<QPointF>& , const QMap<QString,double>&) ;


//QMap<QString,QPointF> parserMaxPoint(const QVector<QPointF>& curvePointList)
//{
//    QPointF MaxTempPoint = curvePointList.first();
//    foreach (QPointF point, curvePointList) {
//        if(point.y()>MaxTempPoint.y())
//            MaxTempPoint = point;
//    }



//}

inline QPointF ParserMax(const QVector<QPointF>& data)
{
    QPointF ret = data.first();
    double maxTemperature = 0.0;
    for(int i=0; i<data.count(); i++) {
        QPointF currentPoint = data.at(i);
        if(currentPoint.y() > maxTemperature)
        {
            ret = currentPoint;
            maxTemperature = currentPoint.y();
        }
    }

    return ret;
}

QMap<QString,QVariant> ParserTestValuedTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double startTemperatureValue = parameter.value("TemperatureMinValue");
    double endTemperatureValue = parameter.value("TemperatureMaxValue");

    result.insert("PresetStr",QString("%1->Max->%2").arg(startTemperatureValue).arg(endTemperatureValue));

    if(data.count()<2)
        return result;

    QPointF startPoint;
    QPointF endPoint;

    QPointF maxPoint = ParserMax(data);

    for(int i = 0; i < data.indexOf(maxPoint); i++) {
        QPointF currentPoint = data.at(i);
        if(currentPoint.y() > startTemperatureValue)
        {
            startPoint = currentPoint;

            for(int i = data.indexOf(maxPoint); i<data.count(); i++) {
                QPointF currentPoint = data.at(i);
                if(currentPoint.y() < endTemperatureValue)
                {
                    endPoint = data.at(i-1);
                    break;
                }
            }
            endPoint = data.last();
            break;
        }
    }

    double testValueTime = 0.0;
    if(!startPoint.isNull())
        testValueTime = endPoint.x() - startPoint.x();

    result.insert("TestValuedTime",testValueTime);
    return result;
}


QMap<QString,QVariant> ParserMaxIncreaseTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;

    if(data.count()<2)
        return result;

    QPointF maxPoint = ParserMax(data);

    result.insert("MaxIncreaseTime",maxPoint.x() - data.first().x());

    return result;
}

QMap<QString,QVariant> ParserMaxDecreaseTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;

    if(data.count()<2)
        return result;

    QPointF maxPoint = ParserMax(data);

    result.insert("MaxDecreaseTime", data.last().x() - maxPoint.x());

    return result;
}

QMap<QString,QVariant> ParserMaxPortMaxDeltaValue (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;

    if(data.count()<2)
        return result;

//    result.insert("PresetStr",QString("%1").arg("MaxDeltaValue"));

    QPointF maxPoint = ParserMax(data);

    result.insert("MaxPortMaxDeltaValue",maxPoint.y());

    return result;
}

QMap<QString,QVariant> ParserMaxTemperature (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;

    if(data.count()<2)
        return result;

    QPointF maxPoint = ParserMax(data);

    result.insert("MaxTemperature",maxPoint.y());

    return result;
}

QMap<QString,QVariant> ParserDistanceToMaxTimeSection (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double distanceToMaxTimeSection = 0.0;

    double sectionHeight = parameter.value("SectionHeight");

    result.insert("PresetStr",QString("%1").arg(sectionHeight));

    if(data.count()<2)
        return result;


    QPointF maxTemperaturePoint = ParserMax(data);

    int index = data.indexOf(maxTemperaturePoint);

    QPointF startSectionPoint = maxTemperaturePoint;
    QPointF endSectionPoint = maxTemperaturePoint;
    //serach start point
    for(int j = index ; j>=0; j--)
    {
        QPointF currentPoint = data.at(j);
        if(maxTemperaturePoint.y() - currentPoint.y() < sectionHeight)
            startSectionPoint = currentPoint;
        else
            break;
    }
    //serach end point
    for(int j = index ; j<data.count(); j++)
    {
         QPointF currentPoint = data.at(j);
        if(maxTemperaturePoint.y() - currentPoint.y() < sectionHeight)
            endSectionPoint = currentPoint;
        else
            break;
    }

    distanceToMaxTimeSection = endSectionPoint.x() - startSectionPoint.x();

    result.insert("DistanceToMaxTimeSection",distanceToMaxTimeSection);

    return result;
}


QMap<QString,QVariant> ParserAboveTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;

    double tempRemainValue = parameter.value("TemperatureValue");
    result.insert("PresetStr",QString("%1").arg(tempRemainValue));

    if(data.count()<2)
        return result;

    double aboveTime = 0.0;

    QPointF startAbovePoint;
    QPointF endAbovePoint;
    for(int i=0; i<data.count(); i++) {
        QPointF currentPoint = data.at(i);

        if(startAbovePoint.isNull()) ///尚未进入维持温区
        {
            if(currentPoint.y()>tempRemainValue) ///触发进入条件
                startAbovePoint = currentPoint;
        }
        else if(currentPoint.y()<tempRemainValue) ///触及维温下限
        {
            endAbovePoint = currentPoint;

            if(endAbovePoint != startAbovePoint)
                aboveTime += (endAbovePoint - startAbovePoint).x();

            startAbovePoint = QPointF();
            endAbovePoint = QPointF();
        }
    }
    if(!startAbovePoint.isNull() && endAbovePoint.isNull() && data.last().x() < tempRemainValue)
    {
        endAbovePoint = data.last();
        if(endAbovePoint != startAbovePoint)
            aboveTime += (endAbovePoint - startAbovePoint).x();
    }

    result.insert("AboveTime",aboveTime);

    return result;
}

QMap<QString,QVariant> ParserTemputureBetweenTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");
    result.insert("PresetStr",QString("%1 -> %2").arg(tempMinValue).arg(tempMaxValue));

    if(data.count()<2)
        return result;

    double betweenTime = 0.0;

    QPointF startBetweenPoint;
    QPointF endBetweenPoint;
    QPointF lastPoint;
    for(int i=0; i<data.count(); i++) {
        QPointF currentPoint = data.at(i);

        if(startBetweenPoint.isNull()) ///尚未进入上下限温区
        {
            if( (lastPoint.y() <tempMinValue && currentPoint.y() > tempMinValue)
                || (lastPoint.y() > tempMaxValue && currentPoint.y() < tempMaxValue) ) ///触发进入条件
                startBetweenPoint = currentPoint;
        }
        else  if( (lastPoint.y() <tempMinValue && currentPoint.y() > tempMinValue)
                || (lastPoint.y() > tempMaxValue && currentPoint.y() < tempMaxValue) ) ///触及退出条件
        {
            endBetweenPoint = currentPoint;

            if(endBetweenPoint != startBetweenPoint)
                betweenTime += (endBetweenPoint - startBetweenPoint).x();

            endBetweenPoint = QPointF();
            startBetweenPoint = QPointF();
        }

        lastPoint = currentPoint;
    }
    if( !startBetweenPoint.isNull()
            && endBetweenPoint.isNull() )
    {
        endBetweenPoint = data.last();
        if(endBetweenPoint != startBetweenPoint)
            betweenTime += (endBetweenPoint - startBetweenPoint).x();
    }

    result.insert("TemputureBetweenTime",betweenTime);

    return result;
}

inline QPointF  LinearInsertPoint(const QPointF& A, const QPointF& B, double x)
{
    return QPointF(x,A.y() + (x-A.x()) * (B.y()- A.y()) / (B.x() -A.x()));
}


inline void ParserDecrease(QPointF&startDecreasePoint, QPointF& endDecreasePoint,
                           const QVector<QPointF>& data,
                           double tempMaxValue,double tempMinValue)
{
    if(data.isEmpty())
        return;

    QPointF MaxTempPoint = data.last();
    for(int i = data.count() -1; i >= 0 ; i--) {
        QPointF currentPoint = data.at(i);

        if(currentPoint.y()>MaxTempPoint.y())
            MaxTempPoint = currentPoint;

        if(endDecreasePoint.isNull()) ///尚未进入升温区
        {
            if(currentPoint.y()>tempMinValue) ///触发进入条件
                endDecreasePoint = currentPoint;
        }
        else if(currentPoint.y()>tempMaxValue) ///触及升温上限
        {
            startDecreasePoint = currentPoint;
            break;
        }
    }
    if(!endDecreasePoint.isNull() && startDecreasePoint.isNull())
        startDecreasePoint = MaxTempPoint;
}


QMap<QString,QVariant> ParserDecreaseTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{

    QMap<QString,QVariant> result;

    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");

    result.insert("PresetStr",QString("%1 -> %2").arg(tempMaxValue).arg(tempMinValue));

    QPointF startDecreasePoint;
    QPointF endDecreasePoint;

    ParserDecrease(startDecreasePoint,endDecreasePoint,data,tempMaxValue,tempMinValue);

    QPointF decrease = endDecreasePoint - startDecreasePoint;
    result.insert("DecreaseTime",decrease.x());

    return result;

}

QMap<QString,QVariant> ParserDecreaseRatio (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{

    QMap<QString,QVariant> result;

    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");
    double ratioTime = parameter.value("RatioSampleTime");

    if(ratioTime <= 0)
        return result;

    result.insert("PresetStr",QString("%1 -> %2 in %3s ").arg(tempMaxValue).arg(tempMinValue).arg(ratioTime));

    QPointF startDecreasePoint;
    QPointF endDecreasePoint;

    //parser decrease range

    ParserDecrease(startDecreasePoint,endDecreasePoint,data,tempMaxValue,tempMinValue);

    //cal decrease ratio
    double startTime = startDecreasePoint.x();
    double endTime = startTime + ratioTime;
    double minRatio = 0;
    QPointF startPoint = startDecreasePoint;
    QPointF endPoint = startDecreasePoint;
    int endIndex = data.indexOf(startPoint);
    while(startTime< endDecreasePoint.x())
    {
        while(endPoint.x() - startPoint.x() < ratioTime && endIndex < data.count()-1 )
        {
            endPoint = data.at(endIndex++);
        }

        endPoint = LinearInsertPoint(data.at(endIndex-1),data.at(endIndex),endTime);
        double currentRadio = (endPoint.y() - startPoint.y()) / (endPoint.x() - startPoint.x());
        if(currentRadio < minRatio)
            minRatio = currentRadio;

        //move to next
        startPoint = endPoint;
        startTime = endTime;
        endTime = startTime + ratioTime;
    }

    result.insert("DecreaseRatio",minRatio);

    return result;

}

QMap<QString,QVariant> ParserMaxDecreaseRatio (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{

    QMap<QString,QVariant> result;

//    double tempMaxValue = parameter.value("TemperatureMaxValue");
//    double tempMinValue = parameter.value("TemperatureMinValue");
    double ratioTime = parameter.value("RatioSampleTime");

    if(ratioTime <= 0)
        return result;

    if(data.count()<2)
        return result;

    result.insert("PresetStr",QString("Tmax -> Tend in %3s ").arg(ratioTime));

    QPointF startDecreasePoint = ParserMax(data);
    QPointF endDecreasePoint = data.last();

    //cal decrease ratio
    double startTime = startDecreasePoint.x();
    double endTime = startTime + ratioTime;
    double minRatio = 0;
    QPointF startPoint = startDecreasePoint;
    QPointF endPoint = startDecreasePoint;
    int endIndex = data.indexOf(startPoint);
    while(startTime< endDecreasePoint.x())
    {
        while(endPoint.x() - startPoint.x() < ratioTime && endIndex < data.count()-1 )
        {
            endPoint = data.at(endIndex++);
        }

        endPoint = LinearInsertPoint(data.at(endIndex-1),data.at(endIndex),endTime);
        double currentRadio = (endPoint.y() - startPoint.y()) / (endPoint.x() - startPoint.x());
        if(currentRadio < minRatio)
            minRatio = currentRadio;

        //move to next
        startPoint = endPoint;
        startTime = endTime;
        endTime = startTime + ratioTime;
    }

    result.insert("MaxDecreaseRatio",minRatio);

    return result;

}

inline void ParserIncrease(QPointF&startIncreasePoint, QPointF& endIncreasePoint,
                           const QVector<QPointF>& data,
                           double tempMaxValue,double tempMinValue)
{

    QPointF MaxTempPoint = data.first();
    for(int i=0; i<data.count(); i++) {
        QPointF currentPoint = data.at(i);

        if(currentPoint.y()>MaxTempPoint.y())
            MaxTempPoint = currentPoint;

        if(startIncreasePoint.isNull()) ///尚未进入升温区
        {
            if(currentPoint.y()>tempMinValue) ///触发进入条件
                startIncreasePoint = currentPoint;
        }
        else if(currentPoint.y()>tempMaxValue) ///触及升温上限
        {
            endIncreasePoint = currentPoint;
            break;
        }
    }
    if(!startIncreasePoint.isNull() && endIncreasePoint.isNull())
        endIncreasePoint = MaxTempPoint;
}

QMap<QString,QVariant> ParserIncreaseTime (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");
    result.insert("PresetStr",QString("%1 -> %2").arg(tempMinValue).arg(tempMaxValue));

    if(data.count()<2)
        return result;

    QPointF startIncreasePoint;
    QPointF endIncreasePoint;

    ParserIncrease(startIncreasePoint,endIncreasePoint,data,tempMaxValue,tempMinValue);

    QPointF increase = endIncreasePoint - startIncreasePoint;
    result.insert("IncreaseTime",increase.x());

    return result;
}

QMap<QString,QVariant> ParserIncreaseRatio (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");
    double ratioTime = parameter.value("RatioSampleTime");
    result.insert("PresetStr",QString("%1 -> %2 in %3s ").arg(tempMinValue).arg(tempMaxValue).arg(ratioTime));

    if(data.count()<2)
        return result;

    QPointF startIncreasePoint;
    QPointF endIncreasePoint;

    ParserIncrease(startIncreasePoint,endIncreasePoint,data,tempMaxValue,tempMinValue);

    //cal increase ratio
    double startTime = startIncreasePoint.x();
    double endTime = startTime + ratioTime;
    double maxRatio = 0;
    QPointF startPoint = startIncreasePoint;
    QPointF endPoint = startIncreasePoint;
    int endIndex = data.indexOf(startPoint);
    while(startTime< endIncreasePoint.x())
    {
        while(endPoint.x() - startPoint.x() < ratioTime && endIndex < data.count()-1 )
        {
            endPoint = data.at(endIndex++);
        }

        endPoint = LinearInsertPoint(data.at(endIndex-1),data.at(endIndex),endTime);
        double currentRadio = (endPoint.y() - startPoint.y()) / (endPoint.x() - startPoint.x());
        if(currentRadio > maxRatio)
            maxRatio = currentRadio;

        //move to next
        startPoint = endPoint;
        startTime = endTime;
        endTime = startTime + ratioTime;
    }

    result.insert("IncreaseRatio",maxRatio);
    return result;
}

QMap<QString,QVariant> ParserAverageRatio (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
    double tempMaxValue = parameter.value("TemperatureMaxValue");
    double tempMinValue = parameter.value("TemperatureMinValue");
    result.insert("PresetStr",QString("%1 -> %2").arg(tempMinValue).arg(tempMaxValue));

    if(data.count()<2)
        return result;

    QPointF startIncreasePoint;
    QPointF endIncreasePoint;

    double averRatio = 0.0;

    ParserIncrease(startIncreasePoint,endIncreasePoint,data,tempMaxValue,tempMinValue);

    if(endIncreasePoint.x() > startIncreasePoint.x())
    {
        averRatio = (endIncreasePoint.y() - startIncreasePoint.y())
                /(endIncreasePoint.x() - startIncreasePoint.x());
    }

    result.insert("AverageRatio",averRatio);
    return result;
}

QMap<QString,QVariant> ParserMaxIncreaseRatio (const QVector<QPointF>& data , const QMap<QString,double>& parameter)
{
    QMap<QString,QVariant> result;
//    double tempMaxValue = parameter.value("TemperatureMaxValue");
//    double tempMinValue = parameter.value("TemperatureMinValue");
    double ratioTime = parameter.value("RatioSampleTime");
    result.insert("PresetStr",QString("Tstart -> Tmax in %3s ").arg(ratioTime));

    if(data.count()<2)
        return result;

    QPointF startIncreasePoint = data.first();
    QPointF endIncreasePoint = ParserMax(data);

    //cal increase ratio
    double startTime = startIncreasePoint.x();
    double endTime = startTime + ratioTime;
    double maxRatio = 0;
    QPointF startPoint = startIncreasePoint;
    QPointF endPoint = startIncreasePoint;
    int endIndex = data.indexOf(startPoint);
    while(startTime< endIncreasePoint.x())
    {
        while(endPoint.x() - startPoint.x() < ratioTime && endIndex < data.count()-1 )
        {
            endPoint = data.at(endIndex++);
        }

        endPoint = LinearInsertPoint(data.at(endIndex-1),data.at(endIndex),endTime);
        double currentRadio = (endPoint.y() - startPoint.y()) / (endPoint.x() - startPoint.x());
        if(currentRadio > maxRatio)
            maxRatio = currentRadio;

        //move to next
        startPoint = endPoint;
        startTime = endTime;
        endTime = startTime + ratioTime;
    }

    result.insert("MaxIncreaseRatio",maxRatio);
    return result;
}

DataProgressHandleAlgorithm *DataProgressHandleAlgorithm::instance()
{
    if(!instanceHandle)
        instanceHandle = new DataProgressHandleAlgorithm;
    return instanceHandle;
}

DataProgressHandleAlgorithm::DataProgressHandleAlgorithm()
{

}

void DataProgressHandleAlgorithm::setInput(const QVector<QPointF> &inputData)
{
    this->sourceData = inputData;
}

void DataProgressHandleAlgorithm::setParameters(const QMap<QString, double> &paramterMap)
{
    this->paramterMap = paramterMap;
}

void DataProgressHandleAlgorithm::setAlgoType(const int &type)
{
    this->currentAlgorithm = type;
}

QMap<QString, QVariant> DataProgressHandleAlgorithm::getCurrentAlgoResult()
{
    switch (currentAlgorithm) {
    case TopTest::MaxTemputure:
    {
        this->resultMap["PresetUnit"] = "-";
        this->resultMap["RefUnit"] = "C";
        this->resultMap["Result"] = this->resultMap["MaxTemperature"];
    }
        break;
    case TopTest::IncreaseTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["IncreaseTime"];
    }
        break;
    case TopTest::AboveTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["AboveTime"];
    }
        break;
    case TopTest::DecreaseTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["DecreaseTime"];
    }
        break;

    case TopTest::IncreaseRatio:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "C/s";
        this->resultMap["Result"] = this->resultMap["IncreaseRatio"];
    }
        break;

    case TopTest::DecreaseRatio:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "C/s";
        this->resultMap["Result"] = this->resultMap["DecreaseRatio"];
    }
        break;

    case TopTest::MaxIncreaseRatio:
    {
        this->resultMap["PresetUnit"] = "-";
        this->resultMap["RefUnit"] = "C/s";
        this->resultMap["Result"] = this->resultMap["MaxIncreaseRatio"];
    }
        break;

    case TopTest::MaxDecreaseRatio:
    {
        this->resultMap["PresetUnit"] = "-";
        this->resultMap["RefUnit"] = "C/s";
        this->resultMap["Result"] = this->resultMap["MaxDecreaseRatio"];
    }
        break;
    case TopTest::AverageRatio:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "C/s";
        this->resultMap["Result"] = this->resultMap["AverageRatio"];
    }
        break;
    case TopTest::DistanceToMaxTimeSection:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["DistanceToMaxTimeSection"];
    }
        break;

    case TopTest::TemputureBetweenTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["TemputureBetweenTime"];
    }
        break;

    case TopTest::MaxIncreaseTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["MaxIncreaseTime"];
    }
        break;

    case TopTest::MaxDecreaseTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["MaxDecreaseTime"];
    }
        break;

    case TopTest::MaxPortMaxDeltaValue:
    {
        this->resultMap["PresetUnit"] = "-";
        this->resultMap["RefUnit"] = "C";
        this->resultMap["Result"] = this->resultMap["MaxPortMaxDeltaValue"];
    }
        break;

    case TopTest::TestValuedTime:
    {
        this->resultMap["PresetUnit"] = "C";
        this->resultMap["RefUnit"] = "s";
        this->resultMap["Result"] = this->resultMap["TestValuedTime"];
    }
        break;


    default:
        break;
    }

    if(paramterMap.contains("RefMinValue") &&
        paramterMap.contains("RefMinValue"))
    {
        double refMinValue = paramterMap.value("RefMinValue");
        double refMaxValue = paramterMap.value("RefMaxValue");

        this->resultMap["RefStr"] = QString("%1 -> %2").arg(refMinValue).arg(refMaxValue);

        bool ok;
        double value = resultMap["Result"].toDouble(&ok);
        if(resultMap.contains("Result") && ok)
        {

            double pwiValue = 100.0 *
                    ( value - (refMinValue + refMaxValue) / 2 )
                    / (refMaxValue - refMinValue);
            this->resultMap["PWI"] = pwiValue;
        }
    }

    return this->resultMap;
}

void DataProgressHandleAlgorithm::update()
{

    this->sourceData = parserWaveData(this->sourceData,this->waveToWaveStartEndMap,currentWaveNo);

    switch (this->currentAlgorithm) {
    case TopTest::MaxTemputure:
    {
        this->resultMap = ParserMaxTemperature(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::IncreaseTime:
    {
        this->resultMap = ParserIncreaseTime(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::AboveTime:
    {
        this->resultMap = ParserAboveTime(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::DecreaseTime:
    {
        this->resultMap = ParserDecreaseTime(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::IncreaseRatio:
    {
        this->resultMap = ParserIncreaseRatio(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::DecreaseRatio:
    {
        this->resultMap = ParserDecreaseRatio(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::MaxIncreaseRatio:
    {
        this->resultMap = ParserMaxIncreaseRatio(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::MaxDecreaseRatio:
    {
        this->resultMap = ParserMaxDecreaseRatio(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::AverageRatio:
    {
        this->resultMap = ParserAverageRatio(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::DistanceToMaxTimeSection:
    {
        this->resultMap = ParserDistanceToMaxTimeSection(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::TemputureBetweenTime:
    {
        this->resultMap = ParserTemputureBetweenTime(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::MaxIncreaseTime:
    {
        this->resultMap = ParserMaxIncreaseTime(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::MaxDecreaseTime:
    {
        this->resultMap = ParserMaxDecreaseTime(this->sourceData,this->paramterMap);
    }
        break;

    case TopTest::MaxPortMaxDeltaValue:
    {
        this->resultMap = ParserMaxPortMaxDeltaValue(this->sourceData,this->paramterMap);
    }
        break;
    case TopTest::TestValuedTime:
    {
        this->resultMap = ParserTestValuedTime(this->sourceData,this->paramterMap);
    }
        break;

//    case TopTest::ReturnTime:
//    {
//    }
//        break;
//    case TopTest::RemainTime:
//    {
//        this->resultMap = ParserAboveTime(this->sourceData,this->paramterMap);
//    }
//        break;
//    case TopTest::TemputureRelativeTime:
//    {
//    }
//        break;

    default:
        break;
    }

    qDebug()<<this->paramterMap;
    qDebug()<<this->resultMap;
}

QMap<QString, QVariant> DataProgressHandleAlgorithm::getOutputData() const
{
    return this->resultMap;
}

QPointF DataProgressHandleAlgorithm::parserTempSectionRange(
        const QList<QVector<QPointF> > &portDataList,
        double startTemp,
        double endTemp)
{
    QPointF ret;

    if(portDataList.isEmpty())
        return ret;

    QList<QPointF> maxPointList;
    QList<QVector<QPointF> > realPointDataList;
    for(int j = 0 ; j < portDataList.count() ; j++)
    {
        QPointF maxPoint = ParserMax(portDataList.at(j));
        if(maxPoint.y() > startTemp)
        {
            maxPointList << maxPoint;
            realPointDataList<<portDataList.at(j);
        }
    }

    qDebug()<<"realPointDataList  count is"<<realPointDataList.count();
    if(realPointDataList.isEmpty())
        return ret;

    ///start
    for(int i = 0 ; i < realPointDataList.first().count() ; i++)
    {
        for(int j = 0 ; j < realPointDataList.count() ; j++)
        {
            QPointF currentPoint = realPointDataList.at(j).at(i);
            if(currentPoint.y() >= startTemp
                    && currentPoint.x() < maxPointList.at(j).x() )
            {
                ret.setX(currentPoint.x());
                break;
            }
        }
        if(ret.x())
            break;
    }

    ///end
    for(int i = 0 ; i < realPointDataList.first().count() ; i++)
    {
        for(int j = 0 ; j < realPointDataList.count() ; j++)
        {
            QPointF currentPoint = realPointDataList.at(j).at(i);
            if(currentPoint.y() <= endTemp
                    && currentPoint.x() > maxPointList.at(j).x() )
            {
                ret.setY(currentPoint.x());
                break;
            }
        }
        if(ret.y())
            break;
    }

    if(ret.y() == 0)
        ret.setY(realPointDataList.first().last().x());

    return ret;
}

int DataProgressHandleAlgorithm::getCurrentWaveNo() const
{
    return currentWaveNo;
}

void DataProgressHandleAlgorithm::setCurrentWaveNo(int value)
{
    currentWaveNo = value;
}

QMap<int, QPointF> DataProgressHandleAlgorithm::getWaveToWaveStartEndMap() const
{
    return waveToWaveStartEndMap;
}

void DataProgressHandleAlgorithm::setWaveToWaveStartEndMap(const QMap<int, QPointF> &value)
{
    waveToWaveStartEndMap = value;
}

void DataProgressHandleAlgorithm::insertWaveStartEnd(const int &waveNo, const QPointF &waveStartEnd)
{
    waveToWaveStartEndMap[waveNo] = waveStartEnd;
}

QPointF DataProgressHandleAlgorithm::getWaveStartEnd(const int &waveNo) const
{
    return waveToWaveStartEndMap[waveNo];
}

QList<QPointF> DataProgressHandleAlgorithm::parserWholeWaveData(const QList<QVector<QPointF> > &inputData, const QMap<int, QPointF> &waveToWaveStartEndMap)
{
    return {};
}


///TODO:need to fix this fuction to real function
QVector<QPointF> DataProgressHandleAlgorithm::parserWaveData(const QVector<QPointF> &inputData, const QMap<int, QPointF> &waveToWaveStartEndMap, int currentWaveNo)
{
    ///实现之前直接返回
    return inputData;

    QVector<QPointF> ret;

    ///check wave define data
    for(int i = 1 ; i <= currentWaveNo; i++)
    {
        if(!waveToWaveStartEndMap.contains(i))
        {
            qWarning()<<"waveToWaveStartEndMap is invalid";
            return ret;
        }
    }

    ///parser every wave
    int startIndex = 0;
    int endIndex = inputData.size() - 1 ;
    int currentIndex = 0;
    for(int i = 1 ; i <= currentWaveNo; i++)
    {
        QPointF startEndPoint = waveToWaveStartEndMap.value(i);
        qDebug()<<"wave no "<<i <<"start and end is"<< startEndPoint;
        bool findStart = false;

        ///parser start
        for( ; currentIndex < inputData.count(); currentIndex++ )
        {
            if(inputData.at(currentIndex).y() > startEndPoint.x() )
            {
                startIndex = currentIndex;
                findStart = true;
                break;
            }
        }

        if(findStart)
        {
            qDebug()<<"wave no "<<i <<"find start at "<<inputData.at(currentIndex);
            ///parser end
            for( ; currentIndex < inputData.count(); currentIndex++ )
            {
                if(inputData.at(currentIndex).y() < startEndPoint.x() )
                {
                    endIndex = currentIndex;
                    qDebug()<<"wave no "<<i <<"find end at "<<inputData.at(currentIndex);
                    break;
                }
            }

            if(endIndex - startIndex < 2)
                break;

        }
        else
        {
            qDebug()<<"wave no "<<i <<"can't find start";
            break;
        }


    }

    if(startIndex && endIndex && endIndex > startIndex)
    {
        qDebug()<<"wave no "<< currentWaveNo;
        qDebug()<<"start is "<<inputData.at(startIndex);
        qDebug()<<"end is "<<inputData.at(endIndex);
        ret = inputData.mid(startIndex, endIndex - startIndex);
    }

    return ret;
}

QMap<int, QPointF> DataProgressHandleAlgorithm::getWaveToHandleWaveMap() const
{
    return waveToHandleWaveMap;
}

void DataProgressHandleAlgorithm::setWaveToHandleWaveMap(const QMap<int, QPointF> &value)
{
    waveToHandleWaveMap = value;
}
