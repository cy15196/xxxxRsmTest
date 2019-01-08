#include "toptestportmanager.h"
#include "serialporttransfer.h"

#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QTimer>

TopTestPortManager * TopTestPortManager::ttpm = nullptr;
TopTestPortManager *TopTestPortManager::instance()
{
    if(!ttpm)
        ttpm = new TopTestPortManager;
    return ttpm;
}

TopTestPortManager::TopTestPortManager(QObject *parent)
    :PortSimpleSource(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    portThread = new QThread;

    portHandle = new SerialPortTransfer;
    this->setTestCmd("FF030125000181E3");
    portHandle->setTestCMD("FF030125000181E3");
    portHandle->setBaudRate(500000);
    portHandle->setValidRegString("");
    portHandle->moveToThread(portThread);
    portThread->start();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(refreshAviablePortList()));

    ///send to portHandle
    connect(this,SIGNAL(bauderateChanged(int)),
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
//    connect(portHandle,SIGNAL(portDataReady(QByteArray)),
//            this,SLOT(onPortDataReady(QByteArray)));

//    connect(portHandle,SIGNAL(currentDeviceNoRespond()),
//            this,SLOT(onDeviceNoRespond()));

//    connect(portHandle,SIGNAL(currentDeviceError()),
//            this,SLOT(onDeviceError()));

//    connect(portHandle,SIGNAL(errorOpenPort(int,QString)),
//            this,SLOT(onErrorOpenPort(int,QString)));

    connect(portHandle,SIGNAL(aviablePortChanged(QStringList)),
            this,SLOT(onSerialPortAviablePortListChanged(QStringList)));

//    connect(portHandle,SIGNAL(refreshPortDone()),
//            this,SIGNAL(refreshPortDone()));

}

TopTestPortManager::~TopTestPortManager()
{
    m_timer->stop();
    m_timer->deleteLater();
}

void TopTestPortManager::addCommandToPort(qint64 timeId, QByteArray cmd)
{
    cmdStack.push_back(QPair<qint64,QByteArray>(timeId,cmd));
}

void TopTestPortManager::closeCurrentPort()
{
//    emit this->needToCloseCurrentPort();

    if(!portHandle || !portThread)
        return;

//    portHandle->moveToThread(0);
    portHandle->blockSignals(true);

//    this->waitForMSecs(200);

    portThread->quit();
//    portThread->terminate();



//    portHandle->blockSignals(false);

//    delete portThread;

//    portHandle = nullptr;
//    portThread = nullptr;

//    portThread = new QThread;
//    portHandle = new SerialPortTransfer;

//    portThread->run();
    portHandle->closeCurrentPort();
    portHandle->setBaudRate(this->bauderate());
    portHandle->setTestCMD(this->testCmd());
    portHandle->setValidRegString("");

//    portHandle->moveToThread(portThread);

    portThread->start();

    portHandle->blockSignals(false);


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

void TopTestPortManager::chooseActivePort(QString portName)
{
    this->setActivePort(portName);
}

void TopTestPortManager::refreshAviablePortList()
{
    emit this->needToRefreshAviablePorts();
}

void TopTestPortManager::changeTestCmd(QString testCmd)
{
    this->setTestCmd(testCmd);
}

void TopTestPortManager::changeBauderate(int baduerate)
{
    this->setBauderate(baduerate);
}

void TopTestPortManager::onSerialPortAviablePortListChanged(QStringList list)
{
    this->setAviablePortList(list);
}


