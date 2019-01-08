#ifndef CLABSOREACTION_H
#define CLABSOREACTION_H


#include <QAction>
#include <QList>
#include "toptestactions_global.h"

class TOPTESTACTIONSSHARED_EXPORT ClabsoEnableHelper: public QObject
{

    Q_OBJECT
public:
    static ClabsoEnableHelper* instance();

    void setActionEnable(const QString& className, bool enable);

signals:
    void actionEnableChanged(const QString& className, bool enable);

private:

    ClabsoEnableHelper(QObject* parent= 0);

    static ClabsoEnableHelper* helper;

};



class TOPTESTACTIONSSHARED_EXPORT ClabsoEnableAction: public QAction
{
    Q_OBJECT
public :

    static inline ClabsoEnableAction* instance(const QString&name);

    ClabsoEnableAction(const QIcon& icon, const QString& text, const QString& objName = QString(), QObject* parent = 0);


private slots:
     virtual void onActionEnableChanged(const QString&className, bool enable);


     void  onClicked(bool check);

     virtual void onTriggered(bool) = 0;
protected:
     static QMap<QString, ClabsoEnableAction*> instanceMap;
};

#endif // CLABSOREACTION_H
