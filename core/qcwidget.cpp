/**
* @file qcwidget.cpp
* @brief 质控界面窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#include <QtGui>
#include <QDebug>

#include "qcwidget.h"
#include "workbench.h"
#include "profile.h"

QcWidget::QcWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
{

}

QcWidget::~QcWidget()
{

}

