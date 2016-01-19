/**
* @file dataresultframe.h
* @brief 数据结果查询框架类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef DATARESULTFRAME_H
#define DATARESULTFRAME_H

#include "mainwindowbase.h"
#include <QSqlTableModel>

class Workbench;
class QSplitter;
class QTableView;

class DataResultFrame : public MainWindowBase
{
    Q_OBJECT

public:
    explicit DataResultFrame(Workbench *workbench, QWidget *parent = 0);
    ~DataResultFrame();

    Workbench *workbench() { return m_workbench; }

private:

private:
    Workbench *m_workbench;

    QSqlTableModel *m_briefModel;
    QSqlTableModel *m_detailModel;
    QTableView *m_briefView;
    QTableView *m_detailView;
    QSplitter *m_splitter;
};

#endif // DATARESULTFRAME_H
