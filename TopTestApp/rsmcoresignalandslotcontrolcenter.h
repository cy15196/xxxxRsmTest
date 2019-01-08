#ifndef RSMCORESIGNALANDSLOTCONTROLCENTER_H
#define RSMCORESIGNALANDSLOTCONTROLCENTER_H

#include <QObject>
#include "rsmcoresignalenum.h"

class RSMCoreSignalAndSlotControlCenter : public QObject
{
    Q_OBJECT
public:

    static RSMCoreSignalAndSlotControlCenter* instance();

signals:

    void startShowDialog();
    void endShowDialog();

    void needChangeToPage(QWidget*widget, int pageEnum);

public slots:

private:

   explicit RSMCoreSignalAndSlotControlCenter(QObject *parent = nullptr);

   static RSMCoreSignalAndSlotControlCenter* rsmSCC;

};

#endif // RSMCORESIGNALANDSLOTCONTROLCENTER_H
