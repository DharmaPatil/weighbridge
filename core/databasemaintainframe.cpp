/**
* @file databasemaintainframe.cpp
* @brief 数据库维护界面类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include "databasemaintainframe.h"
#include "workbench.h"
#include "wbaction.h"

DatabaseMaintainFrame::DatabaseMaintainFrame(Workbench *workbench, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(workbench)
{
    m_toolbar = createFixedToolbar();

    WBAction *actions = this->workbench()->actionManager();
    m_toolbar->addActions(actions->databaseActions->actions());
}

DatabaseMaintainFrame::~DatabaseMaintainFrame()
{

}

