QT += testlib
QT -= gui

INCLUDEPATH +=\
    ../../TopTestCore\

LIBS += -L../../lib\
    -lTopTestCore\

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testdataio.cpp
