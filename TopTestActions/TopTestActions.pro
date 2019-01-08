#-------------------------------------------------
#
# Project created by QtCreator 2018-05-20T16:50:26
#
#-------------------------------------------------
QT += core gui widgets

TARGET = TopTestActions
TEMPLATE = lib

DESTDIR+=../lib
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin

DEFINES += TOPTESTACTIONS_LIBRARY

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
    clabsoenablehelper.cpp \
    newaction.cpp \
    openaction.cpp \
    systemsettingaction.cpp \
    showmonitoraction.cpp

HEADERS += \
        toptestactions.h \
        toptestactions_global.h \ 
    clabsoenablehelper.h \
    newaction.h \
    openaction.h \
    systemsettingaction.h \
    showmonitoraction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


INCLUDEPATH+=\
    ../TopTestCore\
    ../TopTestWidgets\
    ../TopTestUsb\

DEPENDPATH+=\
    ../TopTestCore\
    ../TopTestWidgets\
    ../TopTestUsb\

LIBS+=-L../lib\
    -lTopTestCore\
    -lTopTestWidgets\
    -lTopTestUsb
