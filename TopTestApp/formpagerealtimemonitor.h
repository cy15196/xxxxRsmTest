#ifndef FORMPAGEREALTIMEMONITOR_H
#define FORMPAGEREALTIMEMONITOR_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class FormPageRealtimeMonitor;
}

class FormPageRealtimeMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit FormPageRealtimeMonitor(QWidget *parent = 0);
    ~FormPageRealtimeMonitor();

    void showRealLCDValueList(uchar portNum, QList<double> tempValueList);
    void showCurrentLCDValueList(uchar portNum, QList<double> tempValueList);


    void setPortNum(uchar portNum);

    void init();

    int getSendCount() const;
    void setSendCount(int value);

    int getReciveCount() const;
    void setReciveCount(int value);

    void resetTime();


    bool getMonitorOn() const;
    void setMonitorOn(bool value);

    int getSendDataCount() const;
    void setSendDataCount(int value);

    int getReciveDataCount() const;
    void setReciveDataCount(int value);

    bool getRecordStateOn() const;
    void setRecordStateOn(bool value);

signals:
    void sendCountChanged(int);
    void reciveCountChanged(int);

    void dataCountChanged();

    void monitorOnChanged(bool on);
    void recordStateOnChanged(bool on);

private slots:

    void onNeedToShowDailog(QWidget *w, int sig);

    void refresh();

    void onRulerSliderChanged(double time);

    void addTestDataToMonitor();

    void onTimeOut();

    void onCurrentDeviceNoRespond(bool reset);

    void onReset();
    void onSaveFileToPath(const QString &filePath);

    ///on file data return from device
    void onRuntimeFileDataReady(const QByteArray& array);

    void onDeviceDataChanged(int dataAddress,const QVariant& data);

    void onSendReciveChanged();

    void on_pushButton_refresh_clicked();

    void onMonitorChanged(bool on);
    void onRecordOnChanged(bool on);

    void onDataCountChanged();

private:

    void initToolButtonGroup();


    bool monitorOn = false;
    bool recordStateOn = false;
    double recordTime;
    QTimer autoRefresh;
    int portNum;
    double sampleTime;
    QDateTime testDateTime;
    double lineVelocity;
//    bool inited;

    Ui::FormPageRealtimeMonitor *ui;
    void initLink();

    int readCount = 0;
    int sendCount = 0;
    int reciveCount = 0;

    int sendDataCount = 0;
    int reciveDataCount = 0;

    int recordState = 0;

    bool firstRecordStart = true;
};

#endif // FORMPAGEREALTIMEMONITOR_H
