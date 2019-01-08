#ifndef OPENACTION_H
#define OPENACTION_H

#include "clabsoenablehelper.h"
class TOPTESTACTIONSSHARED_EXPORT OpenAction: public ClabsoEnableAction
{
    Q_OBJECT
public:    
    OpenAction(const QIcon &icon, const QString& text = QString(), const QString& objName = QString(), QObject *parent =0);

    static void refreshHistoryList(const QString &currentFilePath, bool openSuccess);

    static bool openProjectFile(const QString& filePath);

signals:
    void historyRefreshed();
    void fileOpened(const QString &filePath);
private slots:
    void onTriggered(bool);

    void openDataFileSlot(const QString &filePath);

};

#endif // OPENACTION_H
