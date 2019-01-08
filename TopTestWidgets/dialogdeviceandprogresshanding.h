#ifndef DIALOGDEVICEANDPROGRESSHANDING_H
#define DIALOGDEVICEANDPROGRESSHANDING_H

#include "toptestwidgets_global.h"
#include <QDialog>

namespace Ui {
class DialogDeviceAndProgressHanding;
}

class TOPTESTWIDGETSSHARED_EXPORT DialogDeviceAndProgressHanding : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeviceAndProgressHanding(QWidget *parent = 0);
    ~DialogDeviceAndProgressHanding();

private slots:
    void on_toolButton_DeviceSetting_clicked();

    void on_toolButton_ProgressSetting_clicked();

private:
    Ui::DialogDeviceAndProgressHanding *ui;
};

#endif // DIALOGDEVICEANDPROGRESSHANDING_H
