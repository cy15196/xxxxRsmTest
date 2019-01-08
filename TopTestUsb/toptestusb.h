#ifndef TOPTESTUSB_H
#define TOPTESTUSB_H

#include "toptestusb_global.h"
#include "toptestenum.h"
#include <QString>
#include <QDate>
#include <QTime>
#include <QObject>

class SerialPortTransfer;

class TOPTESTUSBSHARED_EXPORT TopTestUsb:public QObject
{
    Q_OBJECT
public:

    static TopTestUsb* instance();

    TopTestUsb();
    ~TopTestUsb();

    QStringList getAviablePortList();
    void setActivePortName(const QString& portName);

    //0x00 读取机器码
    QString ReadDeviceID();

    //0x01 读取/设置采样频率
    double ReadSampleTime(int rowNo = 0x04);
    void SetSampleTime(double deltaTime,int rowNo = 0x04);

    //0x02 读取组数据采样长度 0-3 ？？？？？返回值Data1 和 Data2 么？
    int ReadRowSampleLength(const int& rowNum);

    //0x03 读取采样数据 读取对应组别的采样数据
    QMap<int,QList<double>> ReadRowSampleData(const int& rowNum);

    //0x04 清空机器数据
    void ClearDeviceData();

    //0x05 读取/设置 年月日
    QDate ReadDeviceDate();
    void SetDeviceDate(const QDate& date);

    //0x06 读取/设置 时分秒
    QTime ReadDeviceTime();
    void SetDeviceTime(const QTime& time);

    //0x07 读取各通道当前的温度
    QList<double> ReadPortCurrentValue();

    //0x08 读取机器内部温度
    double ReadDeviceInternalValue();

    //0x09 设置/读取启动方式
    TopTest::TopTestStartMode ReadDeviceStartMode();
    void SetDeviceStartMode(TopTest::TopTestStartMode mode);

    //0x0a 设置/读取延时启动的时间
    void SetDeviceDelayStartTime(int delayTimeSec);
    int ReadDeviceDelayStartTime();

    //0x0b 设置/读取启动温度
    void SetDeviceDelayStartTemputure(int value);
    double ReadDeviceDelayStartTemputure();

    //0x0c 读取测试开始 年月日
    QDate ReadTestStartDate();
    //0x0d 读取测试开始 时分秒
    QTime ReadTestStartTime();

    // 限制通道数
    void SetActivePortNum(const int & portNum);

    //读0位
    double ReadZeroValue();

    //机器复位
    void ResetDevice();

    //限制最高温度
    void SetMaxValue(const double& value);

    //获取Usb的操作句柄
    SerialPortTransfer *getPortHandle() const;
    void setPortHandle(SerialPortTransfer *value);

signals:
    void dataReady();
    void currentDeviceNoRespond(bool needReset);


private:

    QList<uint> sendCMD(TopTest::TopTestCMD cmdCode, uint* data);

    SerialPortTransfer * portHandle;

    int portNum;

};

#endif // TOPTESTUSB_H
