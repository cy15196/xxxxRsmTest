#ifndef SHOWMONITORACTION_H
#define SHOWMONITORACTION_H

#include "clabsoenablehelper.h"
class TOPTESTACTIONSSHARED_EXPORT ShowMonitorAction: public ClabsoEnableAction
{
    Q_OBJECT
public:
    ShowMonitorAction(const QIcon &icon, const QString& text = QString(), const QString& objName = QString(), QObject *parent =0);

private slots:
    void onTriggered(bool);
};

#endif // SHOWMONITORACTION_H
