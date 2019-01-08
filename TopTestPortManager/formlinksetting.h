#ifndef FORMLINKSETTING_H
#define FORMLINKSETTING_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class FormLinkSetting;
}

class FormLinkSetting : public QWidget
{
    Q_OBJECT

public:

    ///keep link form with only one
    static FormLinkSetting* instance();

//    static bool judgeLink();

    ~FormLinkSetting();

//    void onButtonClicked(int button);
//    int standerButtons();

    void hideLinkTypeAndRTUAddress();
    void showLinkTypeAndRTUAddress();

    void resetLink();

signals:
    void activePortChanged(const QString& portName);

    void deviceConnected();

protected:

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void on_comboBox_deviceChoose_currentTextChanged(const QString &arg1);
    void refreshDeviceList();
    void on_comboBox_linkType_currentIndexChanged(const QString &arg1);
    void on_spinBox_rtu_valueChanged(int arg1);

    void onCurrentDeviceNoRespond(bool reset);

    void onCurrentDeviceError();

    void onAviablePortChanged(const QStringList& portList);

//    void on_checkBox_wireless_clicked(bool checked);

//    void onDeviceDataReady(int address,const QVariant& value);

private:
    explicit FormLinkSetting(QWidget *parent = 0);

    Ui::FormLinkSetting *ui;
    QTimer deviceDetectTimer;
    void init();

    bool firstShow;
};

#endif // FORMLINKSETTING_H
