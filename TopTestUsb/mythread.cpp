#include "mythread.h"

#include "serialporttransfer.h"

MyThread::MyThread()
{

}

void MyThread::stop()
{
//    portHandle->blockSignals(true);
//    if(portHandle)
//    {
//        delete portHandle;
//        portHandle = nullptr;
//    }
//    this->quit();
}

void MyThread::run()
{
    this->exec();
}

SerialPortTransfer *MyThread::getPortHandle() const
{
    return portHandle;
}

void MyThread::setPortHandle(SerialPortTransfer *value)
{
    portHandle = value;
}
