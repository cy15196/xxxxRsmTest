#ifndef NEWACTION_H
#define NEWACTION_H

#include "clabsoenablehelper.h"


namespace TopTest
{
  struct ProjectInfo;
}

class TOPTESTACTIONSSHARED_EXPORT NewAction: public ClabsoEnableAction
{
    Q_OBJECT
public:
    NewAction(const QIcon &icon, const QString& text = QString(), const QString& objName = QString(), QObject *parent =0);


signals:
    void historyRefreshed();

private slots:
    void onTriggered(bool);


    bool createEmptyFile(const TopTest::ProjectInfo &info);
};

#endif // NEWACTION_H
