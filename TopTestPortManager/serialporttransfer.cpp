#include "serialporttransfer.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QCoreApplication>
#include <QRegExp>

#include <iostream>


//#include <Windows.h>
//#include <dbt.h>

SerialPortTransfer::SerialPortTransfer(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);
    lastCmdDataTime = QDateTime::currentDateTime();
    writeHasReturn = true;
    isFinised = true;
    BaudRate = 500000;
    sendCount = 0;
//    judgeRepondTimer = 0;
    connect(serial,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
            this,SLOT(onError()));
    connect(serial,SIGNAL(readyRead()),
            this,SLOT(onPortReadyRead()));
//    connect(&judgeRepondTimer,SIGNAL(timeout()),
//            this,SLOT(onJudgeTimerOut()));

}

SerialPortTransfer::~SerialPortTransfer()
{
//    if(judgeRepondTimer)
//    {
//        judgeRepondTimer->stop();
//        delete judgeRepondTimer;
//    }

    if(serial->isOpen())
        serial->close();
    delete serial;
}

#define TopTestPortTest

void SerialPortTransfer::refreshAviablePorts()
{
    qDebug()<<"SerialPortTransfer::refreshAviablePorts()"<<this->BaudRate<<this->testCMD;
    QStringList portNameList;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            //设置波特率
            serial.setBaudRate(BaudRate);
            //设置数据位数
            serial.setDataBits(QSerialPort::Data8);
             //设置奇偶校验
            serial.setParity(QSerialPort::NoParity);
            //设置停止位
            serial.setStopBits(QSerialPort::OneStop);
            //设置流控制
            serial.setFlowControl(QSerialPort::NoFlowControl);

            serial.write(QByteArray::fromHex(testCMD.toLatin1()));
            serial.waitForBytesWritten();
            serial.waitForReadyRead(2000);
//            if(serial.bytesAvailable()==0)
//                continue;
            QByteArray buffer = serial.readAll().toHex();
            qDebug()<<info.portName()<<buffer;

            QRegExp reg(validRegString);
            if(!buffer.isEmpty() && buffer.size() == 14 /*&& reg.exactMatch(buffer)*/ )
                portNameList.append(serial.portName());
            serial.close();
        }

        else
        {
            std::cout<< "port open error"<<serial.error()<<'\t'
                      << serial.errorString().toUtf8().data()<<std::endl;

            QString errorStr =  QString("error open port:%1 with BaudRate %2")
                                                      .arg(serial.portName())
                                                      .arg(this->BaudRate);
            emit this->errorOpenPort(serial.error(),errorStr);
        }
    }

    if(!portNameList.isEmpty())
        emit this->aviablePortChanged(portNameList);

    refreshPortDone();
}

void SerialPortTransfer::setActivePort(const QString &portName)
{
//    serial->close();
//    serial->reset();
    if(serial->isOpen())
        serial->close();

    if(portName.isEmpty())
        return;

    //设置串口名
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.portName() == portName)
        {
            serial->setPortName(portName);
            serial->setPort(info);
            break;
        }
    }

    //打开串口
    if(! serial->open(QIODevice::ReadWrite))
    {
        qDebug()<<"SerialPort:"<<"open serial port failed"<<portName;
        activePortName = "";
        return;
    }
    //设置波特率
    serial->setBaudRate(BaudRate);
    //    serial->setBaudRate(BaudRate);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
     //设置奇偶校验
     serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);

    activePortName = portName;

    qDebug()<<"SerialPort:"<<"open serial port success"<<activePortName<<this->BaudRate;
}

void SerialPortTransfer::sendToPort(const QByteArray &data)
{
//    cmdStack.push_back(data);
    writeToPort(data);
//    noRespondMap.insert(sendCount++,QTime::currentTime());
//    qDebug()<<"current count is"<<noRespondMap;
    //    tryWriteToPort();
}


QByteArray SerialPortTransfer::readPortData()
{
    return reciveBuffer;
}

const int waitInteral = 200;
void SerialPortTransfer::tryWriteToPort()
{

    static int waitCount = 0;
    if(cmdStack.isEmpty())
    {
        waitCount = 0;
        return;
    }
    QDateTime tempDateTime = QDateTime::currentDateTime();
    int ms = lastCmdDataTime.msecsTo(tempDateTime);
    qDebug()<<"ms from last cmd "<< ms;

    if( ms >= waitInteral )
    {
        writeToPort(cmdStack.takeFirst());
    }
    else
    {
        waitCount++;
        QTimer::singleShot(waitInteral * waitCount,this,SLOT(tryWriteToPort()));
    }
}

void SerialPortTransfer::judgeRespond()
{
//    qDebug()<<"SerialPortTransfer::judgeRespond"<<noRespondMap<<"currentSend is"<<sendCount;
    if(lastCmdDataTime.msecsTo(lastReviceDataTime) > 3000)
    {
//        judgeRepondTimer->stop();
        writeHasReturn = true;
        noRespondMap.clear();
//        serial->close();
        emit this->currentDeviceNoRespond();
        return;
    }
}

bool SerialPortTransfer::testPort()
{

    disconnect(serial,SIGNAL(readyRead()),
            this,SLOT(onPortReadyRead()));

    serial->write(QByteArray::fromHex("aa520000000055"));
    serial->waitForBytesWritten();
    serial->waitForReadyRead(500);
    QByteArray buffer = serial->readAll().toHex();
    qDebug()<<buffer;

    connect(serial,SIGNAL(readyRead()),
            this,SLOT(onPortReadyRead()));

    if( buffer.startsWith("55") && buffer.endsWith("aa") )
        return true;
    return false;
}

void SerialPortTransfer::onPortReadyRead()
{
    lastReviceDataTime = QDateTime::currentDateTime();
    writeHasReturn = true;
//    qDebug()<<noRespondMap<<sendCount;
//    if(lastCmdDataTime.msecsTo(lastReviceDataTime) > 3000)
//    {
//        QTime sendTime = noRespondMap[sendCount];
//        if(sendTime.msecsTo(QTime::currentTime()) < 3000)
//        {
//            noRespondMap.remove(sendCount);
//        }
//    }
//    judgeRepondTimer->stop();

    QByteArray bufferData = serial->readAll();
    qDebug()<<"SerialPortTransfer::port data ready:"<<bufferData.toHex();
    qDebug()<<"writeHasReturn"<<writeHasReturn;
    reciveBuffer += bufferData.toHex();

//    if(reciveBuffer.startsWith("55")&&reciveBuffer.endsWith("aa"))
//    {
        qDebug()<<"port data ready:"<<reciveBuffer;
        emit this->portDataReady(bufferData);
//        isFinised = true;
//    }
}

void SerialPortTransfer::onError()
{
    isFinised = true;
    if(serial->error() != QSerialPort::NoError)
    {
        qDebug()<<"Error occured with the port:"<<serial->errorString();
//        emit  this->errorOpenPort(serial->errorString());
        emit this->currentDeviceError();
    }
}

void SerialPortTransfer::onJudgeTimerOut()
{
    qDebug()<<"sender is"<<sender();
    qDebug()<<"timer out";
    judgeRespond();
}


int SerialPortTransfer::getBaudRate() const
{
    return BaudRate;
}

void SerialPortTransfer::setBaudRate(int value)
{
    if(BaudRate == value || value < 0)
        return;

    BaudRate = value;
    qDebug()<<"set baud rate to"<<value;
    setActivePort(activePortName);
}

QList<int> SerialPortTransfer::getAviableBaudRateList()
{
    static QList<int> ret;

    if(ret.isEmpty())
    {
        ret<<1200<<2400<<4800<<9600
          <<19200<<38400<<57600<<115200<<500000;

    }

    return ret;
}

void SerialPortTransfer::waitForReadyRead(int msec)
{
    serial->waitForReadyRead(msec);
}


void SerialPortTransfer::closeCurrentPort()
{
    qDebug()<<"SerialPortTransfer::closeCurrentPort()";
    if(serial->isOpen())
    {
        serial->clear();
        serial->close();
    }
    activePortName = "";
}

void SerialPortTransfer::pushCmdToStack(const QByteArray& cmd)
{
    cmdStack.push_back(cmd);
}

void SerialPortTransfer::writeToPort(const QByteArray & data)
{
//    if(judgeRepondTimer == 0)
//    {
//        judgeRepondTimer = new QTimer;
//        judgeRepondTimer->setInterval(3000);
//        connect(judgeRepondTimer,SIGNAL(timeout()),
//                this,SLOT(onJudgeTimerOut()));
//    }
//    qDebug()<<"SerialPortTransfer::writeToPort()"<<noRespondMap<<sendCount;
    if(lastCmdDataTime.msecsTo(lastReviceDataTime) > 3000)/*noRespondMap.contains(sendCount)*/
    {
//        judgeRepondTimer->stop();
        writeHasReturn = true;
        noRespondMap.clear();
        emit this->currentDeviceNoRespond();
        return;
    }

    //    serial->waitForReadyRead();
    //    serial->waitForBytesWritten();
    //    this->waitForReadyWrite();
    reciveBuffer.clear();
    isFinised = false;
    qDebug()<<"begin to write data to port"<<data.toHex();
    writeHasReturn = false;

    if(serial->isWritable())
    {
        serial->write(data);
        serial->waitForBytesWritten();
    }
    lastCmdDataTime = QDateTime::currentDateTime();
    QTimer::singleShot(3000,this,SLOT(judgeRespond()));
//    judgeRepondTimer->start();
    //    serial->waitForReadyRead();
    //    qDebug()<<"read data"<<serial->readAll().toHex();
}

//bool SerialPortTransfer::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
//{
//    if ("windows_dispatcher_MSG" == eventType

//            || "windows_generic_MSG" == eventType)

//        {

//            MSG * msg = reinterpret_cast<MSG*>(message);

//            if(msg->message == WM_DEVICECHANGE)

//            {

//                PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;

//                switch(msg->wParam)

//                {

//                case DBT_DEVICEARRIVAL://检测到新设备

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)

//                    {

//                        qDebug() << "DBT_DEVICEARRIVAL";

////                        updateMoveDrives();

//                    }

//                    break;

//                case DBT_DEVICEQUERYREMOVE://请求移除设备，可能失败  此时刷新不会让移动设备消失

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)

//                    {

//                        qDebug() << "DBT_DEVICEQUERYREMOVE";

//                    }

//                    break;

//                case DBT_DEVICEQUERYREMOVEFAILED://去除中断

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)

//                    {

//                        qDebug() << "DBT_DEVICEQUERYREMOVEFAILED";

//                    }

//                    break;

//                case DBT_DEVICEREMOVEPENDING://即将删除，仍然有效

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)

//                    {

//                        qDebug() << "DBT_DEVICEREMOVEPENDING";

//                    }

//                    break;

//                case DBT_DEVICEREMOVECOMPLETE://设备不见了

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)

//                    {

//                        qDebug() << "DBT_DEVICEREMOVECOMPLETE";

////                        updateMoveDrives();

//                    }

//                    break;

//                case DBT_CUSTOMEVENT:

//                    if (lpdb->dbch_devicetype == DBT_DEVTYP_HANDLE)

//                    {

//                        qDebug() << "DBT_CUSTOMEVENT";

////                        updateMoveDrives();

//                    }

//                    break;

//                case DBT_DEVNODES_CHANGED:

//                    qDebug() << "DBT_DEVNODES_CHANGED";

////                    updateMoveDrives();

//                    break;

//                default:

//                    qDebug() << msg->wParam;

//                }

////                outputDrives();
//                return true;
//         }

//    }

//    return false;
//}

bool SerialPortTransfer::getIsFinised() const
{
    return isFinised;
}

void SerialPortTransfer::setIsFinised(bool value)
{
    isFinised = value;
}

void SerialPortTransfer::setValidRegString(const QString &value)
{
    validRegString = value;
}

void SerialPortTransfer::setTestCMD(const QString &value)
{
    qWarning()<<"testCMD changed to"<<value;
    if(testCMD == value)
        return;
    testCMD = value;
}

QByteArray SerialPortTransfer::getReciveBuffer() const
{
    return reciveBuffer;
}


//void SerialPortTransfer::waitForReadyWrite()
//{
//    while(!isFinised)
//    {
//        QCoreApplication::processEvents();
//    }
//}
