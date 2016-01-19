TEMPLATE	= app
TARGET		= ReportDesigner

include(../base.pri)

QT	+= sql

INCLUDEPATH += ../Report

#PRECOMPILED_HEADER = stable.h

LIBS +=	-lreport

include(ReportDesigner.pri)

win32:RC_FILE = ReportDesigner.rc

