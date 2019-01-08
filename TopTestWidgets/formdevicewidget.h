#ifndef FORMDEVICEWIDGET_H
#define FORMDEVICEWIDGET_H

#include <QWidget>
#include <QMap>
#include "toptestwidgets_global.h"

namespace Ui {
class FormDeviceWidget;
}

class QListWidgetItem;

class TOPTESTWIDGETSSHARED_EXPORT FormDeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormDeviceWidget(QWidget *parent = 0);
    ~FormDeviceWidget();

    void insertDevice(const QString& tag, QWidget* deviceWidget);

private slots:
    void on_listWidget_device_itemDoubleClicked(QListWidgetItem *item);

private:

    void init();

    Ui::FormDeviceWidget *ui;

    QMap<QString, QWidget*> deviceMap;
};

#endif // FORMDEVICEWIDGET_H
