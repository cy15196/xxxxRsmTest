#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T12:55:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET              = quicreator
TEMPLATE            = lib
MOC_DIR             = temp/moc
RCC_DIR             = temp/rcc
UI_DIR              = temp/ui
OBJECTS_DIR         = temp/obj
#win32:RC_FILE       = other/main.rc
PRECOMPILED_HEADER  = head.h

DESTDIR             +=../lib
QMAKE_POST_LINK     += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin
DEFINES             += QUICREATOR_LIBRARY TOPTEST

INCLUDEPATH         += $$PWD
CONFIG              += warn_off
DEFINES             += demo1

OTHER_FILES             += main.cpp
SOURCES             += framelesshelper.cpp
SOURCES             += quiwidget.cpp
SOURCES             += quicreator.cpp
SOURCES             += highlighter.cpp
SOURCES             += panelwidget.cpp
SOURCES             += frmdevice.cpp
SOURCES             += appinit.cpp

HEADERS             += head.h
HEADERS             += framelesshelper.h
HEADERS             += quiwidget.h
HEADERS             += quicreator.h
HEADERS             += highlighter.h
HEADERS             += panelwidget.h
HEADERS             += frmdevice.h
HEADERS             += appinit.h
HEADERS             += quicreator_global.h

FORMS               += quicreator.ui
FORMS               += frmdevice.ui

RESOURCES           += other/qss.qrc
RESOURCES           += other/main.qrc
