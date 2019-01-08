#ifndef FORMTOPTESTGX80_H
#define FORMTOPTESTGX80_H

#include <QDialog>
#include "toptestwidgets_global.h"

namespace Ui {
class FormTopTestGX80;
}

class TopTestUsb;

class TOPTESTWIDGETSSHARED_EXPORT FormTopTestGX80 : public QDialog
{
    Q_OBJECT

public:
    explicit FormTopTestGX80(QWidget *parent = 0);
    ~FormTopTestGX80();

public slots:
    void refreshState();
    void pushSettingToDevice();

private slots:
    void on_pushButton_Clear_clicked();

    void on_pushButton_Write_clicked();


    void onStateStartModeChanged(int mode);
    void onSetStartModeChanged(int mode);

private:
    Ui::FormTopTestGX80 *ui;
    TopTestUsb* usbHandle;
};

#endif // FORMTOPTESTGX80_H
