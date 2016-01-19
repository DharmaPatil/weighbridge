/**
* @file dataresultwidget.h
* @brief 数据结果查询界面类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef DATARESULTWIDGET_H
#define DATARESULTWIDGET_H

#include <QWidget>

class SqlTableModel;
class QSqlRecord;
class QAciton;
class Workbench;

class DataResultWidget : public QWidget
{
public:
    explicit DataResultWidget(Workbench *workbench, QWidget *parent = 0);
    ~DataResultWidget();

private:
    Workbench *m_workbench;
};

#endif // DATARESULTWIDGET_H
