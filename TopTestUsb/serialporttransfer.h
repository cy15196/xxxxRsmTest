#ifndef SERIALPORTTRANSFER_H
#define SERIALPORTTRANSFER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QStack>
#include <QMap>
//#include <QAbstractNativeEventFilter>
#include "toptestenum.h"
#include "toptestusb_global.h"

class QTimer;
class QSerialPort;
class QSerialPortInfo;


class TOPTESTUSBSHARED_EXPORT SerialPortTransfer : public QObject/*,public  QAbstractNativeEventFilter*/
{
    Q_OBJECT
public:
    explicit SerialPortTransfer(TopTest::DeviceType deviceType = TopTest::TopTestGX80,
                                QObject *parent = nullptr);

    ~SerialPortTransfer();

    QByteArray getReciveBuffer() const;


    void setValidRegString(const QString &value);

    bool getIsFinised() const;
    void setIsFinised(bool value);

    static QList<int> getAviableBaudRateList();

    void waitForReadyRead(int msec = 3000);

    void pushCmdToStack(const QByteArray &cmd);

    void writeToPort(const QByteArray &data);

    int getBaudRate() const;

    qint64 getTimeId() const;
    void setTimeId(const qint64 &value);

signals:
    void portDataReady(const QByteArray& data);
    void currentDeviceNoRespond();
    void currentDeviceError();

    void aviablePortChanged(const QStringList& portList);

    void errorOpenPort(int errorCode, const QString& errorStr);

    void refreshPortDone();

    void portOpenSuccess(const QString& port);

public slots:
    void refreshAviablePorts();
    void setActivePort(const QString& portName);
    void setBaudRate(int value);
    void closeCurrentPort();
    void sendToPort(const QByteArray& data);


    void setTestCMD(const QString &value);


    QByteArray readPortData();

    void tryWriteToPort();

    void judgeRespond();

    bool testPort();

//    void waitForReadyWrite();

private slots:
    void onPortReadyRead();
    void onError();

    void onJudgeTimerOut();



//public:
//    bool nativeEventFilter( const QByteArray &eventType, void *message, long *result );

private:
    QSerialPort* serial;
    QByteArray reciveBuffer;

    QStringList reciveStack;

    bool isFinised;
    bool isOpened;

    TopTest::DeviceType deviceType;

    QString testCMD;
    QString validRegString;

    int BaudRate;
    QString activePortName;

    QStack<QByteArray> cmdStack;

    QDateTime lastCmdDataTime;
    QDateTime lastReviceDataTime;

    bool writeHasReturn;

    int64_t sendCount;

    QMap<int64_t, QTime> noRespondMap;

    qint64 timeId = 0;

//    QTimer* judgeRepondTimer;
};

#endif // SERIALPORTTRANSFER_H
