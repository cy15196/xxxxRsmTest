#include "toptestusb.h"
#include <Windows.h>
#include <QDebug>
//#include "ftd2xx.h"
//#include <QDataStream>

#include "serialporttransfer.h"


#define TopTest_Read 0x52
#define TopTest_Write 0x58

#define TopTest_Send_Begin 0xAA
#define TopTest_Send_End 0x55

#define TopTest_Responde_Begin 0x55
#define TopTest_Responde_End 0xAA
#define TopTest_Empty 0x00

//QMap<> parserReciveData(uint buffer[])
//{

//}

TopTestUsb * usb = 0;

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

inline QByteArray hexToString(uint* numArray, int length = 7)
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

inline QList<uint>  getDataFromHexBuffer(QByteArray data,uint buffer[3])
{
    QByteArray temp = data.mid(6,data.size()-8);

    bool ok;
    int length = temp.size();

    QList<uint> ret;

    while(length>0)
    {
        temp = temp.right(length);
        ret.push_back(QString(temp.left(2)).toInt(&ok,16));
        length -= 2 ;
    }

    if(ret.size()==3)
    {
        for(int i=0;i<3;i++)
            buffer[i] = ret.at(i);
    }

    return ret;
}

//class TopTestUsbPrivate{
//public:

//    TopTestUsbPrivate(){
//        bool state = this->OpenUSBPort();
//        if(!state)
//        {
//            qDebug()<<"open usb port failed";
//            return;
//        }
//        this->SetUSBParams();
//    }

//    ~TopTestUsbPrivate(){
//        this->CloseUSBProt();
//    }

//    bool OpenUSBPort() // 串口设备虚拟成一个handle
//    {
//         FT_STATUS ftStatus;
//         ftStatus = FT_Open(0, &handle);
//         if(ftStatus==FT_OK)
//          return 1;
//         else
//          return 0;
//    }
//    //关闭设备：
//    bool CloseUSBProt()
//    {
//        FT_STATUS ftStatus;
//        ftStatus = FT_Close(handle);
//        if(ftStatus==FT_OK)
//            return 1;
//        else
//            return 0;
//    }

//    bool SendCMD(void* cmd)
//    {
//    //    FT_Write();
//        FT_STATUS ftStatus;
//        DWORD BytesWritten;
//        memcpy(SendBuf,cmd,sizeof(SendBuf));

//        ftStatus = FT_Write(handle,SendBuf,sizeof(SendBuf),&BytesWritten);
//        if(ftStatus == FT_OK){
//        // FT_Write OK
////            qDebug()<<BytesWritten;
//            return 1;
//        }
//        else {
//        // FT_Write Failed
//            return 0;
//        }
//    }

//        //对串口通信进行参数设置：
//    void SetUSBParams()
//    {
//        // 设置波特率（必须的）
//        FT_SetBaudRate(handle, FT_BAUD_115200);
//        // 设置字符格式（必须的）
//        FT_SetDatauintacteristics(handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
//        // 设置输入输出缓存（必须的）
//        FT_SetUSBParameters(handle, 1024, 512);
//        // 清除输入缓存
//        FT_Purge(handle, FT_PURGE_RX);
//        // 清除输出缓存
//        FT_Purge(handle, FT_PURGE_TX);
//    }
//        //数据读取：(这里假定每次只读取一个字节，存放在b中)
//    bool USBRead(uint* data)
//    {
//        DWORD dwRXBytes;
//        DWORD TxBytes;
//        DWORD EventDWord;
//        DWORD RxBytes;
//        FT_STATUS ftStatus;
//        if(handle == NULL) {
//            ftStatus =FT_Open(0, &handle);
//            if(ftStatus!=FT_OK)
//                return 0;
//        }
//        if(handle)
//        {
////            FT_GetStatus(handle, &RxBytes, &TxBytes, &EventDWord);
////            if (RxBytes)
////            {
//                 ftStatus=FT_Read(handle, &ReciveBuf, sizeof(ReciveBuf), &dwRXBytes);   // 这里每次只读取一个字节
//                 if(ftStatus==FT_OK)
//                 {
//                     data[0] = ReciveBuf[3];
//                     data[1] = ReciveBuf[4];
//                     data[2] = ReciveBuf[5];
////                   result += ReciveBuf;
////                     QString s;
////                     s.append(ReciveBuf);

////                     qDebug()<<s.toUtf8().toHex(':');
//                     // 读取成功之后...
//                     return 1;
//                 }
////             }
//        }
//        return 0;
//    }


//    FT_HANDLE handle;
//    uint SendBuf[8];
//    uint ReciveBuf[8];
//};

TopTestUsb *TopTestUsb::instance()
{
    if(!usb)
        usb = new TopTestUsb;
    return usb;
}

TopTestUsb::TopTestUsb()
    :portHandle(new SerialPortTransfer)
{
    portNum = 16;
//    TestDevice();
//    ResetDevice();
//    ReadDeviceID();
//    SetSampleTime(0.1);
//    qDebug()<<"The internal value is"<<ReadDeviceInternalValue();
//    //    ReadDeviceInternalValue();
}

TopTestUsb::~TopTestUsb()
{
    delete portHandle;
}

QStringList TopTestUsb::getAviablePortList()
{
    return {} /*portHandle->refreshAviablePorts()*/;
}

void TopTestUsb::setActivePortName(const QString &portName)
{
    portHandle->setActivePort(portName);
}

QString TopTestUsb::ReadDeviceID()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_CURRENT_DEVICE_CODE,test);
    QString deviceName;
    for(int i=0;i<3;i++)
    {
        if(test[i]>'a' && test[i] < 'z' ||
               test[i]>'A' && test[i] < 'Z' )
            deviceName+= char(test[i]);
        else
            deviceName+= QString::number(test[i]);
    }
    return deviceName;
}

double TopTestUsb::ReadDeviceInternalValue()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_DEVICE_INNER_TEMPERATURE,test);

    return parserTemputureData(test[1],test[2]);
}

TopTest::TopTestStartMode TopTestUsb::ReadDeviceStartMode()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_DEVICE_STARTMODE,test);
    return TopTest::TopTestStartMode(test[0]);
}

void TopTestUsb::SetDeviceDelayStartTime(int delayTimeSec)
{
    uint test[3] = {0};
    SetToInternal(delayTimeSec,0,200)
    test[2] = delayTimeSec;
    sendCMD(TopTest::SET_DEVICE_STRAT_DELAY_TIME,test);
}

int TopTestUsb::ReadDeviceDelayStartTime()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_DEVICE_STRAT_DELAY_TIME,test);
    int delayTimeSec = test[2];
    SetToInternal(delayTimeSec,0,200)
    return delayTimeSec;
}

void TopTestUsb::SetDeviceDelayStartTemputure(int value)
{
    uint test[3] = {0};
    SetToInternal(value,25,100)
    test[2] = value;
    sendCMD(TopTest::SET_TEMPERATURE_STRAT_VALUE,test);
}

double TopTestUsb::ReadDeviceDelayStartTemputure()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_TEMPERATURE_STRAT_VALUE,test);
    int value = test[2];
    SetToInternal(value,25,100)
    return value;
}

QDate TopTestUsb::ReadTestStartDate()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_TEST_DEVICE_DATE,test);
    int year = test[0];
    int month = test[1];
    int day = test[2];

    return QDate(year,month,day);
}

QTime TopTestUsb::ReadTestStartTime()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_TEST_DEVICE_TIME,test);
    int hour = test[0];
    int min = test[1];
    int sec = test[2];

    return QTime(hour,min,sec);
}

void TopTestUsb::SetActivePortNum(const int &portNum)
{
    uint data[3] = {0};
    uint cmdBuf[7];
    cmdBuf[0] = (TopTest_Send_Begin);
    cmdBuf[1] = (TopTest_Write);
    cmdBuf[2] = (0x00);
    cmdBuf[3] = (0x00);
    cmdBuf[4] = (0x00);
    cmdBuf[5] = (portNum);
    cmdBuf[6] = (TopTest_Send_End);

    portHandle->sendToPort(hexToString(cmdBuf));
//    portHandle->sendToPort();
//    bool retVal = d_ptr->SendCMD(cmdBuf);
//    qDebug()<<"send "<<retVal<<"read "<< d_ptr->USBRead(data);
}

double TopTestUsb::ReadZeroValue()
{
    uint data[3] = {0};
    uint cmdBuf[7];
    cmdBuf[0] = (TopTest_Send_Begin);
    cmdBuf[1] = (0x30);
    cmdBuf[2] = (0x00);
    cmdBuf[3] = (0x00);
    cmdBuf[4] = (0x00);
    cmdBuf[5] = (0x00);
    cmdBuf[6] = (TopTest_Send_End);

    portHandle->sendToPort(hexToString(cmdBuf));
//    bool retVal = d_ptr->SendCMD(cmdBuf);
//    qDebug()<<"send "<<retVal<<"read "<< d_ptr->USBRead(data);
    return 0x10000* data[0] + 0x100 * data[1] + data[2];
}

void TopTestUsb::ResetDevice()
{
    uint data[3] = {0};
    uint cmdBuf[7];
    cmdBuf[0] = (TopTest_Send_Begin);
    cmdBuf[1] = (0x32);
    cmdBuf[2] = (0x00);
    cmdBuf[3] = (0x00);
    cmdBuf[4] = (0x00);
    cmdBuf[5] = (0x00);
    cmdBuf[6] = (TopTest_Send_End);

    portHandle->sendToPort(hexToString(cmdBuf));
//    bool retVal = d_ptr->SendCMD(cmdBuf);
//    qDebug()<<"send "<<retVal<<"read "<< d_ptr->USBRead(data);
}

void TopTestUsb::SetMaxValue(const double &value)
{
    uint data[3] = {0};
    uint cmdBuf[7];
    int tempValue = value * 10;
    cmdBuf[0] = (TopTest_Send_Begin);
    cmdBuf[1] = (0x31);
    cmdBuf[2] = (0x00);
    cmdBuf[3] = (0x00);
    cmdBuf[4] = (tempValue & 0xff00);
    cmdBuf[5] = (tempValue & 0x00ff);
    cmdBuf[6] = (TopTest_Send_End);

    portHandle->sendToPort(hexToString(cmdBuf));
//    bool retVal = d_ptr->SendCMD(cmdBuf);
//    qDebug()<<"send "<<retVal<<"read "<< d_ptr->USBRead(data);
}

void TopTestUsb::SetDeviceStartMode(TopTest::TopTestStartMode mode)
{
    uint test[3] = {0};
    test[0] = mode;
    sendCMD(TopTest::READ_DEVICE_STARTMODE,test);
}

double TopTestUsb::ReadSampleTime(int rowNo)
{
    uint test[3] = {rowNo,0,0};//通道数
    sendCMD(TopTest::READ_SAMPLE_FREQUENCY,test);
    uint value = test[1]* 0x100 + test[2];
    double deltaTime = value * 0.05 ;
    if(deltaTime < 0.05)
        deltaTime = 0.05;
    else if(deltaTime > 2000)
        deltaTime = 2000;
    return deltaTime;
}

void TopTestUsb::SetSampleTime(double deltaTime, int rowNo)
{
    uint test[3] = {rowNo,0,0};
    SetToInternal(deltaTime,0.05,2000)
    uint value = deltaTime * 20 ;
    test[1]=value / 0x100;
    test[2]=value - test[1]*0x100;
    sendCMD(TopTest::SET_SAMPLE_FREQUENCY,test);
}

int TopTestUsb::ReadRowSampleLength(const int &rowNum)
{
    uint test[3] = {0};
    test[0] = rowNum;
    sendCMD(TopTest::READ_SAMPLE_LENTGH,test);
    uint length = test[1]* 0x100 + test[2];

    SetToInternal(length,0,0x2000)

    return length;
}

QMap<int,QList<double>> TopTestUsb::ReadRowSampleData(const int &rowNum)
{
    uint test[3] = {rowNum,0,0};
    QMap<int,QList<double>> ret;
    QList<uint> wholeDataList = sendCMD(TopTest::READ_SAMPLE_DATA,test);

    while(wholeDataList.count()>=portNum*2)
    {
        for(int i =1;i<= portNum;i++)
        {
            uint data1 = wholeDataList.takeFirst();
            uint data2 = wholeDataList.takeFirst();

            ret[i].append(parserTemputureData(data1,data2));
        }
    }

    return ret;
    //return parserTemputureData(test[1],test[2]);
}

void TopTestUsb::ClearDeviceData()
{
    uint test[3] = {0};
    sendCMD(TopTest::CLEAR_DEVICE_DATA,test);
}

QDate TopTestUsb::ReadDeviceDate()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_DEVICE_DATE,test);
    int year = 2000+ test[0];
    int month = test[1];
    int day = test[2];

    return QDate(year,month,day);
}

void TopTestUsb::SetDeviceDate(const QDate &date)
{
    uint test[3] = {0};
    test[0] = date.year()-2000;
    test[1] = date.month();
    test[2] = date.day();
    sendCMD(TopTest::SET_DEVICE_DATE,test);
}

QTime TopTestUsb::ReadDeviceTime()
{
    uint test[3] = {0};
    sendCMD(TopTest::READ_DEVICE_TIME,test);
    int hour = test[0];
    int min = test[1];
    int sec = test[2];

    return QTime(hour,min,sec);
}

void TopTestUsb::SetDeviceTime(const QTime &time)
{
    uint test[3] = {0};
    test[0] = time.hour();
    test[1] = time.minute();
    test[2] = time.second();
    sendCMD(TopTest::SET_DEVICE_TIME,test);
}

QList<double> TopTestUsb::ReadPortCurrentValue(/*const int &port*/)
{
    uint test[3] = {0};
//    test[0] = port;
    QList<uint> listData = sendCMD(TopTest::READ_PORT_TEMPERATURE,test);
    QList<double > portData;

    for(int i=0;i<listData.count()-1;i+=2)
    {
        portData.push_back(parserTemputureData(listData[i],listData[i+1]));
    }
    return portData;
}

QList<uint> TopTestUsb::sendCMD(TopTest::TopTestCMD cmdCode, uint *data )
{
    qDebug()<<sizeof(data);
    Q_ASSERT(data!=0);
    uint cmdBuf[7];
    cmdBuf[0] = (TopTest_Send_Begin);
    cmdBuf[1] = (cmdCode & TopTest::TOPTEST_WRITE ? TopTest_Write : TopTest_Read );
    cmdBuf[2] = (cmdCode & 0x0f);
    cmdBuf[3] = (data[0]);
    cmdBuf[4] = (data[1]);
    cmdBuf[5] = (data[2]);
    cmdBuf[6] = (TopTest_Send_End);

//    qDebug()<<hexToString(cmdBuf).toHex();
    QByteArray dataToSend = hexToString(cmdBuf);
    Q_ASSERT(dataToSend.size() == 7);
    portHandle->sendToPort(hexToString(cmdBuf));
//    portHandle->();
    QByteArray result = portHandle->getReciveBuffer();
    qDebug()<<result;

    return getDataFromHexBuffer(result,data);

//    bool retVal = d_ptr->SendCMD(cmdBuf);
//    qDebug()<<"read operation"<< d_ptr->USBRead(data);
}

SerialPortTransfer *TopTestUsb::getPortHandle() const
{
    return portHandle;
}

void TopTestUsb::setPortHandle(SerialPortTransfer *value)
{
    portHandle = value;
}
