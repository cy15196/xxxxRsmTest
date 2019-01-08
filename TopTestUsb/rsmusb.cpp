#include "RSMUsb.h"
#include <Windows.h>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
//#include "ftd2xx.h"
#include <QDataStream>
#include <QThread>
#include <QtWidgets/QMessageBox>
#include "serialporttransfer.h"
#include "mythread.h"
#include <QSerialPort>

#include "directconnet.h"


#define TopTest_Read 0x52
#define TopTest_Write 0x58

#define TopTest_Send_Begin 0xAA
#define TopTest_Send_End 0x55

#define TopTest_Responde_Begin 0x55
#define TopTest_Responde_End 0xAA
#define TopTest_Empty 0x00


/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

///TODO: 将读取数据的操作添加到新线程中，采用信号发射状态量

//QMap<> parserReciveData(uint buffer[])
//{

//}

RSMUsb * usb = 0;

#define SetToInternal(x,min,max)\
     if (x < min)\
        x = min;\
     else if(x>max)\
        x = max;

inline double parserTemputureData(uint data1, uint data2)
{
    uint data = 0.1* (data1* 0x100 + data2) ;
    /// 最高位为符号位，取1表示负数
    double value  = data & 0x7FFF;
    if(data & 0x8000)
        value *= -1;

    SetToInternal(value,-40,1370)

    return value;
}

template <class T>
inline double parserDigtalData(const T& digtaldata, double scaleRatio = 1.00)
{
    double value = scaleRatio * digtaldata ;
//    qDebug()<<"origin data"<<QString::number(digtaldata,16)<<"parser value"<<value;

    return value;
}

inline QByteArray hexToString(uchar* numArray, int length = 8)
{
    QByteArray ret;
    for(int i=0; i<length; i++)
    {
        QString temp = QString::number(numArray[i],16);
        if(temp.length()==1)
            temp = "0"+temp;
        ret += temp.toUtf8();
    }

    qDebug()<<ret;

    return QByteArray::fromHex(ret);
}

inline bool stringToHex(const QByteArray& str, uchar*& buffer, int& length)
{
    qDebug()<<"stringToHex"<<str.toHex();
    if(str.isEmpty())
        return false;
    QDataStream ds(str);
    length = 0;
    QList<uchar> list;
    while(!ds.atEnd())
    {
        uchar temp =0;
        ds>>temp;
        list<<temp;
    }

    length = list.size();
    buffer  = new uchar[length];

    for(int i =0 ; i <length; i++ )
    {
        buffer[i] = list.at(i);
    }

    return true;
}

inline QList<int16_t>  getDataFromHexBuffer(QByteArray data)
{
    QByteArray temp = data/*data.mid(6,data.size()-10)*/;

    bool ok;
    int length = temp.size();

    QList<int16_t> ret;

    while(length>0)
    {
        temp = temp.right(length);
        QString signleDataStr = QString(temp.left(4));
        int16_t value = signleDataStr.toInt(&ok,16);
//        qDebug()<<signleDataStr<<"to value"<<value;
        ret.push_back(value);
        length -= 4 ;
    }

    return ret;
}




void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{
    int i;
    unsigned char tmp[4];
    tmp[0] = 0;
    for(i=0;i< 8;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(7-i);
    }
    dBuf[0] = tmp[0];

}
void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)
{
    int i;
    unsigned short tmp[4];
    tmp[0] = 0;
    for(i=0;i< 16;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(15 - i);
    }
    dBuf[0] = tmp[0];
}



unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;

  while (usDataLen--)
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin) ;
}

inline bool CheckReciveData(const QByteArray& buffer)
{
    //get Crc for data before last 16 bit
    uchar* reciveBuffer;
    int reciveLength;

    bool ok;
    ok = stringToHex(buffer,reciveBuffer,reciveLength);
    if(!ok)
    {
        qDebug()<<"can't convert to hex";
        return false;
    }

    ushort crcCode = CRC16_MODBUS(reciveBuffer,reciveLength-2);
    uchar h =  crcCode & 0x00FF;
    uchar l = (crcCode & 0xFF00) / 0x100;

    qDebug()<<"crc h and l is "<< h << l;
    qDebug()<<"recive h and l  is"<<reciveBuffer[reciveLength-2]<<reciveBuffer[reciveLength-1];

    if( h != reciveBuffer[reciveLength-2] ||
        l != reciveBuffer[reciveLength-1])
    {
        return false;
    }

    return true;
}

inline bool CheckReciveData(char * buffer, int length)
{
    return true;
}


uint16_t RSMUsb::crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}

RSMUsb *RSMUsb::instance()
{
    if(!usb)
    {
        usb = new RSMUsb;
    }
    return usb;
}

RSMUsb::RSMUsb()
{
    initPrivite();

    timeId = QDateTime::currentDateTime().toMSecsSinceEpoch();

    portThread = new QThread;
    portHandle = new SerialPortTransfer;

    this->setBaudRate(500000);
    this->setDeviceAddress(255);
    portHandle->setTestCMD(this->currentTestCmd);
    portHandle->setBaudRate(this->baudRate);
    portHandle->setValidRegString("");

    portHandle->moveToThread(portThread);
    portThread->start();

    ///send to portHandle
    connect(this,SIGNAL(needToSetBaudRate(int)),
            portHandle,SLOT(setBaudRate(int)));

    connect(this,SIGNAL(needToWriteToPort(QByteArray)),
            portHandle,SLOT(sendToPort(QByteArray)));

    connect(this,SIGNAL(needToOpenPort(QString)),
            portHandle,SLOT(setActivePort(QString)));

    connect(this,SIGNAL(needToCloseCurrentPort()),
            portHandle,SLOT(closeCurrentPort()));

    connect(this,SIGNAL(needToRefreshAviablePorts()),
            portHandle,SLOT(refreshAviablePorts()));

    connect(this,SIGNAL(testCmdChanged(QString)),
            portHandle,SLOT(setTestCMD(QString)));

    ///recive from portHandle
    connect(portHandle,SIGNAL(portDataReady(QByteArray)),
            this,SLOT(onPortDataReady(QByteArray)));

    connect(portHandle,SIGNAL(currentDeviceNoRespond()),
            this,SLOT(onDeviceNoRespond()));

    connect(portHandle,SIGNAL(currentDeviceError()),
            this,SLOT(onDeviceError()));

    connect(portHandle,SIGNAL(errorOpenPort(int,QString)),
            this,SLOT(onErrorOpenPort(int,QString)));

    connect(portHandle,SIGNAL(aviablePortChanged(QStringList)),
            this,SLOT(onSerialPortAviablePortListChanged(QStringList)));

    connect(portHandle,SIGNAL(refreshPortDone()),
            this,SIGNAL(refreshPortDone()));

    connect(portHandle,SIGNAL(portOpenSuccess(QString)),
             this,SLOT(onPortOpenSuccess(QString)));
}

RSMUsb::~RSMUsb()
{

    delete activeSerialData;
    delete repNode;

    portThread->terminate();
    delete portThread;
}

void RSMUsb::initPrivite()
{
    isFinished = true;
    isInWait = false;
    portNum = 16;

    deviceAddress = 255;
    canReadFile = true;
    isReadFile = false;
    activeSerialData = new TopTest::SerialData;

//    struct PortValueItemInfo
//    {
//    //    int dataAdress; ///数据地址
//        int dataLength = 1; ///位数，1位代表一个int16_t
//        int dataType = QVariant::Int ;   ///数据类型
//        int eachSpan = 1;                ///每个数据的长度
//        double scale = 1.0;   ///如果数据为double,其需要进行的变换 用读取到的值乘以 scale
//                        ///得到真实意义的值
//        bool writiable = true; ///该数据项是否可写
//    };

    portValueItemMap[TopTest::RsmCMD_FirstTempData]
            = { 16, QVariant::Double,  0.1, false };
    portValueItemMap[TopTest::RsmCMD_FirstVolageData]
            = { 16, QVariant::Double,  0.01, false };

    portValueItemMap[TopTest::RsmCMD_CurrentScaleLevel]
            = { 1, QVariant::Int, 1,  false };
    portValueItemMap[TopTest::RsmCMD_InnerTemp]
            = { 1, QVariant::Double,  0.1, false };
    portValueItemMap[TopTest::RsmCMD_BatteryVolage]
            = { 1, QVariant::Double,  0.01, false };
    portValueItemMap[TopTest::RsmCMD_ErrorCode]
            = { 1, QVariant::Int,  1, false };

    portValueItemMap[TopTest::RsmCMD_RecordState]
            = { 1, QVariant::Int,  1, false };

    portValueItemMap[TopTest::RsmCMD_SoftwareVersion]
            = { 1, QVariant::Int,  1, false };

    portValueItemMap[TopTest::RsmCMD_SaveCMDCode_Begin]
            = { 12, QVariant::Int,  1, false };

    portValueItemMap[TopTest::RsmCMD_Device_DateTime]
            = { 6, QVariant::DateTime,  1, true };

    portValueItemMap[TopTest::RsmCMD_Port_Number]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmCMD_RTU_Address]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmCMD_Sample_Time_Interal]
            = { 1, QVariant::Double,  0.05, true };

    ///TODO:configure scalt of time
    portValueItemMap[TopTest::RsmCMD_Start_Mode]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmCMD_PressStart_DelayTime]
            = { 1, QVariant::Double,  0.05, true };
    portValueItemMap[TopTest::RsmCMD_FirstTempStart_Start_Temp]
            = { 1, QVariant::Double,  0.1, true };
    portValueItemMap[TopTest::RsmCMD_FirstTempStart_Stop_Temp]
            = { 1, QVariant::Double,  0.1, true };
    portValueItemMap[TopTest::RsmCMD_FirstTempStart_Stop_DelayTime]
            = { 1, QVariant::Double,  0.05, true };

    portValueItemMap[TopTest::RsmCMD_StopMode]
            = { 1, QVariant::Int,  1, true };

    portValueItemMap[TopTest::RsmCMD_PortModel]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmCMD_WirelessSwitch]
            = { 1, QVariant::Bool,  1, true };

    portValueItemMap[TopTest::RsmConfig_ConfigPassword]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmConfig_BatteryType]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmConfig_PortNumber]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmConfig_AdjustCMD]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmConfig_ActiveDevice]
            = { 1, QVariant::Int,  1, true };
    portValueItemMap[TopTest::RsmConfig_DeviceCode]
            = { 1, QVariant::Int,  1, false };


    portValueItemMap[TopTest::RsmConfig_LowScale_ZeroAdjust_Begin]
            = { 16, QVariant::Int,  1, false };
    portValueItemMap[TopTest::RsmConfig_LowScale_WholeAdjust_Begin]
            = { 16, QVariant::Int,  1, false };


    portValueItemMap[TopTest::RsmConfig_AD_Data_Begin]
            = { 16, QVariant::Int,  1, false };

    portValueItemMap[TopTest::RsmConfig_MaxValid_Temp]
            = { 1, QVariant::Double,  0.1, true };
    portValueItemMap[TopTest::RsmConfig_CoolEnd_Adjust_TempValue]
            = { 1, QVariant::Double,  0.1, true };

    portValueItemMap[TopTest::RsmConfig_User_Data_ProductNo]
            = { 10, QVariant::String,  1, true };

    portValueItemMap[TopTest::RsmConfig_User_Data_ProduceDateTimeBegin]
            = { 6, QVariant::DateTime,  1, true };
    portValueItemMap[TopTest::RsmConfig_User_Data_DeviceModel]
            = { 10, QVariant::String,  1, true };

    baudRate = 5000000;


    ///init remote object node
    repNode = new QRemoteObjectNode;
    qDebug()<<"connectToRemoteNode"<<repNode->connectToNode(QUrl(QStringLiteral("local:replica")));

    QSharedPointer<PortReplica> ptr;
    ptr.reset(repNode->acquire<PortReplica>());

    this->dc = new DirectConnect(ptr);

}

void RSMUsb::waitForMSecs(int svalue)
{
    isInWait = true;
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    isInWait = false;
}

QStringList RSMUsb::getAviablePortList()
{
    return aviablePortList;
}

void RSMUsb::refreshAviablePortList()
{
    if(!portThread->isRunning())
        portThread->start();
    this->setPortState(RefreshPort);
    emit this->needToRefreshAviablePorts();
}

void RSMUsb::readDeviceValueItem(int address)
{
    if(!portValueItemMap.contains(address))
    {
        qDebug()<<"error: try to write unknown address";
        return;
    }

    PortValueItemInfo info = portValueItemMap[address];
    sendReadCMDNew(deviceAddress,address,info.dataLength);
}

void RSMUsb::writeDeviceValueItem(int address,
                                  const QVariant &value,
                                  int stateRefreshWaitMsecs)
{
    if(!portValueItemMap.contains(address))
    {
        qDebug()<<"error: try to write unknown address";
        return;
    }

    PortValueItemInfo info = portValueItemMap[address];
    if(!info.writiable)
    {
        qDebug()<<"error: adress is not writiable";
        return;
    }

    qDebug()<<"now real write data to device"<<deviceAddress<<address<<value;

    switch (info.dataType) {
    case QVariant::Int:
    {
        if(info.dataLength != 1)
        {
            qDebug()<<"error: dataLength is not right";
            return;
        }

        int16_t data = value.toInt();
        sendWriteSingleCMD(deviceAddress,address,data);
    }
        break;
    case QVariant::Double:
    {
        if(info.dataLength != 1)
        {
            qDebug()<<"error: dataLength is not right";
            return;
        }

        qDebug()<<"write double value with scale"<<info.scale;

        int16_t data = value.toDouble() / info.scale;
        sendWriteSingleCMD(deviceAddress,address,data);
    }
        break;
    case QVariant::String:
    {
        QString writeStr = value.toString();
        while(writeStr.length()<info.dataLength)
        {
            writeStr+=" ";
        }

        QList<int16_t> dataList;
        for(int i=0; i< 10 ; i++)
        {
            int16_t value = writeStr.at(i).toLatin1();
            dataList<<value;
        }
        sendWriteMultiCMD(deviceAddress,address,dataList);
    }
        break;
    case QVariant::Bool:
    {
        if(info.dataLength != 1)
        {
            qDebug()<<"error: dataLength is not right";
            return;
        }

        int16_t data = value.toBool() ? 1 : 0;
        sendWriteSingleCMD(deviceAddress,address,data);
    }
        break;
    case QVariant::DateTime:
    {
        QDateTime dateTime = value.toDateTime();
        QDate date = dateTime.date();
        QTime time = dateTime.time();
        QList<int16_t> dataList;
        dataList<<date.year()<<date.month()<<date.day()
               <<time.hour()<<time.minute()<<time.second();
        sendWriteMultiCMD(deviceAddress,address,dataList);
    }
        break;
    default:
    {
        if(!info.writiable)
        {
            qDebug()<<"error: write not handled in type"<<info.dataType;
            return;
        }
    }
        break;
    }

    if(stateRefreshWaitMsecs > 0)
    {
        this->waitForMSecs(stateRefreshWaitMsecs);
        RSMUsb::instance()->readDeviceValueItem(address);
    }
}

void RSMUsb::setActivePortName(const QString &portName)
{
    if(activePortName == portName)
        return;
    activePortName = portName;
    if(!portName.isEmpty())
    {
        this->setPortState(SetActivePort);
        qDebug()<<"try open port"<<portName;
        emit this->needToOpenPort(portName);
    }
}

void RSMUsb::onPortDataReady(QByteArray buffer)
{
    qDebug()<<"recive data"<<buffer.toHex()<<buffer.length();
    portReturnBuffer += buffer;
    qDebug()<<"current buffer"<<portReturnBuffer.toHex();
    if(returnCMDCode != sendCMDCode)
    {
        if(isReadFile)
        {
            if(portReturnBuffer.length() >  (1+1+2))
            {
                QDataStream ds(portReturnBuffer);
                ds>>returnAddress>>returnCMDCode>>readFilereturnDataLength;
//                bool ok;
//                QByteArray lengthArray = portReturnBuffer.mid(2,2).toHex();
//                qDebug()<<lengthArray;
//                readFilereturnDataLength = QString(lengthArray).toInt(&ok,16);
                qDebug()<<"fill address return cmdCode and datalength"
                        <<returnAddress<<returnCMDCode<<readFilereturnDataLength;

            }
        }
        else
        {
            if(portReturnBuffer.length() >  (1+1+1))
            {
                QDataStream ds(portReturnBuffer);
                ds>>returnAddress>>returnCMDCode>>returnDataLength;
                qDebug()<<"fill address return cmdCode and datalength"
                        <<returnAddress<<returnCMDCode<<returnDataLength;
            }
        }
    }

    if( returnCMDCode == sendCMDCode  )
    {
        if(isReadFile)
        {
            if( portReturnBuffer.length() == (4 + readFilereturnDataLength + 2) )
            {

                returnData = portReturnBuffer.mid(4,readFilereturnDataLength);
                QDataStream ds(portReturnBuffer.right(2));
                ds>>returnCRC;
//                qDebug()<<"file recive end"<<returnData.toHex();
                bool dataValid = CheckReciveData(portReturnBuffer);
                qDebug()<<"check data"<<dataValid;
                if(dataValid)
                    emit this->fileContentReady(returnData);
                else
                    emit this->dataNotValid();
                isFinished = true;
                portReturnBuffer.clear();
//                returnData.clear();
            }
        }
        else
        {
            if(sendCMDCode == 0x03)
            {
                if( portReturnBuffer.length() == (3 + returnDataLength + 2) )
                {

                    returnData = portReturnBuffer.mid(3,returnDataLength);
                    qDebug()<<"read data recive end"<<returnData.toHex();
                    QDataStream ds(portReturnBuffer.right(2));
                    ds>>returnCRC;
                    bool dataValid = CheckReciveData(portReturnBuffer);
                    qDebug()<<"check data"<<dataValid;
                    if(dataValid)
                        updateCurrentSerialData(sendAddress);
                    else
                        emit this->dataNotValid();
                    isFinished = true;
                    portReturnBuffer.clear();
//                    returnData.clear();
                }
            }
            else
            {
                if( portReturnBuffer.length() == (2+2+2+2) )
                {
                    qDebug()<<"write data recive end"<<returnData.toHex();
                    qDebug()<<"check data"<<CheckReciveData(portReturnBuffer);
                    //returnData = portReturnBuffer.mid(3,readFilereturnDataLength);
                    QDataStream ds(portReturnBuffer.right(2));
                    ds>>returnCRC;
                    bool dataValid = CheckReciveData(portReturnBuffer);
                    qDebug()<<"check data"<<dataValid;
                    if(dataValid)
                    {
//                        updateCurrentSerialData(sendAddress);
                    }
                    else
                    {
                        emit this->dataNotValid();
                    }
                    isFinished = true;

//                    returnData.clear();
                    isFinished = true;
                    portReturnBuffer.clear();
                }
            }
        }
    }
}

void RSMUsb::stopWaitForReadyWrite()
{
    if(!isFinished)
    {
        portReturnBuffer.clear();
        qDebug()<<"stop or time out";
        isFinished = true;
    }
}

void RSMUsb::onSerialPortAviablePortListChanged(const QStringList &portList)
{
    if(portList.isEmpty())
        return;
    aviablePortList = portList;
    this->setPortState(WaitForWork);
    emit this->aviablePortChanged(aviablePortList);
}

void RSMUsb::onDeviceNoRespond()
{
    static int errorCount = 0;
    errorCount++;
    if(errorCount>=2)
    {
        bool  needResetLinkConfig = false;
        if(QMessageBox::Yes ==  QMessageBox::critical(0,tr("Error"),
                              tr("Device has no respones,please check your connection!\n"
                                 "Choose 'yes' to reset current connection, and 'Ok' to close this window."),
                              QMessageBox::Yes,QMessageBox::Ok))
        {
            needResetLinkConfig = true;
        }
        errorCount =0;

        sender()->blockSignals(true);
        emit this->currentDeviceNoRespond(needResetLinkConfig);
        sender()->blockSignals(true);
    }
}

void RSMUsb::onErrorOpenPort(const int errorCode ,const QString &errorString)
{
    sender()->blockSignals(true);
//    if(!this->getActivePortName().isEmpty())
//    {
//    if(errorCode != QSerialPort::PermissionError)
//    {
//        QMessageBox::critical(0,"Error",errorString);
//    }
//    }
    sender()->blockSignals(false);
}

void RSMUsb::onDeviceError()
{
    sender()->blockSignals(true);
    QMessageBox::critical(0,tr("Error"),
                          tr("Device Has some error,please reset it and try again"));
    emit this->currentDeviceError();
    sender()->blockSignals(false);
}

void RSMUsb::onPortOpenSuccess(const QString &port)
{
    this->activePortName = port;
    emit this->activePortChanged(port);
}

void RSMUsb::onPortRefreshDone()
{
//    this->setPortState(NoValid);
}

//QList<double> RSMUsb::readPortsRealtimeTempValueInCDegree(const uchar &portNumberBegin, const uchar &portNumberEnd)
//{
//    QList<int16_t> digtalData = sendReadCMD(
//                deviceAddress,
//                TopTest::RsmCMD_FirstTempData + portNumberBegin -1,
//                portNumberEnd - portNumberBegin + 1);

//    qDebug()<<"RSMUsb::readPortsRealtimeTempValueInCDegree"<<digtalData.count()<<digtalData;


//    QList<double> ret;
//    foreach (int16_t data, digtalData) {
//        ret.push_back(parserDigtalData(data,0.1));
//    }

//    return ret;
//}

//QList<double> RSMUsb::readPortsRealtimeVoltageValueInMv(const uchar &portNumberBegin, const uchar &portNumberEnd)
//{
//    QList<int16_t> digtalData = sendReadCMD(
//                deviceAddress,
//                TopTest::RsmCMD_FirstVolageData + portNumberBegin -1 ,
//                portNumberEnd - portNumberBegin + 1);

//    qDebug()<<"RSMUsb::readPortsRealtimeVoltageValueInMv"<<digtalData.count()<<digtalData;

//    QList<double> ret;
//    foreach (int16_t data, digtalData) {
//        ret.push_back(parserDigtalData(data,0.01));
//    }

//    return ret;
//}

//bool RSMUsb::getCurrentIsHighScale()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_CurrentScaleLevel,
//            1);

//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return false;
//}

//double RSMUsb::readCurrentInteralPCBTempValueInCDegree()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_InnerTemp,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first(),0.1);
//    return 20.0;
//}

//double RSMUsb::readBatteryVoltageInV()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_BatteryVolage,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first(),0.01);
//    return 5.0;
//}

//int RSMUsb::getErrorCode()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_ErrorCode,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();

//    return TopTest::RsmDeviceError_NoneError;
//}

//QDateTime RSMUsb::readDeviceDateTime()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_Device_Data_Year,
//            6);
//    if(digtalData.count()>=6)
//        return QDateTime(QDate(digtalData[0],digtalData[1],digtalData[2]),
//                QTime(digtalData[3],digtalData[4],digtalData[5]));

//    return QDateTime::currentDateTime();
//}

//void RSMUsb::writeDeviceDateTime(const QDateTime &dataTime)
//{
//    QList<int16_t> dataList;
//    QDate date = dataTime.date();
//    QTime time = dataTime.time();
//    dataList<<date.year()<<date.month()<<date.day()
//            <<time.hour()<<time.minute()<<time.second();
//    sendWriteMultiCMD(deviceAddress,
//            TopTest::RsmCMD_Device_Data_Year,
//            dataList);
//}

//uchar RSMUsb::readDeviceNumberOfPort()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_Port_Number,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return 8;
//}

//void RSMUsb::writeDeviceNumberOfPort(const uchar &portNum)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_Port_Number,
//                       portNum);
//}

//uchar RSMUsb::readDeviceNetworkAddress()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_RTU_Address,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return 255;
//}

//void RSMUsb::writeDeviceNetworkAddress(const uchar &address)
//{
//    qDebug()<<"begin to write rtu address"<<address;
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_RTU_Address,
//                       address);
//}

//double RSMUsb::readSampleInteralTime()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_Sample_Time_Interal,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first(),0.05);
//    return 0.10;
//}

//void RSMUsb::writeSampleInteralTime(double interalTime)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_Sample_Time_Interal,
//                       interalTime/0.05 );
//}

//int RSMUsb::readDeviceStartMode()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_Start_Mode,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return 0;
//}

//void RSMUsb::writeDeviceStartMode(int mode)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_Start_Mode,
//                       mode);
//}

//double RSMUsb::readPressButtonDelayTime()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_PressStart_DelayTime,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first());
//    return 0.0;
//}

//void RSMUsb::writePressButtonDelayTime(double delayTime)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_PressStart_DelayTime,
//                       delayTime);
//}

//double RSMUsb::readFirstTimeTempStart_StartTempInC()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_FirstTempStart_Start_Temp,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first());
//    return 0.0;
//}

//void RSMUsb::writeFirstTimeTempStart_StartTempInC(double value)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_FirstTempStart_Start_Temp,
//                       value * 10);
//}

//double RSMUsb::readFirstTimeTempStart_StopTempInC()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_FirstTempStart_Stop_Temp,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first());
//    return 0.0;
//}

//void RSMUsb::writeFirstTimeTempStart_StopTempInC(double value)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_FirstTempStart_Stop_Temp,
//                       value * 10);
//}

//double RSMUsb::readFirstTimeTempStart_StopDelayTime()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_FirstTempStart_Stop_DelayTime,
//            1);
//    if(!digtalData.isEmpty())
//        return parserDigtalData(digtalData.first());
//    return 0.0;
//}

//void RSMUsb::writeFirstTimeTempStart_StopDelayTime(double value)
//{
//    sendWriteSingleCMD(deviceAddress,
//                       TopTest::RsmCMD_FirstTempStart_Stop_DelayTime,
//                       value);
//}

void RSMUsb::sendReadCMD(uchar deviceAddress, ushort dataAddress, ushort dataLength)
{
    int bufferSize = 8;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x03;
    cmdBuf[index++] = (dataAddress & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataAddress & 0x00FF;
    cmdBuf[index++] = (dataLength & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataLength & 0x00FF;

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    isFinished = false;
    sendCMDCode = 0x03;
    returnCMDCode = 0x00;
    isReadFile = false;
    returnData.clear();
    tryWriteToPort(hexToString(cmdBuf,bufferSize));

    delete cmdBuf;
}

void RSMUsb::sendWriteSingleCMD(uchar deviceAddress, ushort dataAddress, int16_t data)
{
    qDebug()<<"sendWriteSingleCMD "<<deviceAddress<<dataAddress<<data;

    int bufferSize = 8;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x06;
    cmdBuf[index++] = (dataAddress & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataAddress & 0x00FF;
    cmdBuf[index++] = (data & 0xFF00) / 0x100;
    cmdBuf[index++] =  data & 0x00FF;

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    isFinished = false;
    sendCMDCode = 0x06;
    returnCMDCode = 0x00;
    isReadFile = false;
    returnData.clear();
    portReturnBuffer.clear();
    tryWriteToPort(hexToString(cmdBuf,bufferSize));
//    portHandle->sendToPort();

    waitForMSecs(50);
//    waitForReadyWrite();
//    QByteArray result = portHandle->getReciveBuffer();
//    qDebug()<<result;

    delete cmdBuf;
}

void RSMUsb::sendWriteMultiCMD(uchar deviceAddress, ushort dataAddress, const QList<int16_t> &writeDataList)
{
    int dataCount = writeDataList.count();

    int bufferSize = 9 + dataCount * 2;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x10;
    cmdBuf[index++] = (dataAddress & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataAddress & 0x00FF;
    cmdBuf[index++] = 0x00;
    cmdBuf[index++] =  dataCount & 0x00FF;
    cmdBuf[index++] =  dataCount*2 & 0x00FF;

    qDebug()<<writeDataList;
    for(int i=0; i <writeDataList.count(); i++ )
    {
        int16_t value = writeDataList[i];
        cmdBuf[index++] = (value & 0xFF00) / 0x100;
        cmdBuf[index++] =  value & 0x00FF;
    }

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    isFinished = false;
    sendCMDCode = 0x10;
    returnCMDCode = 0x00;
    isReadFile = false;
    returnData.clear();
    tryWriteToPort(hexToString(cmdBuf,bufferSize));
//    portHandle->sendToPort();
//    waitForReadyWrite();
    QByteArray result; /*= portHandle->getReciveBuffer();*/
    qDebug()<<result;

    delete cmdBuf;
}

void RSMUsb::sendFileCMD(uchar deviceAddress, int16_t dataLength, char *data)
{
    int bufferSize = 6 + dataLength;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x20;
    cmdBuf[index++] = (dataLength & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataLength & 0x00FF;

    for(int i=0;i<dataLength;i++)
    {
        cmdBuf[index++] = data[i];
    }

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    isFinished = false;
    sendCMDCode = 0x20;
    returnCMDCode = 0x00;
    isReadFile = true;
    returnData.clear();
    portReturnBuffer.clear();
    sendAddress = deviceAddress;


    tryWriteToPort(hexToString(cmdBuf,bufferSize));

    if(this->baudRate == 500000)
    {
        waitForMSecs(150);
    }
    else
    {
        waitForMSecs(500/*300 + dataLength * 2 * 5 + 32 * 2 * 5*/);
    }

    delete cmdBuf;
}

void RSMUsb::sendReadCMDNew(uchar deviceAddress, ushort dataAddress, ushort dataLength)
{
    if(isInWait)
        return;

    int bufferSize = 8;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x03;
    cmdBuf[index++] = (dataAddress & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataAddress & 0x00FF;
    cmdBuf[index++] = (dataLength & 0xFF00) / 0x100;
    cmdBuf[index++] =  dataLength & 0x00FF;

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    isFinished = false;
    sendCMDCode = 0x03;
    returnCMDCode = 0x00;
    isReadFile = false;
    returnData.clear();
    portReturnBuffer.clear();
    sendAddress = dataAddress;
    qDebug()<<"sendAddress is"<<sendAddress;

    tryWriteToPort(hexToString(cmdBuf,bufferSize));

    delete cmdBuf;

    if(this->baudRate == 500000)
    {
        waitForMSecs(200);
    }
    else
    {
        waitForMSecs(300 + 6 * dataLength);
    }
}

//#define JudgeRemainDataLength(n) qDebug()<<"n "<<n;

#define JudgeRemainDataLength(n) \
        if(remainDataLength < n) \
        {\
                qDebug()<<__LINE__<<"length not enough,break";\
                break; \
        }\
        remainDataLength -= n;\
        qDebug()<<"n "<<n<<"remian Data Length"<<remainDataLength;


#define takeUnuseData(list,n) \
    for(int i =0; i<n; i++) \
    {                       \
       list.takeFirst();    \
    }                       \
    qDebug()<<"remianData"<<list;

///TODO::提供更加精确的更新服务
void RSMUsb::updateCurrentSerialData(int dataAddress/*, ushort dataLength*/)
{
    static int reciveCount = 0;
    QList<int16_t> digtalDataList = getDataFromHexBuffer(this->returnData.toHex());
    qDebug()<<"RSMUsb::updateCurrentSerialData"
           <<"dataAddress"<<dataAddress
           <<"reciveCount"<<++reciveCount
           <<"datalength "<<digtalDataList.count()
           <<"data list "<<digtalDataList;

    int remainDataLength = digtalDataList.count();
    switch (dataAddress) {
    //    RsmCMD_FirstTempData = 0x0000,
    case TopTest::RsmCMD_FirstTempData:
    {
        JudgeRemainDataLength(16)

        QList<double> portTempValues;
        for(int i = 0; i< 16; i++)
        {
            double tempValue = digtalDataList.takeFirst() * 0.1;
            if(tempValue < -200.0)
                tempValue = -200.0;
            portTempValues<< tempValue;
        }
        activeSerialData->portTempValues = portTempValues ;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_FirstVolageData;
        qDebug()<<"update portTempValues"<<portTempValues;
    }
    //    RsmCMD_FirstVolageData = 0x0010,
    case TopTest::RsmCMD_FirstVolageData:
    {
        JudgeRemainDataLength(16)

        QList<double> portVolValues;
        for(int i = 0; i< 16; i++)
        {
            portVolValues<< digtalDataList.takeFirst() * 0.01;
        }
        activeSerialData->portVolValues = portVolValues ;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_CurrentScaleLevel;
        qDebug()<<"update portVolValues"<<portVolValues;
    }
    ///user data
    //    RsmCMD_CurrentScaleLevel = 0x0020,
    case TopTest::RsmCMD_CurrentScaleLevel:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_CurrentScaleLevel]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_InnerTemp;
    }
    //    RsmCMD_InnerTemp = 0x0021,
    case TopTest::RsmCMD_InnerTemp:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_InnerTemp]
                = digtalDataList.takeFirst() * 0.1;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_BatteryVolage;
    }
    //    RsmCMD_BatteryVolage = 0x0022,
    case TopTest::RsmCMD_BatteryVolage:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_BatteryVolage]
                = digtalDataList.takeFirst() * 0.01;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_ErrorCode;
    }
    //    RsmCMD_ErrorCode = 0x0023,
    case TopTest::RsmCMD_ErrorCode:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_ErrorCode]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_RecordState;
    }
    case TopTest::RsmCMD_RecordState:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_RecordState]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_SoftwareVersion;
    }

    case TopTest::RsmCMD_SoftwareVersion:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_SoftwareVersion]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_SaveCMDCode_Begin;
    }
    //    RsmCMD_SaveCMDCode_Begin = 0x0024,
    case TopTest::RsmCMD_SaveCMDCode_Begin:
    {
        JudgeRemainDataLength(1)
//        activeSerialData->userData[TopTest::RsmCMD_SaveCMDCode_Begin]
//                = digtalDataList[dataAddress];
        takeUnuseData(digtalDataList,1)
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_Device_DateTime;

        JudgeRemainDataLength(TopTest::RsmCMD_Device_DateTime - 1 - TopTest::RsmCMD_SaveCMDCode_Begin)
        takeUnuseData(digtalDataList,TopTest::RsmCMD_Device_DateTime - 1 - TopTest::RsmCMD_SaveCMDCode_Begin)
    }
    //    RsmCMD_Device_Data_Year= 0x0030,
    //    RsmCMD_Device_Data_Moth= 0x0031,
    //    RsmCMD_Device_Data_Day= 0x0032,
    //    RsmCMD_Device_Time_Hour= 0x0033,
    //    RsmCMD_Device_Time_Minuter= 0x0034,
    //    RsmCMD_Device_Time_Second= 0x0035,
    case TopTest::RsmCMD_Device_DateTime:
    {
        JudgeRemainDataLength(6)

        int year = digtalDataList.takeFirst();
        int month = digtalDataList.takeFirst();
        int day = digtalDataList.takeFirst();
        int hour = digtalDataList.takeFirst();
        int min = digtalDataList.takeFirst();
        int sec = digtalDataList.takeFirst();
        QDateTime deviceDateTime(
                    QDate(year,month,day),
                    QTime(hour,min,sec)
                    );
        activeSerialData->userData[TopTest::RsmCMD_Device_DateTime]
                = deviceDateTime;
        emit this->deviceDataChanged(dataAddress);
        qDebug()<<"deviceDateTime is"<<deviceDateTime<<year<<month<<day<<hour<<min<<sec;
        dataAddress = TopTest::RsmCMD_Port_Number;
    }
//    RsmCMD_Port_Number = 0x0036,
    case TopTest::RsmCMD_Port_Number:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_Port_Number]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_RTU_Address;
    }
//    RsmCMD_RTU_Address = 0x0037,
    case TopTest::RsmCMD_RTU_Address:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_RTU_Address]
                = digtalDataList.takeFirst();
        qDebug()<<"rtu address"<<activeSerialData->userData[TopTest::RsmCMD_RTU_Address];
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_Sample_Time_Interal;
    }
//    RsmCMD_Sample_Time_Interal = 0x0038,
    case TopTest::RsmCMD_Sample_Time_Interal:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_Sample_Time_Interal]
                = digtalDataList.takeFirst() * 0.05;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_Start_Mode;
    }
//    RsmCMD_Start_Mode = 0x0039,
    case TopTest::RsmCMD_Start_Mode:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_Start_Mode]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_PressStart_DelayTime;
    }
//    RsmCMD_PressStart_DelayTime = 0x003A,
    case TopTest::RsmCMD_PressStart_DelayTime:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_PressStart_DelayTime]
                = digtalDataList.takeFirst()* 0.05;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_FirstTempStart_Start_Temp;
    }
//    RsmCMD_FirstTempStart_Start_Temp = 0x003B,
    case TopTest::RsmCMD_FirstTempStart_Start_Temp:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Start_Temp]
                = digtalDataList.takeFirst() * 0.1;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_FirstTempStart_Stop_Temp;
    }
//    RsmCMD_FirstTempStart_Stop_Temp = 0x003C,
    case TopTest::RsmCMD_FirstTempStart_Stop_Temp:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Stop_Temp]
                = digtalDataList.takeFirst() * 0.1;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_FirstTempStart_Stop_DelayTime;
    }
//    RsmCMD_FirstTempStart_Stop_DelayTime = 0x003D,
    case TopTest::RsmCMD_FirstTempStart_Stop_DelayTime:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_FirstTempStart_Stop_DelayTime]
                = digtalDataList.takeFirst() * 0.05 ;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_StopMode;

     }

    case TopTest::RsmCMD_StopMode:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_StopMode]
                = digtalDataList.takeFirst() ;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_PortModel;

        JudgeRemainDataLength(TopTest::RsmCMD_PortModel - 1 - TopTest::RsmCMD_StopMode)
        takeUnuseData(digtalDataList,TopTest::RsmCMD_PortModel - 1 - TopTest::RsmCMD_StopMode)
    }
//    RsmCMD_SecondTempStart_Start_Temp = 0x003E,
//    RsmCMD_SecondTempStart_Stop_Temp = 0x003F,
//    RsmCMD_SecondTempStart_Stop_DelayTime = 0x0040,

//    RsmCMD_ThirdAndForwardTempStart_Start_Temp = 0x0041,
//    RsmCMD_ThirdAndForwardTempStart_Stop_Temp = 0x0042,
//    RsmCMD_ThirdAndForwardTempStart_Stop_DelayTime = 0x0043,

//    RsmCMD_TempChart_JudgeMode = 0x0044,
//    RsmCMD_TempChart_ProtJudgeState = 0x0045,

//    RsmCMD_TempChart_JudgeTime1 = 0x0046,
//    RsmCMD_TempChart_JudgeTime2 = 0x0047,
//    RsmCMD_TempChart_JudgeTemp_Max = 0x0048,
//    RsmCMD_TempChart_JudgeTemp_Min = 0x0049,

//    RsmCMD_PortModel = 0x004A,
    case TopTest::RsmCMD_PortModel:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_PortModel]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmCMD_WirelessSwitch;
    }
//    RsmCMD_PowerRadio = 0x004B,
//    RsmCMD_WirelessSwitch = 0x004B,
    case TopTest::RsmCMD_WirelessSwitch:
    {
        JudgeRemainDataLength(1)

        activeSerialData->userData[TopTest::RsmCMD_WirelessSwitch]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress++;
        qDebug()<<"userData"<<activeSerialData->userData;
    }
        break;

    ///configure data
//        RsmConfig_ConfigPassword = 0x0100,
    case TopTest::RsmConfig_ConfigPassword:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_ConfigPassword]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_BatteryType;
    }
//        RsmConfig_BatteryType = 0x0101,
    case TopTest::RsmConfig_BatteryType:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_BatteryType]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_PortNumber;
    }
//        RsmConfig_PortNumber = 0x0102,
    case TopTest::RsmConfig_PortNumber:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_PortNumber]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_AdjustCMD;
    }
//        RsmConfig_AdjustCMD = 0x0103,
    case TopTest::RsmConfig_AdjustCMD:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_AdjustCMD]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_ActiveDevice;
    }
//        RsmConfig_ActiveDevice = 0x0104,
    case TopTest::RsmConfig_ActiveDevice:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_ActiveDevice]
                = digtalDataList.takeFirst();
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_DeviceCode;
    }
//        RsmConfig_DeviceCode = 0x0105,
    case TopTest::RsmConfig_DeviceCode:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_DeviceCode]
                = digtalDataList.takeFirst();
        qDebug()<<"device code"<<activeSerialData->configData[TopTest::RsmConfig_DeviceCode];
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_LowScale_ZeroAdjust_Begin;

//        JudgeRemainDataLength(TopTest::RsmConfig_LowScale_ZeroAdjust_Begin - 1 - TopTest::RsmConfig_DeviceCode)
//        takeUnuseData(digtalDataList,TopTest::RsmConfig_LowScale_ZeroAdjust_Begin - 1 - TopTest::RsmConfig_DeviceCode)
    }
//        RsmConfig_LowScale_ZeroAdjust_Begin = 0x0106,
//        RsmConfig_LowScale_ZeroAdjust_End = 0x0115,
//        RsmConfig_HighScale_ZeroAdjust_Begin = 0x0116,
//        RsmConfig_HighScale_ZeroAdjust_End = 0x0125,
    case TopTest::RsmConfig_LowScale_ZeroAdjust_Begin:
    {
        JudgeRemainDataLength(16)

        QList<int> portZeroAdjustValues;
        for(int i = 0; i< 16; i++)
        {
            portZeroAdjustValues<< digtalDataList.takeFirst();
        }
        activeSerialData->portZeroAdjustValues = portZeroAdjustValues ;
        qDebug()<<"update portZeroAdjustValues"<<portZeroAdjustValues;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_LowScale_WholeAdjust_Begin;

        JudgeRemainDataLength(TopTest::RsmConfig_LowScale_WholeAdjust_Begin - 1 - TopTest::RsmConfig_LowScale_ZeroAdjust_End)
        takeUnuseData(digtalDataList,TopTest::RsmConfig_LowScale_WholeAdjust_Begin - 1 - TopTest::RsmConfig_LowScale_ZeroAdjust_End)
        }

//        RsmConfig_LowScale_WholeAdjust_Begin = 0x0126,
//        RsmConfig_LowScale_WholeAdjust_End = 0x0135,
    case TopTest::RsmConfig_LowScale_WholeAdjust_Begin:
    {
        JudgeRemainDataLength(16)

        QList<int> portWholeAdjustValues;
        for(int i = 0; i< 16; i++)
        {
            portWholeAdjustValues<< digtalDataList.takeFirst();
        }
        activeSerialData->portWholeAdjustValues = portWholeAdjustValues ;
        qDebug()<<"update portWholeAdjustValues"<<portWholeAdjustValues;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_AD_Data_Begin;

        JudgeRemainDataLength(TopTest::RsmConfig_AD_Data_Begin - 1 - TopTest::RsmConfig_LowScale_WholeAdjust_End)
        takeUnuseData(digtalDataList,TopTest::RsmConfig_AD_Data_Begin - 1 - TopTest::RsmConfig_LowScale_WholeAdjust_End)
        }
//        RsmConfig_HighScale_WholeAdjust_Begin = 0x0136,
//        RsmConfig_HighScale_WholeAdjust_End = 0x0145,

//        RsmConfig_AD_Data_Begin = 0x0146,
//        RsmConfig_AD_Data_End = 0x0155,
    case TopTest::RsmConfig_AD_Data_Begin:
    {
        JudgeRemainDataLength(16)

        QList<int> portADValues;
        for(int i = 0; i< 16; i++)
        {
            portADValues<< digtalDataList.takeFirst();
        }
        activeSerialData->portADValues = portADValues ;
        qDebug()<<"update portADValues"<<portADValues;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_MaxValid_Temp;
    }

//        RsmConfig_MaxValid_Temp = 0x0156,
    case TopTest::RsmConfig_MaxValid_Temp:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_MaxValid_Temp]
                = digtalDataList.takeFirst() * 0.1;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_CoolEnd_Adjust_TempValue;
    }
//        RsmConfig_CoolEnd_Adjust_TempValue = 0x0157,
    case TopTest::RsmConfig_CoolEnd_Adjust_TempValue:
    {
        JudgeRemainDataLength(1)

        activeSerialData->configData[TopTest::RsmConfig_CoolEnd_Adjust_TempValue]
                = digtalDataList.takeFirst() * 0.1;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_User_Data_ProductNo;

        JudgeRemainDataLength(TopTest::RsmConfig_User_Data_ProductNo - 1 - TopTest::RsmConfig_CoolEnd_Adjust_TempValue)
        takeUnuseData(digtalDataList,TopTest::RsmConfig_User_Data_ProductNo - 1 - TopTest::RsmConfig_CoolEnd_Adjust_TempValue)
        }
//        RsmConfig_User_Data_Begin = 0x0160,
//        RsmConfig_User_Data_ProductNo = RsmConfig_User_Data_Begin,
    case TopTest::RsmConfig_User_Data_ProductNo:
    {

        JudgeRemainDataLength(10)

        QString ret;
        for(int i = 0 ; i < 10 ; i++)
        {
            ret.insert(i,(char) digtalDataList.takeFirst());
        }
        activeSerialData->configData[TopTest::RsmConfig_User_Data_ProductNo]
                = ret;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_User_Data_ProduceDateTimeBegin;
    }
//        RsmConfig_User_Data_ProduceDateTimeBegin = RsmConfig_User_Data_ProductNo+2,
    case TopTest::RsmConfig_User_Data_ProduceDateTimeBegin:
    {
        JudgeRemainDataLength(6)

        int year = digtalDataList.takeFirst();
        int month = digtalDataList.takeFirst();
        int day = digtalDataList.takeFirst();
        int hour = digtalDataList.takeFirst();
        int min = digtalDataList.takeFirst();
        int sec = digtalDataList.takeFirst();
        QDateTime produceDateTime(
                  QDate(year,month,day),
                  QTime(hour,min,sec)
                  );
        activeSerialData->configData[TopTest::RsmConfig_User_Data_ProduceDateTimeBegin]
                = produceDateTime;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_User_Data_DeviceModel;
    }
//        RsmConfig_User_Data_DeviceModel = RsmConfig_User_Data_ProduceDateTimeBegin+6,
    case TopTest::RsmConfig_User_Data_DeviceModel:
    {
        JudgeRemainDataLength(10)

        QString ret;
        for(int i = 0 ; i < 10 ; i++)
        {
            ret.insert(i,(char) digtalDataList.takeFirst());
        }
        activeSerialData->configData[TopTest::RsmConfig_User_Data_DeviceModel]
                = ret;
        emit this->deviceDataChanged(dataAddress);
        dataAddress = TopTest::RsmConfig_User_Data_DeviceModel_End;
        qDebug()<<"configure"<<activeSerialData->configData;
    }
//        RsmConfig_User_Data_DeviceModel_End = RsmConfig_User_Data_DeviceModel+10,
//        RsmConfig_User_Data_End = 0x09F

    default:
        break;
    }
}

void RSMUsb::resetLink()
{

    if(!portHandle || !portThread)
        return;

//    portHandle->moveToThread(0);
    portHandle->blockSignals(true);
    this->blockSignals(true);
    this->setPortState(ResetPort);

    timeId = QDateTime::currentDateTime().toMSecsSinceEpoch();

    this->waitForMSecs(20);

    portThread->quit();
//    portThread->terminate();



//    portHandle->blockSignals(false);

//    delete portThread;

//    portHandle = nullptr;
//    portThread = nullptr;

//    portThread = new QThread;
//    portHandle = new SerialPortTransfer;

//    portThread->run();
    portHandle->setTimeId(timeId);
    portHandle->closeCurrentPort();
    portHandle->setBaudRate(this->baudRate);
    portHandle->setTestCMD(this->currentTestCmd);
    portHandle->setValidRegString("");

//    portHandle->moveToThread(portThread);

    this->setPortState(RefreshPort);

    portThread->start();

    portHandle->blockSignals(false);
    this->blockSignals(false);


//    ///send to portHandle
//    connect(this,SIGNAL(needToSetBaudRate(int)),
//            portHandle,SLOT(setBaudRate(int)));

//    connect(this,SIGNAL(needToWriteToPort(QByteArray)),
//            portHandle,SLOT(sendToPort(QByteArray)));

//    connect(this,SIGNAL(needToOpenPort(QString)),
//            portHandle,SLOT(setActivePort(QString)));

//    connect(this,SIGNAL(needToCloseCurrentPort()),
//            portHandle,SLOT(closeCurrentPort()));

//    connect(this,SIGNAL(needToRefreshAviablePorts()),
//            portHandle,SLOT(refreshAviablePorts()));

//    connect(this,SIGNAL(testCmdChanged(QString)),
//            portHandle,SLOT(setTestCMD(QString)));

//    ///recive from portHandle
//    connect(portHandle,SIGNAL(portDataReady(QByteArray)),
//            this,SLOT(onPortDataReady(QByteArray)));

//    connect(portHandle,SIGNAL(currentDeviceNoRespond()),
//            this,SLOT(onDeviceNoRespond()));

//    connect(portHandle,SIGNAL(currentDeviceError()),
//            this,SLOT(onDeviceError()));

//    connect(portHandle,SIGNAL(errorOpenPort(int,QString)),
//            this,SLOT(onErrorOpenPort(int,QString)));

//    connect(portHandle,SIGNAL(aviablePortChanged(QStringList)),
//            this,SLOT(onSerialPortAviablePortListChanged(QStringList)));
}

qint64 RSMUsb::getTimeId() const
{
    return timeId;
}

void RSMUsb::setTimeId(const qint64 &value)
{
    timeId = value;
}

RSMUsb::PortState RSMUsb::getPortState() const
{
    return portState;
}

void RSMUsb::setPortState(const PortState &value)
{
    portState = value;
}

int RSMUsb::getBaudRate() const
{
    return baudRate;
}

void RSMUsb::setBaudRate(int value)
{
    baudRate = value;
    //    emit this->needToSetBaudRate(baudRate);
}

void RSMUsb::tryWriteToPort(const QByteArray &data)
{
    if(!portThread->isRunning())
        portThread->start();

    this->setPortState(Working);
    emit this->needToWriteToPort(data);
}

bool RSMUsb::getCanReadFile() const
{
    return canReadFile;
}

void RSMUsb::setCanReadFile(bool value)
{
    canReadFile = value;
}

uchar RSMUsb::getDeviceAddress() const
{
    return deviceAddress;
}

void RSMUsb::setDeviceAddress(const uchar &value)
{
    deviceAddress = value;
    QString cmd ;

    int bufferSize = 8;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = deviceAddress;
    cmdBuf[index++] = 0x03;
    cmdBuf[index++] = 0x01;
    cmdBuf[index++] = 0x25;
    cmdBuf[index++] = 0x00;
    cmdBuf[index++] = 0x01;

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    this->currentTestCmd = hexToString(cmdBuf,bufferSize).toHex();

    qDebug()<<"testCmd is"<<this->currentTestCmd;

    emit this->testCmdChanged(this->currentTestCmd);
}

void RSMUsb::closeCurrentPort()
{
    ///try exit event loop to drop any queue of slots
//    emit this->needToCloseCurrentPort();

//    if(!this->getActivePortName().isEmpty())
    this->resetLink();
}

void RSMUsb::clearDeviceData()
{
    activeSerialData->userData.clear();
    activeSerialData->configData.clear();
    activeSerialData->portTempValues.clear();
    activeSerialData->portVolValues.clear();
    activeSerialData->portADValues.clear();
    activeSerialData->portWholeAdjustValues.clear();
    activeSerialData->portZeroAdjustValues.clear();
}

bool RSMUsb::getIsFinished() const
{
    return isFinished;
}

void RSMUsb::setIsFinished(bool value)
{
    isFinished = value;
}



QString RSMUsb::getActivePortName() const
{
    return activePortName;
}

void RSMUsb::refreshAdjustData()
{
    //send cmd to refresh buffer data
    qDebug()<<"RSMUsb::refreshUserData()";
    sendReadCMDNew(deviceAddress,
                   TopTest::RsmConfig_LowScale_ZeroAdjust_Begin,
                   TopTest::RsmConfig_LowScale_WholeAdjust_End
                   - TopTest::RsmConfig_LowScale_ZeroAdjust_Begin + 1);
}

void RSMUsb::refreshSpecialData(int dataAddress, int dataLength)
{
    //send cmd to refresh buffer data
    qDebug()<<"RSMUsb::refreshUserData()";
    sendReadCMDNew(deviceAddress,
                   dataAddress,
                   dataLength );
}

void RSMUsb::refreshUserData()
{
    //send cmd to refresh buffer data
    qDebug()<<"RSMUsb::refreshUserData()";
    sendReadCMDNew(deviceAddress,
                   TopTest::RsmCMD_FirstTempData,
                   TopTest::RsmCMD_WirelessSwitch - TopTest::RsmCMD_FirstTempData +1 );
}

void RSMUsb::refreshConfigureData()
{
    //send cmd to refresh buffer data
    qDebug()<<"RSMUsb::refreshConfigureData()";
    sendReadCMDNew(deviceAddress,
                   TopTest::RsmConfig_ConfigPassword,
                   TopTest::RsmConfig_User_Data_DeviceModel_End -TopTest::RsmConfig_ConfigPassword +1 );

}

QVariant RSMUsb::getDeviceMapValue(TopTest::RsmCMD cmdCode)
{
    if(cmdCode < TopTest::RsmConfig_ConfigPassword )
        return activeSerialData->userData[cmdCode];
    else
        return activeSerialData->configData[cmdCode];
}

QList<double> RSMUsb::getPortTempValues()
{
    return activeSerialData->portTempValues;
}

QList<double> RSMUsb::getPortVolValues()
{
    return activeSerialData->portVolValues;
}

QList<int> RSMUsb::getPortADValues()
{
    return activeSerialData->portADValues;
}

QList<int> RSMUsb::getPortWholeAdjustValues()
{
    return activeSerialData->portWholeAdjustValues;
}

QList<int> RSMUsb::getPortZeroAdjustValues()
{
    return activeSerialData->portZeroAdjustValues;
}

QMap<int, QList<double> > RSMUsb::parserFileDataToPortMap(const int& portNum,
                                                 const QByteArray &array)
{
    QMap<int, QList<double> > ret;
    QDataStream ds(array);
    int16_t digtalTemp = 0;
    while(!ds.atEnd())
    {
        QList<double > tempList;
        for(int i=0;i<portNum;i++)
        {
            if(ds.atEnd())
                break;
            ds>>digtalTemp;
            double temp = digtalTemp * 0.1;
            if(temp < -200.0)
                temp = -200.0;
            tempList<< temp;
        }

        if(tempList.count() == portNum)
        {
            for(int i=0;i<portNum;i++)
            {
                ret[i+1].append(tempList[i]);
            }
        }
        else
            break;
    }

    return ret;
}

QList<QList<double> > RSMUsb::parserFileDataToRecordList(const int &portNum,
                                                         const QByteArray &array)
{
    QList<QList<double> > ret;
    QDataStream ds(array);
    int16_t digtalTemp = 0;
    while(!ds.atEnd())
    {
        QList<double > tempList;
        for(int i=0;i<portNum;i++)
        {
            if(ds.atEnd())
                break;
            ds>>digtalTemp;
            double temp = digtalTemp * 0.1;
            if(temp < -200.0)
                temp = -200.0;
            temp = TopTestCore::instance()->getRealValueFromSI(
                        temp,
                        TopTestCore::instance()->getCurrentTemputureUnit()
                        );
            tempList<< temp;
        }

        if(tempList.count() == portNum)
        {
//            for(int i=0;i<portNum;i++)
//            {
                ret<<tempList;
//            }
        }
        else
            break;
    }

    return ret;
}

QString RSMUsb::readFileRecordListInfo()
{
    QString dataStr("read file info");
    sendFileCMD(deviceAddress,dataStr.length(),dataStr.toUtf8().data());
    return returnData;
}

void RSMUsb::removeFileRecordHeadOnes(const int &num)
{
    QString dataStr = QString("del his file:%1").arg(num);
    sendFileCMD(deviceAddress,dataStr.length(),dataStr.toUtf8().data());
    //    return returnData;
}

void RSMUsb::clearFileRecord()
{
    QString dataStr = QString("del all file");
    sendFileCMD(deviceAddress,dataStr.length(),dataStr.toUtf8().data());
}

QByteArray RSMUsb::readFileContent(const QString &fileName, int dataLength)
{
    qDebug()<<"going to read file"<<fileName<<dataLength;

    QByteArray fileContent;

    int offset = 0, len = 0;

    const int bufferLength = 2048;

    while( dataLength > 0)
    {
        len = dataLength > bufferLength ? bufferLength : dataLength;

        QString dataStr = QString("read file:%1;offset:%2;len:%3")
                .arg(fileName).arg(offset).arg(len);

        qDebug()<<"file cmd is"<<dataStr;

        sendFileCMD(deviceAddress,dataStr.length(),dataStr.toUtf8().data());

        if(returnData.contains("read file error")
/*              || !QString(returnData).startsWith(QString("read file:%1;offset:%2;len:")
                                         .arg(fileName).arg(offset))*/)
        {
            qDebug()<<"read file error";
            fileContent.clear();
            break;
        }

//        QStringList strList = QString(returnData).split(";");
//        if(strList.count() == 4)
//            fileContent += strList.at(3);
//        qDebug()<<"add content to whole"<<returnData;
        fileContent += returnData ;
        qDebug()<<"current file content length"<<fileContent.length();
        emit needUpdateReadFileProgress(len);

        offset += bufferLength;
        dataLength -= bufferLength;
    }

    return fileContent;
}

QByteArray RSMUsb::readRealtimeFileContent(int dataLength)
{
    qDebug()<<"RSMUsb::readRealtimeFileContent"<<dataLength;
    Q_ASSERT(dataLength<=4096);
    return readFileContent("rt-data.bin",dataLength);
}

void RSMUsb::readRealtimeFileContentNew(int dataLength)
{
    qDebug()<<"RSMUsb::readRealtimeFileContentNew"<<dataLength;
    Q_ASSERT(dataLength<=240);
    return readFileContentNew("rt-data.bin",dataLength);
}

void RSMUsb::readFileContentNew(const QString &fileName, int dataLength)
{
    qDebug()<<"going to read file"<<fileName<<dataLength;

    QString dataStr = QString("read file:%1;offset:%2;len:%3")
            .arg(fileName).arg(0).arg(dataLength);

    qDebug()<<"file cmd is"<<dataStr;

    sendFileCMD(deviceAddress,dataStr.length(),dataStr.toUtf8().data());

    if(returnData.contains("read file error")
/*              || !QString(returnData).startsWith(QString("read file:%1;offset:%2;len:")
                                     .arg(fileName).arg(offset))*/)
    {
        qDebug()<<"read file error";
        returnData.clear();
        return;
    }
    qDebug()<<"current file content"<<returnData;
}

//int RSMUsb::readSensorType()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_PortModel,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return -1;
//}

//void RSMUsb::writeSensorType(int type)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmCMD_PortModel,type);
//}

//int RSMUsb::readPowerRadio()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_PowerRadio,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return -1;
//}

//void RSMUsb::writePowerRadio(int radioType)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmCMD_PowerRadio,radioType);
//}

//int RSMUsb::readWirelessSwitch()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmCMD_WirelessSwitch,
//            1);
//    if(!digtalData.isEmpty())
//        return digtalData.first();
//    return 0;
//}

//void RSMUsb::writeWirelessSwitch(bool on)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmCMD_PowerRadio,on ? 1 : 0);
//}

//int16_t RSMUsb::readConfigPasswd()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_ConfigPassword,1);
//    if(!resultList.isEmpty())
//        return resultList.first();
//    else
//        return 0x0000;
//}

//void RSMUsb::writeConfigPasswd(int16_t passwd)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_ConfigPassword,passwd);
//}

//int16_t RSMUsb::readBatteryType()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_BatteryType,1);
//    if(!resultList.isEmpty())
//        return resultList.first();
//    else
//        return 0x0000;
//}

//void RSMUsb::writeBatteryType(int16_t type)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_BatteryType,type);
//}

//int16_t RSMUsb::readDeviceMaxPortNum()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_PortNumber,1);
//    if(!resultList.isEmpty())
//        return resultList.first();
//    else
//        return -1;
//}

//void RSMUsb::writetDeviceMaxPortNum(int16_t maxPortNum)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_PortNumber,maxPortNum);
//    if(readDeviceNumberOfPort()!= -1)
//    {
//        if(readDeviceNumberOfPort() > maxPortNum)
//             writeDeviceNumberOfPort(maxPortNum);
//    }
//}

//void RSMUsb::writeAdjustCommand(int16_t command)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_AdjustCMD,command);
//}

//void RSMUsb::writeAllAdjustCommand()
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_AdjustCMD,0x10);
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_AdjustCMD,0x20);
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_AdjustCMD,0x30);
//}

//int16_t RSMUsb::readDeviceActiveState()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_ActiveDevice,1);
//    if(!resultList.isEmpty())
//        return resultList.first();
//    else
//        return -1;
//}

//void RSMUsb::writeDeviceActive(int16_t code)
//{
////    qDebug()<<"writeDeviceActive"<<code;
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_ActiveDevice, code);
//}

//int16_t RSMUsb::readDeviceID()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_DeviceCode,1);
//    if(!resultList.isEmpty())
//        return resultList.first();
//    else
//        return 0x0000;
//}

//QList<int> RSMUsb::readLowScaleAdjustZeroValue()
//{
//    QList<int> ret;
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmConfig_LowScale_ZeroAdjust_Begin,
//            16);
//    foreach (int16_t digValue, digtalData) {
//        ret<<digValue;
//    }
//    return ret;
//}

//QList<int> RSMUsb::readLowScaleAdjustWholeValue()
//{
//    QList<int> ret;
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmConfig_LowScale_WholeAdjust_Begin,
//            16);
//    foreach (int16_t digValue, digtalData) {
//        ret<<digValue;
//    }
//    return ret;
//}

//QList<int> RSMUsb::readHighScaleAdjustZeroValue()
//{
//    QList<int> ret;
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmConfig_HighScale_ZeroAdjust_Begin,
//            16);
//    foreach (int16_t digValue, digtalData) {
//        ret<<digValue;
//    }
//    return ret;
//}

//QList<int> RSMUsb::readHighScaleAdjustWholeValue()
//{
//    QList<int> ret;
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmConfig_HighScale_WholeAdjust_Begin,
//            16);
//    foreach (int16_t digValue, digtalData) {
//        ret<<digValue;
//    }
//    return ret;
//}

//QList<int16_t> RSMUsb::readADValueList()
//{
//    return sendReadCMD(deviceAddress,TopTest::RsmConfig_AD_Data_Begin,16);
//}

//double RSMUsb::readMaxTestTemp()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_MaxValid_Temp,1);
//    if(!resultList.isEmpty())
//        return parserDigtalData(resultList.first(),0.1);
//    else
//        return 2000.0;
//}

//void RSMUsb::writeMaxTestTemp(double maxTemp)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_MaxValid_Temp,maxTemp*10);
//}

//double RSMUsb::readCoolSenserAdjustValue()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,TopTest::RsmConfig_CoolEnd_Adjust_TempValue,1);
//    if(!resultList.isEmpty())
//        return parserDigtalData(resultList.first(),0.1);
//    else
//        return 0.0;
//}

//void RSMUsb::writeCoolSenserAdjustValue(double coolAdjustValue)
//{
//    sendWriteSingleCMD(deviceAddress,TopTest::RsmConfig_CoolEnd_Adjust_TempValue,coolAdjustValue*10);
//}

//QString RSMUsb::readDeviceModelStr()
//{
//    ///
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,
//                                            TopTest::RsmConfig_User_Data_DeviceModel,10);
//    QString ret;

//    for(int i=0;i<10;i++)
//    {
//        int16_t digValue = resultList[i];
//        ret.insert(i, (char) digValue);
//    }

//    qDebug()<<"deviceModelStr"<<ret;
//    return ret;
//}

//void RSMUsb::writeDeviceModelStr(const QString &modelStr)
//{
//    //
//    qDebug()<<"RSMUsb::writeDeviceModelStr"<<modelStr<<modelStr.length();
//    QString writeStr = modelStr;
//    while(writeStr.length()<10)
//    {
//        writeStr+=" ";
//    }

//    QList<int16_t> dataList;
//    for(int i=0; i< 10 ; i++)
//    {
//        int16_t value = writeStr.at(i).toLatin1();
//        dataList<<value;
//    }

//    sendWriteMultiCMD(deviceAddress,TopTest::RsmConfig_User_Data_DeviceModel,dataList);

//}

//QString RSMUsb::readDeviceProductID()
//{
//    QList<int16_t> resultList = sendReadCMD(deviceAddress,
//                                            TopTest::RsmConfig_User_Data_ProductNo,10);
//    QString ret;

//    for(int i=0;i<10;i++)
//    {
//        int16_t digValue = resultList[i];
//        ret.insert(i, (char) digValue);
//    }

//    qDebug()<<"readDeviceProductID"<<ret;
//    return ret;
//}

//void RSMUsb::writeDeviceProductID(const QString& id)
//{
//    qDebug()<<"RSMUsb::writeDeviceProductID"<<id<<id.length();
//    QString writeStr = id;
//    while(writeStr.length()<10)
//    {
//        writeStr+=" ";
//    }

//    QList<int16_t> dataList;
//    for(int i=0; i< 10 ; i++)
//    {
//        int16_t value = writeStr.at(i).toLatin1();
//        dataList<<value;
//    }

//    sendWriteMultiCMD(deviceAddress,TopTest::RsmConfig_User_Data_ProductNo,dataList);
//}

//QDateTime RSMUsb::readDeviceProduceDateTime()
//{
//    QList<int16_t> digtalData = sendReadCMD(deviceAddress,
//            TopTest::RsmConfig_User_Data_ProduceDateTimeBegin,
//            6);
//    if(digtalData.count()>=6)
//        return QDateTime(QDate(digtalData[0],digtalData[1],digtalData[2]),
//                QTime(digtalData[3],digtalData[4],digtalData[5]));

//    return QDateTime::currentDateTime();
//}

//void RSMUsb::writeDeviceProduceDataTime(QDateTime dateTime)
//{
//    QList<int16_t> dataList;
//    QDate date = dateTime.date();
//    QTime time = dateTime.time();
//    dataList<<date.year()<<date.month()<<date.day()
//            <<time.hour()<<time.minute()<<time.second();
//    sendWriteMultiCMD(deviceAddress,
//            TopTest::RsmConfig_User_Data_ProduceDateTimeBegin,
//            dataList);
//}


void RSMUsb::waitForReadyWrite(int msec)
{
    QTimer::singleShot(msec,this,SLOT(stopWaitForReadyWrite()));
    while(!isFinished)
    {
        QCoreApplication::processEvents();
    }
}

//void RSMUsb::waitForReadyRead(int msec)
//{
//    portHandle->waitForReadyRead(msec);
//}
