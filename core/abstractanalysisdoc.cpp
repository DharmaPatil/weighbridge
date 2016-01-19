/**
* @file abstractanalysisdoc.cpp
* @brief 轨道衡测量分析结果数据组织基类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/

#include <QMessageBox>
#include <QAbstractButton>
#include <QTimerEvent>

#include "global.h"
#include "abstractanalysisdoc.h"
#include "workbench.h"
#include "analysistask.h"

AbstractAnalysisDoc::AbstractAnalysisDoc(Workbench *parent)
    : QObject(parent)
    , m_workbench(parent)
    , m_msgBox(0)
{

}

AbstractAnalysisDoc::~AbstractAnalysisDoc()
{

}

void AbstractAnalysisDoc::start(bool showLayout)
{
    Q_UNUSED(showLayout)
}

void AbstractAnalysisDoc::pause()
{

}

void AbstractAnalysisDoc::onTaskStatusChanged(weighbridge::AnalysisStatus s)
{
    Q_UNUSED(s)
}

void AbstractAnalysisDoc::onTaskFinished()
{

}

void AbstractAnalysisDoc::onSaveSampleData(QString &filename)
{
    Q_UNUSED(filename)
}

void AbstractAnalysisDoc::onAddSampleData(QList<int> &data)
{

}

int AbstractAnalysisDoc::filter(QList<int> &data, int index)
{
	return 0;
}

int AbstractAnalysisDoc::diff(QList<int> data, int index)
{
	return 0;
}
