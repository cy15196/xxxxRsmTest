#include "rsmcoresignalandslotcontrolcenter.h"

RSMCoreSignalAndSlotControlCenter* RSMCoreSignalAndSlotControlCenter::rsmSCC = 0;

RSMCoreSignalAndSlotControlCenter::RSMCoreSignalAndSlotControlCenter(QObject *parent) : QObject(parent)
{

}

RSMCoreSignalAndSlotControlCenter *RSMCoreSignalAndSlotControlCenter::instance()
{
    if(!rsmSCC)
    {
        rsmSCC = new RSMCoreSignalAndSlotControlCenter;
    }

    return rsmSCC;
}
