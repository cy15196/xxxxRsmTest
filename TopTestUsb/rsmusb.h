#ifndef RSMUsb_H
#define RSMUsb_H

#include "toptestusb_global.h"
#include "toptestenum.h"
#include "topteststructdata.h"
#include "toptestcore.h"
#include <QString>
#include <QObject>
#include <QDateTime>
#include <QStack>

class QRemoteObjectNode;

struct PortValueItemInfo
{
    PortValueItemInfo()
    {

    }

    PortValueItemInfo(int dataLength,
                      int dataType,
                      double scale,
                      bool writiable)
    {
        this->dataLength = dataLength;
        this->dataType = dataType;
        this->scale = scale;
        this->writiable = writiable;
    }
//    int dataAdress; ///数据地址
    int dataLength = 1; ///位数，1位代表一个int16_t
    int dataType = QVariant::Int ;   ///数据类型
    double scale = 1.0;   ///如果数据为double,其需要进行的变换 用读取到的值乘以 scale
                    ///得到真实意义的值
    bool writiable = true; ///该数据项是否可写
};

#define RsmCMDReadDeclare(cmdAddress, classType)\
   classType  read##cmdAddress();

#define RsmCMDReadWriteDeclare(classType)\
   classType  read##cmdAddress();\
   void  write##cmdAddress(const classType& value);


class SerialPortTransfer;
class MyThread;
class DirectConnect;

class TOPTESTUSBSHARED_EXPORT RSMUsb:public QObject
{
    Q_OBJECT
public:

    enum PortState{
        NoValid,
        ResetPort,
        RefreshPort,
        SetActivePort,
        WaitForWork,
        Working
    };

    static RSMUsb* instance();

    static QMap<int,QList<double>> parserFileDataToPortMap(
            const int &portNum,
            const QByteArray& array);

    static QList<QList<double> > parserFileDataToRecordList(
            const int &portNum,
            const QByteArray& array);


    static uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);


    ~RSMUsb();

    void initPrivite();

    //获取可用端口列表
    QStringList getAviablePortList();

    void refreshAviablePortList();


    //读取数据项
    void readDeviceValueItem(int address);
    void writeDeviceValueItem(int address,
                              const QVariant& value,
                              int stateRefreshWaitMsecs = 300);


    //读取记录列表，返回数据记录
    QString readFileRecordListInfo();
    QString readFileRecordListInfoNew();

    //删除前几条记录 ，默认为一条
    void removeFileRecordHeadOnes(const int& num = 1);

    //清空数据记录
    void clearFileRecord();

    //读取文件数据内容
    QByteArray readFileContent(const QString& fileName,int dataLength);

    //读取实时文件数据
    QByteArray readRealtimeFileContent(int dataLength);
    void  readRealtimeFileContentNew(int dataLength);
    void  readFileContentNew(const QString& fileName,int dataLength);



    bool getIsFinished() const;
    void setIsFinished(bool value);

    void waitForReadyWrite(int msec = 3000);

//    void waitForReadyRead(int msec = 3000);

    QString getActivePortName() const;

    /********************综合命令 **************************************/

    void refreshAdjustData();
    void refreshUserData();
    void refreshConfigureData();
    QVariant getDeviceMapValue(TopTest::RsmCMD cmdCode);

    QList<double> getPortTempValues();
    QList<double> getPortVolValues();
    QList<int> getPortADValues();
    QList<int> getPortWholeAdjustValues();
    QList<int> getPortZeroAdjustValues();
//    void refreshPortData();

    void waitForMSecs(int svalue);


    uchar getDeviceAddress() const;
    void setDeviceAddress(const uchar &value);

    void closeCurrentPort();

    void clearDeviceData();

    ///lower cost when just refresh one data
    void refreshSpecialData(int dataAddress, int dataLength);

    bool getCanReadFile() const;
    void setCanReadFile(bool value);

    int getBaudRate() const;
    void setBaudRate(int value);

    void tryWriteToPort(const QByteArray& data);

    PortState getPortState() const;
    void setPortState(const PortState &value);

    qint64 getTimeId() const;
    void setTimeId(const qint64 &value);

public slots:
    //设置活动、工作端口
    void setActivePortName(const QString& portName);

    void onPortDataReady(QByteArray buffer);

    void stopWaitForReadyWrite();

    void onSerialPortAviablePortListChanged(const QStringList &portList);

    void onDeviceNoRespond();

    void onErrorOpenPort(const int errorCode, const QString& errorString);

    void onDeviceError();

    void onPortOpenSuccess(const QString& port);

    void onPortRefreshDone();

signals:

    /// deal with serial port
    void needToSetBaudRate(int rate);

    void needToWriteToPort(const QByteArray& data);

    void needToCloseCurrentPort();

    void needToRefreshAviablePorts();

    void needToOpenPort(const QString& portName);


    ///emit to ui
    void activePortChanged(const QString& portName);

    void deviceDataChanged(int dataAddress,const QVariant& value = QVariant());

    void aviablePortChanged(const QStringList& portList);

    void currentDeviceNoRespond(bool needReset);

    void currentDeviceError();

    void baudRateChanged(int rate);

    void fileContentReady(const QByteArray& fileData);

    void dataNotValid();

    void needUpdateReadFileProgress(const int& progress);

    void testCmdChanged(const QString& testCmd);

    void refreshPortDone();

//    void userDataChanged(const TopTest::SerialUserData& userData);
//    void configureDataChanged(const TopTest::SerialConfigureData& configData);
private:

    RSMUsb();

    void sendReadCMD(uchar deviceAddress,
                               ushort dataAddress,
                               ushort dataLength);

    void sendWriteSingleCMD(uchar deviceAddress ,
                            ushort dataAddress,
                            int16_t data);


    void sendWriteMultiCMD(uchar deviceAddress,
                           ushort dataAddress,
                           const QList<int16_t> & writeDataList);


    void sendFileCMD(uchar deviceAddress,
                     int16_t dataLength,
                     char* data);

    ///new method CMD
    void sendReadCMDNew(uchar deviceAddress,
                        ushort dataAddress,
                        ushort dataLength);

    void updateCurrentSerialData(int dataAddress);

    void resetLink();

    void pushCommandToPort();

    TopTest::SerialData * activeSerialData;

    int portNum;

    uchar deviceAddress;

    int baudRate;

    QString activePortName;

    QByteArray currentTestCmd;

    bool isFinished;

    bool isReadFile;
    bool isInWait;

    QByteArray portReturnBuffer;

    int sendAddress;
    uchar returnAddress;

    uchar sendCMDCode;
    uchar returnCMDCode;

    uchar returnDataLength;
    ushort readFilereturnDataLength;

    QByteArray returnData;
    int16_t returnCRC;

    bool canReadFile;

    QThread * portThread;
    SerialPortTransfer* portHandle;

    QMap<int,PortValueItemInfo> portValueItemMap;


    QStringList aviablePortList;

    QStack<QPair<qint64,QByteArray>> commandStack;

    DirectConnect* dc = nullptr;

    QRemoteObjectNode* repNode;

    PortState portState = NoValid;

    qint64 timeId = 0;
};

#endif // RSMUsb_H
