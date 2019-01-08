#ifndef TOPTESTENUM_H
#define TOPTESTENUM_H

#include "clabso_ft_macro.h"

namespace TopTest {

enum ProjectType{
   DataAnalysis,
   ProgressHandle
};

enum FormWidgetType{
    LABEL,
    LCD,
    LINEEDIT
};

enum UnitType
{
    C=0x00,
    F=0x01,

    s=0x02,
    perMin=s,
    min=0x03,
    perS=min,

    inch=0x10,
    feet=0x20,

    mm=0x30,
    cm=0x40,
    m=0x50,

    inchPerMin = inch|perMin,
    feetPerMin = feet|perMin,

    mmPerMin = mm|perMin,
    cmPerMin = cm|perMin,
    mPerMin = m|perMin
};

enum DeviceType{
    TopTestGX80,
    RSMII
};

enum TopTestRW{
    TOPTEST_READ = 0x00,
    TOPTEST_WRITE = 0x10
};

enum RsmRW{
    Rsm_Read = 0x03,
    Rsm_Write_Single = 0x06,
    Rsm_Write_Multi = 0x10
};

enum RsmCMD{
    RsmCMD_FirstTempData = 0x0000,
    RsmCMD_FirstVolageData = 0x0010,
    RsmCMD_CurrentScaleLevel = 0x0020,
    RsmCMD_InnerTemp = 0x0021,
    RsmCMD_BatteryVolage = 0x0022,
    RsmCMD_ErrorCode = 0x0023,
    RsmCMD_RecordState = 0x0024,
    RsmCMD_SoftwareVersion = 0x0025,
    RsmCMD_SaveCMDCode_Begin = 0x0026,
    RsmCMD_SaveCMDCode_End = 0x002F,
    RsmCMD_Device_DateTime= 0x0030,
    RsmCMD_Device_Data_Year= 0x0030,
    RsmCMD_Device_Data_Moth= 0x0031,
    RsmCMD_Device_Data_Day= 0x0032,
    RsmCMD_Device_Time_Hour= 0x0033,
    RsmCMD_Device_Time_Minuter= 0x0034,
    RsmCMD_Device_Time_Second= 0x0035,
    RsmCMD_Port_Number = 0x0036,
    RsmCMD_RTU_Address = 0x0037,
    RsmCMD_Sample_Time_Interal = 0x0038,
    RsmCMD_Start_Mode = 0x0039,
    RsmCMD_PressStart_DelayTime = 0x003A,

    RsmCMD_FirstTempStart_Start_Temp = 0x003B,
    RsmCMD_FirstTempStart_Stop_Temp = 0x003C,
    RsmCMD_FirstTempStart_Stop_DelayTime = 0x003D,

    RsmCMD_StopMode = 0x003E,
    RsmCMD_SecondTempStart_Start_Temp = 0x003E,
    RsmCMD_SecondTempStart_Stop_Temp = 0x003F,
    RsmCMD_SecondTempStart_Stop_DelayTime = 0x0040,

    RsmCMD_ThirdAndForwardTempStart_Start_Temp = 0x0041,
    RsmCMD_ThirdAndForwardTempStart_Stop_Temp = 0x0042,
    RsmCMD_ThirdAndForwardTempStart_Stop_DelayTime = 0x0043,

    RsmCMD_TempChart_JudgeMode = 0x0044,
    RsmCMD_TempChart_ProtJudgeState = 0x0045,

    RsmCMD_TempChart_JudgeTime1 = 0x0046,
    RsmCMD_TempChart_JudgeTime2 = 0x0047,
    RsmCMD_TempChart_JudgeTemp_Max = 0x0048,
    RsmCMD_TempChart_JudgeTemp_Min = 0x0049,

    RsmCMD_PortModel = 0x004A,
    RsmCMD_PowerRadio = 0x004B,
    RsmCMD_WirelessSwitch = 0x004B,

    RsmConfig_ConfigPassword = 0x0100,
    RsmConfig_BatteryType = 0x0101,
    RsmConfig_PortNumber = 0x0102,
    RsmConfig_AdjustCMD = 0x0103,
    RsmConfig_ActiveDevice = 0x0104,
    RsmConfig_DeviceCode = 0x0105,
    RsmConfig_LowScale_ZeroAdjust_Begin = 0x0106,
    RsmConfig_LowScale_ZeroAdjust_End = 0x0115,
    RsmConfig_HighScale_ZeroAdjust_Begin = 0x0116,
    RsmConfig_HighScale_ZeroAdjust_End = 0x0125,

    RsmConfig_LowScale_WholeAdjust_Begin = 0x0126,
    RsmConfig_LowScale_WholeAdjust_End = 0x0135,
    RsmConfig_HighScale_WholeAdjust_Begin = 0x0136,
    RsmConfig_HighScale_WholeAdjust_End = 0x0145,

    RsmConfig_AD_Data_Begin = 0x0146,
    RsmConfig_AD_Data_End = 0x0155,

    RsmConfig_MaxValid_Temp = 0x0156,
    RsmConfig_CoolEnd_Adjust_TempValue = 0x0157,
    RsmConfig_User_Data_Begin = 0x0160,
    RsmConfig_User_Data_ProductNo = 0x0160,//+10
    RsmConfig_User_Data_ProduceDateTimeBegin = RsmConfig_User_Data_ProductNo+10,//+6
    RsmConfig_User_Data_DeviceModel = RsmConfig_User_Data_ProduceDateTimeBegin+6,//+10
    RsmConfig_User_Data_DeviceModel_End = RsmConfig_User_Data_DeviceModel+10,
    RsmConfig_User_Data_End = 0x19F

};

enum RsmDeviceErrorCode{
    RsmDeviceError_NoneError=0x0000,
    RsmDeviceError_StorageError = 0x0001,
    RsmDeviceError_TempSensorError = 0x0002,
    RsmDeviceError_ClockChipError = 0x0004,
    RsmDeviceError_CrystalError = 0x0008
};

enum TopTestCMD{
    READ_CURRENT_DEVICE_CODE = 0x00,
    READ_SAMPLE_FREQUENCY = 0x01,
    SET_SAMPLE_FREQUENCY = TOPTEST_WRITE | READ_SAMPLE_FREQUENCY,
    READ_SAMPLE_LENTGH = 0x02,
    READ_SAMPLE_DATA = 0x03,
    CLEAR_DEVICE_DATA = 0x04 | TOPTEST_WRITE ,
    READ_DEVICE_DATE = 0x05,
    SET_DEVICE_DATE = TOPTEST_WRITE | READ_DEVICE_DATE ,
    READ_DEVICE_TIME = 0x06,
    SET_DEVICE_TIME =TOPTEST_WRITE | READ_DEVICE_TIME,
    READ_PORT_TEMPERATURE = 0x07,
    READ_DEVICE_INNER_TEMPERATURE = 0x08,
    READ_DEVICE_STARTMODE = 0x09,
    SET_DEVICE_STARTMODE = TOPTEST_WRITE | READ_DEVICE_STARTMODE,
    READ_DEVICE_STRAT_DELAY_TIME = 0x0a,
    SET_DEVICE_STRAT_DELAY_TIME = TOPTEST_WRITE | READ_DEVICE_STRAT_DELAY_TIME,
    READ_TEMPERATURE_STRAT_VALUE = 0x0b,
    SET_TEMPERATURE_STRAT_VALUE = TOPTEST_WRITE | READ_TEMPERATURE_STRAT_VALUE,
    READ_TEST_DEVICE_DATE = 0x0c,
    READ_TEST_DEVICE_TIME = 0x0d,
};

FT_stringEnum(TopTestStartMode,
    Direct,
    DelayByTime,
    DelayByTemputure
)

FT_stringEnum(TopTestStopMode,
    DirectStop,
    DelayByTimeAndTemputure,
)

FT_stringEnum(ProgressType,
    MaxTemputure,
    IncreaseTime,
    AboveTime,
    DecreaseTime,
    IncreaseRatio,
    DecreaseRatio,
    MaxIncreaseRatio,
    MaxDecreaseRatio,
    AverageRatio,
    DistanceToMaxTimeSection,
    TemputureBetweenTime,
    MaxIncreaseTime,
    MaxDecreaseTime,
    MaxPortMaxDeltaValue,
    TestValuedTime
)

}

#endif // TOPTESTENUM_H
