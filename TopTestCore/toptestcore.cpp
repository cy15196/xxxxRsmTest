#include "toptestcore.h"
#include <QDir>

static TopTestCore* core = nullptr;

TopTestCore *TopTestCore::instance()
{
    if(core == nullptr)
    {
        core = new TopTestCore;
    }
    return core;
}

void TopTestCore::destoryInstance()
{
    Q_ASSERT_X(core != nullptr,"TopTestCore::destoryInstance","read violation");
    delete core;
}

double TopTestCore::convertToSIValue(const double &value, int type)
{
    double tempValue = value;
    if(type & UnitType::F) //convert F to C
        tempValue = (tempValue - 32) / 1.8;

    if(type & UnitType::cm) //m to cm
    {
        tempValue *= 100;
    }
    else if(type & UnitType::mm) //m to mm
    {
        tempValue *= 1000;
    }
    else if(type & UnitType::inch) //m to inch
    {
        tempValue *= 39.3700787;
    }
    else if(type & UnitType::feet) //m to feet
    {
        tempValue *= 39.3700787;
    }

    if(type & UnitType::min) // /s to /min
        tempValue *= 60.0;

    return tempValue;
}

//double TopTestCore::getCurrentRealValueFromSI(const double &SIValue)
//{
//    return getRealValueFromSI(SIValue,this->unitType);
//}

//double TopTestCore::convertCurrentToSIValue(const double &value)
//{
//    return convertToSIValue(value,this->unitType);
//}

QString TopTestCore::getProgressFileSavePath()
{
    return QDir::homePath()+"/Documents/TopTest/ProgressFile.data";
}

double TopTestCore::getRealValueFromSI(const double &SIValue, int type)
{
    double tempValue = SIValue;
    if(type & UnitType::F) //convert C to F
        tempValue = tempValue * 1.8 +32;

    if(type & UnitType::cm) //m to cm
    {
        tempValue *= 100;
    }
    else if(type & UnitType::mm) //m to mm
    {
        tempValue *= 1000;
    }
    else if(type & UnitType::inch) //m to inch
    {
        tempValue *= 39.3700787;
    }
    else if(type & UnitType::feet) //m to feet
    {
        tempValue *= 39.3700787;
    }

    if(type & UnitType::min) // /s to /min
        tempValue *= 60.0;

    return tempValue;
}

TopTestCore::TopTestCore()
{
    ///get defalut device and progress lists
    unitType = (TopTest::UnitType)(TopTest::C | TopTest::s | TopTest::m);
}

UnitType TopTestCore::getUnitType() const
{
    return unitType;
}

void TopTestCore::setUnitType(const UnitType &value)
{
    if(unitType == value)
        return;
    unitType = value;
    emit this->unitTypeChanged(value);
}

QString TopTestCore::getCurrentProjectPath() const
{
    return currentProjectPath;
}

void TopTestCore::setCurrentProjectPath(const QString &value)
{
    currentProjectPath = value;
}

int TopTestCore::getCurrentLengthUnit()
{
    return unitType & 0xF0;
}

int TopTestCore::getCurrentTimeUnit()
{
    return unitType & 0x0E;
}

int TopTestCore::getCurrentTemputureUnit()
{
    return unitType & 0x01;
}

QString TopTestCore::getCurrentLengthUnitStr()
{
    switch (unitType & 0xF0 ) {
    case TopTest::inch:
        return QCoreApplication::translate("UnitType","inch");
        break;
    case TopTest::feet:
        return QCoreApplication::translate("UnitType","feet");
        break;
    case TopTest::mm:
        return QCoreApplication::translate("UnitType","mm");
        break;
    case TopTest::cm:
        return QCoreApplication::translate("UnitType","cm");
        break;
    case TopTest::m:
        return QCoreApplication::translate("UnitType","m");
        break;
    default:
        return QCoreApplication::translate("UnitType","m");
        break;
    }
    return QCoreApplication::translate("UnitType","m");
}

QString TopTestCore::getCurrentTimeUnitStr()
{
    switch (unitType & 0x0E ) {
    case TopTest::min:
        return QCoreApplication::translate("UnitType","min");
        break;
    case TopTest::s:
        return QCoreApplication::translate("UnitType","s");
        break;
    default:
        return QCoreApplication::translate("UnitType","s");
        break;
    }
    return QCoreApplication::translate("UnitType","s");
}

QString TopTestCore::getCurrentTemputureUnitStr()
{
    switch (unitType & 0x01 ) {
    case TopTest::F:
        return QCoreApplication::translate("UnitType","F");
        break;
    case TopTest::C:
        return QCoreApplication::translate("UnitType","C");
        break;
    default:
        return QCoreApplication::translate("UnitType","C");
        break;
    }
    return QCoreApplication::translate("UnitType","C");
}

//QString TopTestCore::getLanguage() const
//{
//    return language;
//}

//void TopTestCore::setLanguage(const QString &value)
//{
//    language = value;
//}

QStringList TopTestCore::getDefinedDeviceList()
{
    return {"Gx80","RSM II"};
}

QStringList TopTestCore::getDefinedProcessList()
{
    return {"Process 01","Process 02"};
}

QStringList TopTestCore::getDefinedDeviceProcessList()
{
    return {"Device Process 01","Device Process 02"};
}
