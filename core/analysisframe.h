/**
* @file dataresultframe.h
* @brief 数据模拟仿真框架类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef ANALYSISFRAME_H
#define ANALYSISFRAME_H

#include "../core/mainwindowbase.h"

class Workbench;
class AnalysisWidget;

class AnalysisFrame : public MainWindowBase
{
    Q_OBJECT

public:
    AnalysisFrame(Workbench *workbench, QWidget *parent = 0);
    ~AnalysisFrame();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void onManualTest();
    void updateStatus();

private:
    Workbench *m_workbench;
    AnalysisWidget *m_widget;
    QToolBar *m_toolbar;
};

#endif // ANALYSISFRAME_H
