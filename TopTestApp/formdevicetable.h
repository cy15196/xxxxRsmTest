#ifndef FORMDEVICETABLE_H
#define FORMDEVICETABLE_H

#include <QTableWidget>
#include <QMap>

class FormDeviceTable: public QTableWidget
{
    Q_OBJECT
public:
    FormDeviceTable(QWidget* parent = 0);
    ~FormDeviceTable();


    QMap<QString,QVariant> saveTableToMap();
    void loadDataToTable(const QMap<QString,QVariant>& data, bool singleSection = false);

    int getAreaCount() const;

public slots:
    void setAreaCount(int value);

private:
    int areaCount; //温区数
};

#endif // FORMDEVICETABLE_H
