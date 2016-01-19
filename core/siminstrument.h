/**
* @file siminstrument.h
* @brief 模拟仪器类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef SIMINSTRUMENT_H
#define SIMINSTRUMENT_H

#include "instrument.h"

class QTimer;
class Workbench;

class SimInstrument : public Instrument
{
    Q_OBJECT

public:
    SimInstrument(Workbench *parent = 0);
    ~SimInstrument();

    virtual bool start();
    virtual void pause();
    virtual void end();

    virtual const InstrParamHeader *param() const { return 0; }
    virtual void setParam(const InstrParamHeader *pa);
    QIODevice *openParamFile(QIODevice::OpenMode mode);

private slots:
    void timeout();

private:
    QTimer *m_timer;
};

#endif // SIMINSTRUMENT_H
