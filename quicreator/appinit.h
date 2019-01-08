#ifndef APPINIT_H
#define APPINIT_H

#include <QObject>

#ifdef TOPTEST
#include "quicreator_global.h"
class QUICREATORSHARED_EXPORT AppInit : public QObject
#else
class AppInit : public QObject
#endif
{
    Q_OBJECT
public:
    static AppInit *Instance();
    explicit AppInit(QObject *parent = 0);    

    void start();

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static AppInit *self;

signals:

public slots:
};

#endif // APPINIT_H
