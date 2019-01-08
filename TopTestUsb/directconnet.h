#ifndef DIRECTCONNET_H
#define DIRECTCONNET_H

#include "rep_port_replica.h"

class DirectConnect : public QObject
{
    Q_OBJECT
public:
    DirectConnect(QSharedPointer<PortReplica> ptr);
    ~DirectConnect() override;

signals:
    void sig_sendMessage(QString);
    void sig_newMessage(QString);

protected slots:
    void slot_lastMessageChanged(QString);
    void slot_serverTimeChanged(QString);
    void slot_testPod(Foo);
    void slot_stateChanged(QRemoteObjectReplica::State, QRemoteObjectReplica::State);

private:
    QSharedPointer<PortReplica> m_ptr;
};

#endif // DIRECTCONNET_H
