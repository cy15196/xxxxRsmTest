#ifndef RSMCORESIGNALENUM_H
#define RSMCORESIGNALENUM_H

#include "clabso_ft_macro.h"

enum NaviEnum{
    Page_Home,
    Page_DataVisual,
    Page_VirtualReserach,
    Page_RealtimeTest,
    Page_FileSetting,
    Page_OnlineMonitor,
    Page_SPKCPK,
    Page_Help,
    Page_Count
};

enum ToolButtonEnum{
    Home_DataVisual = Page_DataVisual,
    Dialog_Login = Page_Count,
    Dialog_OnlineTestSetting,
    Dialog_HardSetting,
    Dialog_SystemSetting,
    Dialog_CurrentLinkDevice,
    Dialog_DataDownload,
    Dialog_DataPortDefine,
    Dialog_DataCut,
    Dialog_DataCompare,
    Dialog_FileOptionChoose,
    Dialog_FileHandleWave,
    Dialog_FileInfoSet,
    Dialog_FileOpenOther,
    Dialog_FileSave,
    Dialog_FileDelete,
    Dialog_SetVelocity,
    Dialog_OutToData,
    Dialog_OutToFile,
    Dialog_Monitor_LinkSetting,
    Dialog_Monitor_Switch,
    Dialog_Monitor_Record_Switch,
    Dialog_Monitor_Reset,
    Tab_FileSetting_Product,
    Tab_FileSetting_Device,
    Tab_FileSetting_Process,
    Tab_FileSetting_Exchange,
    Chart_ShowDetail,
    Chart_FitAll,
    Chart_SetAxis,
    Chart_SetDistanceAndVelocity,
    Chart_MoveSection,
    Chart_MoveCurve,
    Chart_AddHLine,
    Chart_AddVLine,
    Chart_Reapply,
    Chart_ShowTempSection

};


#endif // RSMCORESIGNALENUM_H
