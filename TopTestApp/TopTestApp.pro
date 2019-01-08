#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T20:44:39
#
#-------------------------------------------------

QT       += core gui quick printsupport charts network serialport remoteobjects

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR+=../bin

TARGET = TopTestApp
TEMPLATE = app

QMAKE_POST_LINK += windeployqt.exe $$OUT_PWD/../bin/TopTestApp.exe

#include($$PWD/../../ThirdParty/boost166.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_ICONS = global.ico

SOURCES += main.cpp\
        toptestmainwindow.cpp \
    toptestapplication.cpp \
    clabsotoolbutton.cpp \
    formmoduletab.cpp \
    mainwindow.cpp \
    rsmmainwindow.cpp \
    formnavibar.cpp \
    formpagehome.cpp \
    formrsmtoolbuttongroup.cpp \
    formpagedatavisual.cpp \
    formlogin.cpp \
    rsmcoresignalandslotcontrolcenter.cpp \
    dialogharddevicesetting.cpp \
    listbuddy.cpp \
    buttonlist.cpp \
    formdatachart.cpp \
    formresulttable.cpp \
    formdeviceinfotable.cpp \
    dialogdatadownload.cpp \
    formfilesetting.cpp \
    formfilesettingbasewidget.cpp \
    formproductcontent.cpp \
    formdevicecontent.cpp \
    formprogresshandle.cpp \
    formdatapage.cpp \
    dialogsavedatafile.cpp \
    dialogopenfile.cpp \
    formportdefinepic.cpp \
    formdraglabel.cpp \
    dialogfilesettingchoose.cpp \
    dialogportdefine.cpp \
    formdialogheader.cpp \
    dialogbase.cpp \
    formdialog.cpp \
    dialogsystemsetting.cpp \
    dialogcurrentlinkdevices.cpp \
    formdevicetable.cpp \
    formfileinfo.cpp \
    dialogprintoutput.cpp \
    dialogexceldataoutput.cpp \
    formpagerealtimemonitor.cpp \
    dialogchartaxissetting.cpp \
    formchartaxissetting.cpp \
    formcustermplotchart.cpp \
#    ../qcustomplot/qcustomplot.cpp \
    commonhelper.cpp \
    formlinemanager.cpp \
    formlinksetting.cpp \
    formsettingfileexchange.cpp \
    formexchangecontent.cpp \
    pdfwriter.cpp \
    dialoghandlewave.cpp

HEADERS  += toptestmainwindow.h \
    toptestapplication.h \
    clabsotoolbutton.h \
    formmoduletab.h \
    mainwindow.h \
    rsmmainwindow.h \
    formnavibar.h \
    formpagehome.h \
    formrsmtoolbuttongroup.h \
    formpagedatavisual.h \
    formlogin.h \
    rsmcoresignalenum.h \
    rsmcoresignalandslotcontrolcenter.h \
    dialogharddevicesetting.h \
    listbuddy.h \
    buttonlist.h \
    formdatachart.h \
    formresulttable.h \
    formdeviceinfotable.h \
    dialogdatadownload.h \
    formfilesetting.h \
    formfilesettingbasewidget.h \
    formproductcontent.h \
    formdevicecontent.h \
    formprocesscontent.h \
    formprogresshandle.h \
    formdatapage.h \
    dialogsavedatafile.h \
    dialogopenfile.h \
    formportdefinepic.h \
    formdraglabel.h \
    dialogfilesettingchoose.h \
    dialogportdefine.h \
    formdialogheader.h \
    dialogbase.h \
    formdialog.h \
    dialogsystemsetting.h \
    dialogcurrentlinkdevices.h \
    formdevicetable.h \
    formfileinfo.h \
    dialogprintoutput.h \
    dialogexceldataoutput.h \
    formpagerealtimemonitor.h \
    dialogchartaxissetting.h \
    formchartaxissetting.h \
    formcustermplotchart.h \
#    ../qcustomplot/qcustomplot.h \
    commonhelper.h \
    formlinemanager.h \
    formlinksetting.h \
    formsettingfileexchange.h \
    formexchangecontent.h \
    pdfwriter.h \
    dialoghandlewave.h

FORMS    += toptestmainwindow.ui \
    formmoduletab.ui \
    mainwindow.ui \
    rsmmainwindow.ui \
    formnavibar.ui \
    formpagehome.ui \
    formpagedatavisual.ui \
    formlogin.ui \
    dialogharddevicesetting.ui \
    formdatachart.ui \
    formresulttable.ui \
    formdeviceinfotable.ui \
    dialogdatadownload.ui \
    formfilesetting.ui \
    formfilesettingbasewidget.ui \
    formproductcontent.ui \
    formdevicecontent.ui \
    formprogresshandle.ui \
    formdatapage.ui \
    dialogsavedatafile.ui \
    dialogopenfile.ui \
    formportdefinepic.ui \
    dialogfilesettingchoose.ui \
    dialogportdefine.ui \
    formdialogheader.ui \
    formdialog.ui \
    dialogsystemsetting.ui \
    dialogcurrentlinkdevices.ui \
    formfileinfo.ui \
    dialogprintoutput.ui \
    dialogexceldataoutput.ui \
    formpagerealtimemonitor.ui \
    dialogchartaxissetting.ui \
    formcustermplotchart.ui \
    formlinemanager.ui \
    formlinksetting.ui \
    formexchangecontent.ui \
    dialoghandlewave.ui

RESOURCES += \
    toptest.qrc

INCLUDEPATH+=\
    ../TopTestCore\
    ../TopTestWidgets\
    ../TopTestUsb\
    ../TopTestActions\
    ../FipsItemDelegate\
    ../quicreator\

LIBS+=-L../lib\
    -lTopTestCore\
    -lTopTestWidgets\
    -lTopTestUsb\
    -lTopTestActions\
    -lFipsItemDelegate\
    -lquicreator\

DEPENDPATH+=\
    ../TopTestCore\
    ../TopTestWidgets\
    ../TopTestUsb\
    ../TopTestActions\
    ../quicreator\

LIBS+=-lDbgHelp

QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG

DISTFILES += \
    rsm.qss \
    test \
    clabso_ico.rc
