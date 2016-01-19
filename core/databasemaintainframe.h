/**
* @file databasemaintainframe.h
* @brief 数据库维护类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef DATABASEMAINTAINFRAME_H
#define DATABASEMAINTAINFRAME_H

#include "mainwindowbase.h"

class Workbench;

class DatabaseMaintainFrame : public MainWindowBase
{
    Q_OBJECT

public:
    explicit DatabaseMaintainFrame(Workbench *workbench, QWidget *parent = 0);
    ~DatabaseMaintainFrame();

	Workbench *workbench() const { return m_workbench; }

private:
    Workbench *m_workbench;
    QToolBar *m_toolbar;
};

#endif // DATABASEMAINTAINFRAME_H
