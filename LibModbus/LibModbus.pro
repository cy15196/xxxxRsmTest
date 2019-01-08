#-------------------------------------------------
#
# Project created by QtCreator 2018-08-24T22:14:02
#
#-------------------------------------------------

QT       -= gui

TARGET = LibModbus
TEMPLATE = lib

DEFINES += LIBMODBUS_LIBRARY DLLBUILD
DESTDIR+=../lib

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
        libmodbus.cpp \
    libmodbus.cpp \
    modbus.c \
    modbus-data.c \
    modbus-rtu.c \
    modbus-tcp.c

HEADERS += \
        libmodbus.h \
        libmodbus_global.h \ 
    config.h \
    libmodbus.h \
    libmodbus_global.h \
    modbus.h \
    modbus-private.h \
    modbus-rtu.h \
    modbus-rtu-private.h \
    modbus-tcp.h \
    modbus-tcp-private.h \
    modbus-version.h \
#    modbus-version.h.in

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    LibModbus.pro

DISTFILES += \
    config.h.win32

LIBS +=\
 -lws2_32
