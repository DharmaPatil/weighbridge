#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T16:59:21
#
#-------------------------------------------------
TARGET = weighbridge
TEMPLATE = app

include(../base.pri)

INCLUDEPATH += ../core ../common
LIBS += -lcore -lzgu100sgy

win32:RC_FILE = wbapp.rc

SOURCES += \
    main.cpp

OTHER_FILES += \
readme.txt \
settings.ini \
wbapp.rc

HEADERS +=

RESOURCES +=
