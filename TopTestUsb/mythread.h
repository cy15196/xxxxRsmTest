#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class SerialPortTransfer;

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();

    void pullPortHandleToThread(QThread* thread);
    void stop();

    void run() override;


    SerialPortTransfer *getPortHandle() const;
    void setPortHandle(SerialPortTransfer *value);

private:
    SerialPortTransfer * portHandle = nullptr;
};

#endif // MYTHREAD_H
