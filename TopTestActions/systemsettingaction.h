#ifndef SYSTEMSETTINGACTION_H
#define SYSTEMSETTINGACTION_H

#include "clabsoenablehelper.h"
class TOPTESTACTIONSSHARED_EXPORT SystemSettingAction: public ClabsoEnableAction
{
    Q_OBJECT
public:
    SystemSettingAction(const QIcon &icon, const QString& text = QString(), const QString& objName = QString(), QObject *parent =0);
private slots:
    void onTriggered(bool);
};

#endif // SYSTEMSETTINGACTION_H
