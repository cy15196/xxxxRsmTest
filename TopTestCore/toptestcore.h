#ifndef TOPTESTCORE_H
#define TOPTESTCORE_H

#include "toptestcore_global.h"
#include "toptestenum.h"
#include <QStringList>
#include <QObject>

using namespace TopTest;



class TOPTESTCORESHARED_EXPORT TopTestCore:public QObject
{

    Q_OBJECT
public:

    static TopTestCore* instance();
    static void destoryInstance();

    ///deal with dog
    int convertToDogNumber(const int& origin);
    int convertFromDogNumber(const int& origin);

    ///deal with unit
    double getRealValueFromSI(const double& SIValue, int type);
    double convertToSIValue(const double& value, int type);

//    double getCurrentTempRealValueFromSI(const double& SIValue);
//    double getCurrentLengthRealValueFromSI(const double& SIValue);
//    double getCurrentTimeRealValueFromSI(const double& SIValue);
//    double convertCurrentToSIValue(const double& value);

    UnitType getUnitType() const;
    void setUnitType(const UnitType &value);

    QString getProgressFileSavePath();

//    QString getLanguage() const;
//    void setLanguage(const QString &value);
    inline const char* getDataFileSuffix()
    {
        return ".tpd";
    }
    inline const char* getDataFileFilter()
    {
        return "TopTest Data File (*.tpd)";
    }

    inline const char* getDataFileVersion()
    {
        return "1.0";
    }

    QStringList getDefinedDeviceList();
    QStringList getDefinedProcessList();
    QStringList getDefinedDeviceProcessList();

    QString getCurrentProjectPath() const;
    void setCurrentProjectPath(const QString &value);

    int getCurrentLengthUnit();
    int getCurrentTimeUnit();
    int getCurrentTemputureUnit();

    QString getCurrentLengthUnitStr();
    QString getCurrentTimeUnitStr();
    QString getCurrentTemputureUnitStr();

signals:
    void unitTypeChanged(int type);

private:
    TopTestCore();

    UnitType unitType;

    QString currentProjectPath;
};

#endif // TOPTESTCORE_H
