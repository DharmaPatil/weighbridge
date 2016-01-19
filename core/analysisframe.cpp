/**
* @file dataresultframe.cpp
* @brief 数据模拟仿真框架类实现
* @ingroup ui
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include "analysisframe.h"
#include "workbench.h"
#include "analysiswidget.h"
#include "wbaction.h"

AnalysisFrame::AnalysisFrame(Workbench *workbench, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(workbench)
{
    m_toolbar = createFixedToolbar();

    WBAction *actions = this->workbench()->actionManager();
    m_toolbar->addAction(actions->simuDrawingCurveAction);
    m_toolbar->addAction(actions->simuVertifyAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->simuPrevPageAction);
    m_toolbar->addAction(actions->simuNextPageAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->simuFirstPageAction);
    m_toolbar->addAction(actions->simuLastPageAction);

    m_widget = new AnalysisWidget(workbench, this);
    setCentralWidget(m_widget);
}

AnalysisFrame::~AnalysisFrame()
{

}

void AnalysisFrame::onManualTest()
{

}

void AnalysisFrame::updateStatus()
{

}
