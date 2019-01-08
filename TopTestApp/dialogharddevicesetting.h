#ifndef DIALOGHARDDEVICESETTING_H
#define DIALOGHARDDEVICESETTING_H

#include <QMap>
#include <QTimer>

#include "dialogbase.h"

namespace Ui {
class DialogHardDeviceSetting;
}

//class QLCDNumber;
class QAbstractButton;
class QLabel;
class QRadioButton;

class DialogHardDeviceSetting : public DialogBase
{
    Q_OBJECT

public:
    explicit DialogHardDeviceSetting(QWidget *parent = 0);
    ~DialogHardDeviceSetting();




public slots:
    void refreshDataInfo();
    void onApply();

    void refreshWhenPortChanged();


protected:
    void showEvent(QShowEvent* event);

    void hideEvent(QHideEvent* event);

private slots:
    void onButtonClicked(int role);
    int standerButtons();


    void on_pushButton_clicked();

    void onEditValueChanged();

    void onRadioButtonChecked(bool checked);


//    void on_comboBox_linkType_currentIndexChanged(const QString &arg1);

//    void refreshDeviceList();

//    void on_comboBox_deviceChoose_currentTextChanged(const QString &arg1);

    void on_checkBox_autoRefresh_clicked(bool checked);

    void on_pushButton_2_clicked();

    void onDeviceDataChanged(int dataAddress,const QVariant& data);

    void onUnitTypeChanged(int unitType);

private:

    void init();

    Ui::DialogHardDeviceSetting *ui;

    QMap<QObject*,QLabel*> editToLabelMap;

    QTimer autoRefreshTimer;
//    QTimer deviceDetectTimer;

    QMap<int,QRadioButton*> enumToRadioButtonMap;

    static DialogBase* linkWidgetTemp;

//    QList<QLCDNumber*> lcdNumberList;

};

#endif // DIALOGHARDDEVICESETTING_H
