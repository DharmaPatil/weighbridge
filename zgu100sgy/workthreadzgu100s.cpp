/**
* @file workthreadzgu100s.cpp
* @brief 轨道衡工作线程类实现
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#include "workthreadzgu100s.h"
#include "zgu100sgy.h"
#include "workbench.h"
#include "global.h"

WorkthreadZGU100S::WorkthreadZGU100S(Zgu100sgy *instr, QObject *parent)
    : QThread(parent)
    , m_instrument(instr)
{

}

WorkthreadZGU100S::~WorkthreadZGU100S()
{

}

Workbench *WorkthreadZGU100S::workbench() const
{
    return m_instrument->workbench();
}

void WorkthreadZGU100S::start()
{

}

void WorkthreadZGU100S::pause()
{

}

void WorkthreadZGU100S::stop()
{

}

void WorkthreadZGU100S::run()
{

}
