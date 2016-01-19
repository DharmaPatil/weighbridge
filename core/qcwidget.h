/**
* @file qcwidget.h
* @brief 质控窗口类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#ifndef QCWIDGET_H
#define QCWIDGET_H

#include <QWidget>

class Workbench;

class QcWidget : public QWidget
{
    Q_OBJECT

public:
    QcWidget(Workbench *wb, QWidget *parent = 0);
    ~QcWidget();
    
    Workbench *workbench() const { return m_workbench; }
    
private:
    Workbench *m_workbench;
};

#endif // QCWIDGET_H
