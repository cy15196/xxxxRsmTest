#ifndef DIALOGCURRENTLINKDEVICES_H
#define DIALOGCURRENTLINKDEVICES_H

#include <QWidget>

#include "dialogbase.h"

namespace Ui {
class DialogCurrentLinkDevices;
}

class DialogCurrentLinkDevices : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogCurrentLinkDevices(QWidget *parent = 0);
    ~DialogCurrentLinkDevices();

    void onButtonClicked(int role);
    int standerButtons();

public slots:
    void refreshDeviceList();

private:
    void init();
    Ui::DialogCurrentLinkDevices *ui;
};

#endif // DIALOGCURRENTLINKDEVICES_H
