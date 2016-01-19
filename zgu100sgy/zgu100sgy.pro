#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T10:35:44
#
#-------------------------------------------------

QT       += sql

TARGET = zgu100sgy
TEMPLATE = lib

include(../base.pri)

DEFINES += ZGU100SGY_LIBRARY

INCLUDEPATH += \
        ../common \
        ../core \
        ../qwt

LIBS += \
        -lcommon \
        -lcore \
        -lqwt

SOURCES += zgu100sgy.cpp \
    workthreadzgu100s.cpp \
    zgu100sgyplugin.cpp \
    messagebuilder.cpp \
    connectionpage.cpp

HEADERS += zgu100sgy.h\
        zgu100sgy_global.h \
    workthreadzgu100s.h \
    zgu100sgyplugin.h \
    messagebuilder.h \
    connectionpage.h

FORMS += \
    connectionpage.ui

