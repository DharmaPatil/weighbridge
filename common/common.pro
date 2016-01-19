#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T11:01:56
#
#-------------------------------------------------

TARGET = common
TEMPLATE = lib

QT       += sql script

include(../base.pri)

DEFINES += COMMON_LIBRARY

SOURCES += common.cpp \
    analysisresult.cpp \
    global.cpp \
    profile.cpp \
    serial.cpp \
    machineconfig.cpp

HEADERS += common.h\
        common_global.h \
    analysisresult.h \
    global.h \
    profile.h \
    serial.h \
    machineconfig.h

PRECOMPILED_HEADER =

win32 {
        LIBS += -lAdvapi32
        RC_FILE = common.rc
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
