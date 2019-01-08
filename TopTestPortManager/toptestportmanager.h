#ifndef TOPTESTPORTMANAGER_H
#define TOPTESTPORTMANAGER_H

#include "rep_port_source.h"
class SerialPortTransfer;
class QThread;

class TopTestPortManager: public PortSimpleSource
{

    Q_OBJECT
public:
    static TopTestPortManager* instance();
    explicit TopTestPortManager(QObject *parent = nullptr);
    ~TopTestPortManager() override;

public slots:
    void addCommandToPort(qint64 timeId,QByteArray cmd) override;
    void closeCurrentPort() override;
    void chooseActivePort(QString portName) override;
    void refreshAviablePortList() override;
    void changeTestCmd(QString testCmd) override;
    void changeBauderate(int baduerate)  override;


    void onSerialPortAviablePortListChanged(QStringList list);


signals:

    /// deal with serial port
    void needToSetBaudRate(int rate);

    void needToWriteToPort(const QByteArray& data);

    void needToCloseCurrentPort();

    void needToRefreshAviablePorts();

    void needToOpenPort(const QString& portName);

private:
    QTimer *m_timer;
    SerialPortTransfer* portHandle = nullptr;
    QThread* portThread = nullptr;

    static TopTestPortManager * ttpm;

    QStack<QPair<qint64,QByteArray>> cmdStack;
};

#endif // TOPTESTPORTMANAGER_H
