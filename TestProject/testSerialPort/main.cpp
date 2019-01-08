#include <QCoreApplication>

#include <QDebug>
#include "serialporttransfer.h"
#include "toptestusb.h"
#include "rsmusb.h"
#include <QDateTime>
#include <QTimer>
//#include <QDataStream>

void waitForMSecs(int svalue)
{
    QTime dieTime = QTime::currentTime().addMSecs(svalue);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    SerialPortTransfer serialPortTransfer;
//    QStringList portList = serialPortTransfer.refreshAviablePorts();
//    qDebug()<< portList;

//    foreach (QString portName, portList) {
//        serialPortTransfer.setActivePort(portName);
//        qDebug()<<"test port :"<<portName<< serialPortTransfer.testPort();
//    }

//    char sendBuffer[7] = {0};
//    sendBuffer[0] = 0xaa;
//    sendBuffer[1] = 0x52;
//    sendBuffer[2] = 0x00;
//    sendBuffer[3] = 0x00;
//    sendBuffer[4] = 0x00;
//    sendBuffer[5] = 0x00;
//    sendBuffer[6] = 0x55;

//    QByteArray arrayBuffer;
//    arrayBuffer.append(sendBuffer);

//    arrayBuffer = QByteArray::fromHex("aa520000000055");
//    QDataStream ds(&arrayBuffer);
//    ds<<0xaa<<0x52<<0x07<<0x00<<0x00<<0x00<<0x55;
//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa320000000055"));

//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa520000000055"));
//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa520800000055"));
//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa520700010055"));
//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa320000020055"));
//    serialPortTransfer.sendToPort(QByteArray::fromHex("aa520800030055"));

//    while(1)
//    {
//        serialPortTransfer.sendToPort(QByteArray::fromHex("aa520700010055"));
//    }

//    TopTestUsb usb;
//    usb.setPortHandle(&serialPortTransfer);

//    usb.ReadDeviceID();

    RSMUsb::instance()->setBaudRate(500000);
//    portHandle->refreshAviablePorts();
//    RSMUsb::instance()->getPortHandle()->setBaudRate(9600);
    RSMUsb::instance()->refreshAviablePortList();
    waitForMSecs(2000);
    QStringList aList = RSMUsb::instance()->getAviablePortList();
//    QDateTime startTime;
//    QDateTime endTime;
    if(!aList.isEmpty())
    {
        RSMUsb::instance()->setActivePortName(aList.first());
//        RSMUsb::instance()->readDeviceValueItem(TopTest::RsmCMD_BatteryVolage);
//        startTime = QDateTime::currentDateTime();
//        for(int i = 0; i < 10;i++)
//        {
            RSMUsb::instance()->refreshUserData();
            waitForMSecs(300);
            RSMUsb::instance()->refreshAdjustData();
////            RSMUsb::instance()->refreshUserData();
////            waitForMSecs(20);
//        }
//        endTime = QDateTime::currentDateTime();
//        qDebug()<<"start is"<<startTime<<"end time is"<<endTime;
//        qDebug()<<"cost time is"<<startTime.msecsTo(endTime);

//        int portNum = RSMUsb::instance()
//                ->getDeviceMapValue(TopTest::RsmCMD_Port_Number).toInt();
//        for(int i = 0; i < 20;i++)
//        {
//            QByteArray array = RSMUsb::instance()->readRealtimeFileContent(2 * portNum * 10 );
//            qDebug()<<RSMUsb::instance()->parserFileDataToPortMap(portNum,array);

//            waitForMSecs(1000);
//        }

    }

    return a.exec();
}
