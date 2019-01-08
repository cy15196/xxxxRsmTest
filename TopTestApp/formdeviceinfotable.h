#ifndef FORMDEVICEINFOTABLE_H
#define FORMDEVICEINFOTABLE_H

#include <QWidget>

namespace Ui {
class FormDeviceInfoTable;
}

class QTableWidget;

class FormDeviceInfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit FormDeviceInfoTable(QWidget *parent = 0);
    ~FormDeviceInfoTable();

    void setPortInfo(const QMap<int,QPair<QString,QColor>> & portInfo);

    void applyDevice(const QString& deviceName);
    void applyDevice(const QMap<QString,QVariant>& deviceMap);

    void test();

    QTableWidget* getTableWidget();

private:

    Ui::FormDeviceInfoTable *ui;
};

#endif // FORMDEVICEINFOTABLE_H
