#include "devicesetting.h"
#include <QMap>

QMap<QString,DeviceSetting *> devMap;

DeviceSetting *DeviceSetting::instance(const QString &name)
{
    DeviceSetting* setting = devMap[name];
    if(setting == 0)
    {
        setting = new DeviceSetting;
        devMap[name] = setting;
    }
    return setting;
}

DeviceSetting::DeviceSetting(QObject *parent)
    :QObject(parent)
{

}
