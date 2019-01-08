#include <QCoreApplication>
#include <QString>

#include "toptestenum.h"

class ForTranslate
{
public:
    ForTranslate();
};

ForTranslate::ForTranslate()
{
//    FT_stringEnum(ProgressType,
//        MaxTemputure,
//        IncreaseTime,
//        AboveTime,
//        DecreaseTime,
//        IncreaseRatio,
//        DecreaseRatio,
//        MaxIncreaseRatio,
//        MaxDecreaseRatio,
//        AverageRatio,
//        DistanceToMaxTimeSection,
//        TemputureBetweenTime,
//        MaxIncreaseTime,
//        MaxDecreaseTime,
//        MaxPortMaxDeltaValue,
//        TestValuedTime
//    )
    QCoreApplication::translate("ProgressType","MaxTemputure");
    QCoreApplication::translate("ProgressType","IncreaseTime");
    QCoreApplication::translate("ProgressType","AboveTime");
    QCoreApplication::translate("ProgressType","DecreaseTime");
    QCoreApplication::translate("ProgressType","IncreaseRatio");

    QCoreApplication::translate("ProgressType","DecreaseRatio");
    QCoreApplication::translate("ProgressType","MaxIncreaseRatio");
    QCoreApplication::translate("ProgressType","MaxDecreaseRatio");
    QCoreApplication::translate("ProgressType","AverageRatio");
    QCoreApplication::translate("ProgressType","DistanceToMaxTimeSection");

    QCoreApplication::translate("ProgressType","TemputureBetweenTime");
    QCoreApplication::translate("ProgressType","MaxIncreaseTime");
    QCoreApplication::translate("ProgressType","MaxDecreaseTime");
    QCoreApplication::translate("ProgressType","MaxPortMaxDeltaValue");
    QCoreApplication::translate("ProgressType","TestValuedTime");

//    FT_stringEnum(TopTestStartMode,
//        Direct,
//        DelayByTime,
//        DelayByTemputure
//    )
    QCoreApplication::translate("TopTestStartMode","Direct");
    QCoreApplication::translate("TopTestStartMode","DelayByTime");
    QCoreApplication::translate("TopTestStartMode","DelayByTemputure");


}
