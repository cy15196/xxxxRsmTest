TEMPLATE = subdirs
CONFIG += ORDERED
SUBDIRS += \
    TopTestCore \
    TopTestUsb \
    FipsItemDelegate \
    LibModbus\
    TopTestActions \
    TopTestWidgets \
    TestProject \
    TopTestApp \
    RSMConfigure \
    InstllerEmptyProject\
    quicreator \
    TopTestPortManager

TRANSLATIONS +=\
    Rsm_zh_CN.ts\
    Rsm_en.ts \
    Rsm_zh_TW.ts

DISTFILES += \
    Rsm_en.ts \
    Rsm_zh_CN.ts \
    Rsm_zh_TW.ts \
    Rsm_en.qm \
    Rsm_zh_CN.qm\
    Rsm_zh_TW.qm

OTHER_FILES += \
    Rsm_en.ts \
    Rsm_zh_CN.ts \
    Rsm_zh_TW.ts \
    Rsm_en.qm \
    Rsm_zh_CN.qm\
    Rsm_zh_TW.qm

#1.在线监控软件，电脑休眠/睡眠 再启动后，监控页面增加了无效的数据
#1.1 将实时监控窗口和数据下载的槽函数做成互斥的
#2.一个软件连接硬件情况下，另外一个软件尝试连接时无限提示打开端口失败
#3.切换不同的硬件，在线监控页面需要重新读取仪器的通道数
#4.软件运行一段时间后，切换连接非常缓慢
#5.输出数据时时间
#6.仿真运行的标志位

