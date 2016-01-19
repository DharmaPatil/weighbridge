#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T09:55:26
#
#-------------------------------------------------

TARGET = core
TEMPLATE = lib

include(../base.pri)

QT       += sql script declarative

INCLUDEPATH += \
../common \
../qwt \
../Report

LIBS += -lcommon -lqwt -lreport

DEFINES += CORE_LIBRARY

SOURCES += core.cpp \
    abstractanalysisdoc.cpp \
    abstractanalysisdocmodel.cpp \
    analysisdoc.cpp \
    analysisdocmodel.cpp \
    analysistemplate.cpp \
    calibrationdoc.cpp \
    calibrationmanager.cpp \
    calibrationparam.cpp \
    datastorage.cpp \
    instrument.cpp \
    instrumentfactory.cpp \
    leastsquares.cpp \
    linearregression.cpp \
    mainwindow.cpp \
    mainwindowbase.cpp \
    measuretemplate.cpp \
    measuretemplatemanager.cpp \
    pointdata.cpp \
    qcdoc.cpp \
    regression.cpp \
    reportprinter.cpp \
    sample.cpp \
    siminstrument.cpp \
    standardcurve.cpp \
    stdcurvemanager.cpp \
    stdcurveplot.cpp \
    wbaction.cpp \
    weightdoc.cpp \
    workbench.cpp \
    analysistemplatemanager.cpp \
    instrumentplugin.cpp \
    instrtypedialog.cpp \
    wbapp.cpp \
    appwindow.cpp \
    logindialog.cpp \
    messagewidget.cpp \
    standardcurvemodel.cpp \
    analysisframe.cpp \
    databasemaintainframe.cpp \
    dataresultframe.cpp \
    dataresultwidget.cpp \
    logdataframe.cpp \
    weightframe.cpp \
    weightwidget.cpp \
    weightdocmodel.cpp \
    analysiswidget.cpp \
    samplecurveplot.cpp \
    statusplot.cpp \
    staticweightwidget.cpp \
    qcwidget.cpp \
    qcframe.cpp \
    calibrationdocmodel.cpp \
    dynamiccalibrationwidget.cpp \
    staticcalibrationwidget.cpp \
    calibrationframe.cpp \
    stdcurvedialog.cpp \
    optionframe.cpp \
    optionwidget.cpp \
    customdefinepagewidget.cpp \
    systemoperationwidget.cpp \
    systemsetuppagewidget.cpp \
    blink.cpp \
    networkconfigwidget.cpp \
    generalparamwidget.cpp \
    gainparamwidget.cpp

HEADERS += core.h\
        core_global.h \
    abstractanalysisdoc.h \
    abstractanalysisdocmodel.h \
    analysisdoc.h \
    analysisdocmodel.h \
    analysistemplate.h \
    calibrationdoc.h \
    calibrationmanager.h \
    calibrationparam.h \
    datastorage.h \
    instrparamheader.h \
    instrument.h \
    instrumentfactory.h \
    leastsquares.h \
    linearregression.h \
    mainwindow.h \
    mainwindowbase.h \
    measuretemplate.h \
    measuretemplatemanager.h \
    pointdata.h \
    qcdoc.h \
    regression.h \
    reportprinter.h \
    sample.h \
    siminstrument.h \
    standardcurve.h \
    stdcurvemanager.h \
    stdcurveplot.h \
    wbaction.h \
    weightdoc.h \
    workbench.h \
    analysistemplatemanager.h \
    instrumentplugin.h \
    instrtypedialog.h \
    wbapp.h \
    appwindow.h \
    logindialog.h \
    messagewidget.h \
    standardcurvemodel.h \
    analysisframe.h \
    databasemaintainframe.h \
    dataresultframe.h \
    dataresultwidget.h \
    logdataframe.h \
    weightframe.h \
    weightwidget.h \
    weightdocmodel.h \
    analysiswidget.h \
    samplecurveplot.h \
    statusplot.h \
    staticweightwidget.h \
    qcwidget.h \
    qcframe.h \
    calibrationdocmodel.h \
    dynamiccalibrationwidget.h \
    staticcalibrationwidget.h \
    calibrationframe.h \
    stdcurvedialog.h \
    optionframe.h \
    optionwidget.h \
    customdefinepagewidget.h \
    systemsetuppagewidget.h \
    systemoperationwidget.h \
    blink.h \
    networkconfigwidget.h \
    generalparamwidget.h \
    gainparamwidget.h

win32{
        LIBS += -lAdvapi32
        RC_FILE = core.rc
}

FORMS += \
    mainwindow.ui \
    stdcurvewidget.ui \
    instrtypedialog.ui \
    logindialog.ui \
    messagewidget.ui \
    operationwidget.ui \
    networkconfigwidget.ui \
    generalparamwidget.ui \
    gainparamwidget.ui

RESOURCES += \
    core.qrc

DISTFILES += \
    Resources/static.png

