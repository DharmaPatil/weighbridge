TEMPLATE = lib
TARGET = report

include(../base.pri)

QT		+= sql

DEFINES += REPORT_LIB
INCLUDEPATH += ../common

#PRECOMPILED_HEADER =

LIBS +=	-lcommon

include(Report.pri)

win32:RC_FILE = report.rc
