#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T21:26:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = TopTestCore
TEMPLATE = lib

DESTDIR+=../lib
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin

DEFINES += TOPTESTCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += toptestcore.cpp \
    devicesetting.cpp \
    dataiomanager.cpp \
    dataprogresshandlealgorithm.cpp \
    clabso_ft_dpi.cpp \
    toptestdevice.cpp \
    clabso_ft_io.cpp \
    toptestsettingmanager.cpp \
    toptestfortranslate.cpp

HEADERS += toptestcore.h\
        toptestcore_global.h \
    devicesetting.h \
    toptestenum.h \
    dataiomanager.h \
    dataprogresshandlealgorithm.h \
    clabso_ft_dpi.h \
    topteststructdata.h \
    toptestdevice.h \
    clabso_ft_io.h \
    clabso_ft_macro.h \
    toptestsettingmanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS +=

#RESOURCES += \
#    deviceconfigure.qrc
