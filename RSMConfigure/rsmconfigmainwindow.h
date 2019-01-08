#ifndef RSMCONFIGMAINWINDOW_H
#define RSMCONFIGMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class RSMConfigMainWindow;
}

class RSMConfigMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RSMConfigMainWindow(QWidget *parent = 0);
    ~RSMConfigMainWindow();

    void refreshSettingValues(const QList<int>& rows = {0,1,2,3,4,5,6,7});

    void refreshAdjustValue(const QList<int>& rows = {0,1,2,3});

    void init();

    void refreshWhenPortChanged();

    ///重置所有界面数据和连接状态
    void reset();

    void resetLink();
public slots:
    void onAviablePortChanged(const QStringList &portList);
    void onCurrentDeviceError();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private slots:
    void on_toolButton_writeLock_clicked(bool checked);

    void on_toolButtona_activeState_clicked(bool checked);

    void onSettingPushButtonClicked();
    void onAdjustPushBottonClicked();

    void on_pushButton_refresh_clicked();

    void on_checkBox_autoRefresh_clicked(bool checked);

    void refreshPortValues();

    void refreshDeviceList();

    void on_comboBox_deviceChoose_currentTextChanged(const QString &arg1);

//    void on_comboBox_BaudRate_currentTextChanged(const QString &arg1);

    void on_toolButton_wirelessSwitch_clicked(bool checked);

    void onDeviceDataChanged(int dataAddress, const QVariant &value);

    void on_spinBox_rtu_valueChanged(int arg1);

    void on_comboBox_linkType_currentIndexChanged(int index);

    void onWriteLockToggled(bool checked);

    void onCurrentDeviceNoRespond(bool reset);

private:
    Ui::RSMConfigMainWindow *ui;

    QTimer autoRefreshTimer;
    QTimer deviceDetectTimer;

    QList<int > currentSettingRefreshRowList;
};

#endif // RSMCONFIGMAINWINDOW_H
