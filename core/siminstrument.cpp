/**
* @file siminstrument.cpp
* @brief 模拟仪器类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include <QtCore>
#include <QTime>
#include <math.h>
#include "global.h"
#include "siminstrument.h"

SimInstrument::SimInstrument(Workbench *parent)
    : Instrument(parent)
    , m_timer(new QTimer(this))
{

}

SimInstrument::~SimInstrument()
{

}

bool SimInstrument::start()
{
    return true;
}

void SimInstrument::pause()
{

}

void SimInstrument::end()
{

}

void SimInstrument::setParam(const InstrParamHeader *pa)
{
    pa = pa;
}

QIODevice *SimInstrument::openParamFile(QIODevice::OpenMode mode)
{
    Q_UNUSED(mode)
    return 0;
}

void SimInstrument::timeout()
{

}
