#-------------------------------------------------
#
# Project created by QtCreator 2017-06-16T15:10:48
#
#-------------------------------------------------

QT       += widgets

TARGET = FipsItemDelegate
TEMPLATE = lib

DEFINES += FIPSITEMDELEGATE_LIBRARY
DESTDIR+=../lib
QMAKE_POST_LINK += copy $$replace(DESTDIR,/,\\)\\$$TARGET".dll" ..\bin

SOURCES += \
    list_item_delegate.cpp \
    map_item_delegate.cpp \
    dimension_item_delegate.cpp \
    color_item_delegate.cpp \
    spin_item_delegate.cpp \
    color_list_item_delegate.cpp \
    assemble_item_delegate.cpp \
#    vector_item_delegate.cpp \
#    fipsitemdelegate.cpp \
    validator_item_delegate.cpp \
    presort_item_delegate.cpp \
    dependentcolumndelegate.cpp \
    checktoolbutton_item_delegate.cpp \
    line_color_item_delegate.cpp \
    port_choose_item_delegate.cpp

HEADERS +=\
        fipsitemdelegate_global.h \
    list_item_delegate.h \
    map_item_delegate.h \
    dimension_item_delegate.h \
    color_item_delegate.h \
    spin_item_delegate.h \
    color_list_item_delegate.h \
    assemble_item_delegate.h \
#    vector_item_delegate.h \
#    fipsitemdelegate.h \
    validator_item_delegate.h \
    presort_item_delegate.h \
    dependentcolumndelegate.h \
    checktoolbutton_item_delegate.h \
    line_color_item_delegate.h \
    templateedititemdelegate.h \
    port_choose_item_delegate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


#RESOURCES += \
#    resource_item.qrc
