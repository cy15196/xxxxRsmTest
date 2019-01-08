#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T20:05:10
#
#-------------------------------------------------

QT       += core  gui widgets serialport
QT       += remoteobjects

TARGET = TopTestUsb
TEMPLATE = lib

#include(H:/Qt/QtCode/ThirdParty/TopTest.pri)

DESTDIR+=../lib
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin &
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".pdb" ..\bin

INCLUDEPATH +=\
    ../TopTestCore\

LIBS += -L../lib\
    -lTopTestCore\


DEFINES += TOPTESTUSB_LIBRARY

#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        toptestusb.cpp \
    serialporttransfer.cpp \
    rsmusb.cpp \
    mythread.cpp \
    directconnet.cpp

HEADERS += \
        toptestusb.h \
        toptestusb_global.h \ 
    serialporttransfer.h \
    rsmusb.h \
    mythread.h \
    directconnet.h

OTHER_FILES += \
        USBtestDlg.h\
        USBtestDlg.cpp\

unix {
    target.path = /usr/lib
    INSTALLS += target
}

REPC_REPLICA = ../TopTestPortManager/port.rep

###需要调试QSerialPort通信超时的错误


