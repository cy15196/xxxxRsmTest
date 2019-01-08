#include "directconnet.h"

#include <QDebug>

DirectConnect::DirectConnect(QSharedPointer<PortReplica> ptr)
    : QObject(nullptr), m_ptr(ptr)
{
    qDebug()<<"DirectConnect::DirectConnect";
     QObject::connect(this, SIGNAL(sig_sendMessage(QString)), m_ptr.data(), SLOT(reveiceMessage(QString)));

     QObject::connect(m_ptr.data(), SIGNAL(lastMessageChanged(QString)), this, SLOT(slot_lastMessageChanged(QString)));
     QObject::connect(m_ptr.data(), SIGNAL(serverTime(QString)), this, SLOT(slot_serverTimeChanged(QString)));
     QObject::connect(m_ptr.data(), SIGNAL(testPod(Foo)), this, SLOT(slot_testPod(Foo)));
     QObject::connect(m_ptr.data(), SIGNAL(stateChanged(QRemoteObjectReplica::State,QRemoteObjectReplica::State)),
                      this, SLOT(slot_stateChanged(QRemoteObjectReplica::State,QRemoteObjectReplica::State)));
}

DirectConnect::~DirectConnect()
{

}

void DirectConnect::slot_lastMessageChanged(QString msg)
{
    //pros test
    QString text = "Pros: " + msg;

    emit sig_newMessage(text);
}

void DirectConnect::slot_serverTimeChanged(QString msg)
{
    //signal test
    QString text = "Signal: " + msg;

    emit sig_newMessage(text);
}

void DirectConnect::slot_testPod(Foo foo)
{
    //Pod test
    QList<QString> list = foo.bar();

    for(auto itor = list.begin(); itor != list.end(); itor++)
    {
        QString text = "POD: " + *itor;
        emit sig_newMessage(text);
    }
}

void DirectConnect::slot_stateChanged(QRemoteObjectReplica::State new_state, QRemoteObjectReplica::State old_state)
{
    //State test
    QMetaEnum metaEnum = QMetaEnum::fromType<QRemoteObjectReplica::State>();
    QString text = "State: ";

    text.append(metaEnum.valueToKey(old_state));
    text.append("->");
    text.append(metaEnum.valueToKey(new_state));



    qDebug()<<"state changed"<<text;

    emit sig_newMessage(text);
}
