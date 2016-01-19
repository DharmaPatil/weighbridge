/**
* @file qcframe.h
* @brief 质控界面定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#ifndef QCFRAME_H
#define QCFRAME_H

#include <QWidget>
#include "mainwindowbase.h"

class Workbench;
class QcWidget;
class WBAction;

class QcFrame : public MainWindowBase
{
    Q_OBJECT

public:
    QcFrame(Workbench *wb, QWidget *parent = 0);
    ~QcFrame();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void selectFile();
    void print();
    void prevPage();
    void nextPage();
    void prevData();
    void nextData();

private:
    Workbench *m_workbench;
    QcWidget *m_qcWidget;
    ToolBar *toolbar;
    WBAction *actions;

};

#endif // QCFRAME_H
