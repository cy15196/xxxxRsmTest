#ifndef DIALOGDEVICEHANDLE_H
#define DIALOGDEVICEHANDLE_H

#include <QDialog>
#include "toptestwidgets_global.h"

namespace Ui {
class DialogDeviceHandle;
}

class TOPTESTWIDGETSSHARED_EXPORT DialogDeviceHandle : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeviceHandle(QWidget *parent = 0);
    ~DialogDeviceHandle();

private:
    Ui::DialogDeviceHandle *ui;
};

#endif // DIALOGDEVICEHANDLE_H
