#ifndef DEVICESETTING_H
#define DEVICESETTING_H

#include <QObject>

class DeviceSetting : public QObject
{
    Q_OBJECT
public:
    DeviceSetting* instance(const QString& name);

signals:
    void DataChanged(const QString& name, const QVariant& data);

private:
    DeviceSetting(QObject * parent = nullptr);
};

#endif // DEVICESETTING_H
