#ifndef DIALOGFILESETTINGCHOOSE_H
#define DIALOGFILESETTINGCHOOSE_H

#include <QDialog>
#include <QMap>

#include "dialogbase.h"

namespace Ui {
class DialogFileSettingChoose;
}

class QAbstractButton;
class QComboBox;

class DialogFileSettingChoose : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogFileSettingChoose(QWidget *parent = 0);
    ~DialogFileSettingChoose();

    double getVelocity() const;
    void setVelocity(double value);

    void setCurrentProcess(const QString& processName);
    void setCurrentDevice(const QString& deviceName);
    void setCurrentProduct(const QString& productName);

    int standerButtons();

signals:
    void productChooseChanged(const QString& productName);
    void deviceChooseChanged(const QString& deviceName);
    void progressChooseChanged(const QString& progressName);

    void velocityChanged(double velocity);

private slots:
    void onButtonClicked(int role);

    void on_comboBox_Product_currentIndexChanged(int index);

    void on_comboBox_Device_currentIndexChanged(int index);

    void on_comboBox_Progress_currentIndexChanged(int index);

    void on_doubleSpinBox_velocity_valueChanged(double arg1);

private:

    double velocity;

    ///read all system saved record and set each to currentChoose one
    void init();

    Ui::DialogFileSettingChoose *ui;

    QMap<QString,QComboBox*> typeToComboBoxEditMap;
};

#endif // DIALOGFILESETTINGCHOOSE_H
