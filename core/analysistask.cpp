#include "global.h"
#include "analysistask.h"
#include "workbench.h"

AnalysisTask::AnalysisTask(Workbench *wb, QObject *parent)
    : QObject(parent)
    , m_workbench(wb)
{
    m_sample.clear();
    m_repeat = 1;
}

AnalysisTask::AnalysisTask(Workbench *wb, const QString &sample,  int repeat, QObject *parent)
    : QObject(parent)
    , m_workbench(wb)
    , m_sample(sample)
    , m_repeat(repeat)
{

}

AnalysisTask::~AnalysisTask()
{

}

