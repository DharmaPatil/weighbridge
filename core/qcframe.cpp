/**
* @file qcframe.cpp
* @brief 质控界面实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#include <QDebug>

#include "qcwidget.h"
#include "qcframe.h"
#include "workbench.h"
#include "wbaction.h"
#include "profile.h"
#include "wbapp.h"

QcFrame::QcFrame(Workbench *wb, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(wb)
{
    actions = workbench()->actionManager();
    toolbar = createFixedToolbar();
    toolbar->addActions(actions->qcActions->actions());
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolbar->setToolButtonStyle(actions->qcSelectGroupAction, Qt::ToolButtonTextBesideIcon);
    toolbar->setToolButtonStyle(actions->qcChangeScaleAction, Qt::ToolButtonTextBesideIcon);
    toolbar->setToolButtonStyle(actions->qcDisplayNewAction, Qt::ToolButtonTextBesideIcon);

    toolbar->setToolButtonAutoRepeat(actions->qcPrevDataAction);
    toolbar->setToolButtonAutoRepeat(actions->qcNextDataAction);
    toolbar->setToolButtonAutoRepeat(actions->qcPrevPageAction);
    toolbar->setToolButtonAutoRepeat(actions->qcNextPageAction);

    connect(actions->qcPrevDataAction,    SIGNAL(triggered()),   this, SLOT(prevData()));
    connect(actions->qcNextDataAction,    SIGNAL(triggered()),   this, SLOT(nextData()));
    connect(actions->qcPrevPageAction,    SIGNAL(triggered()),   this, SLOT(prevPage()));
    connect(actions->qcNextPageAction,    SIGNAL(triggered()),   this, SLOT(nextPage()));
    connect(actions->qcPrintAction,	      SIGNAL(triggered()),   this, SLOT(print()));
}

QcFrame::~QcFrame()
{

}

void QcFrame::selectFile()
{

}

void QcFrame::print()
{

}

void QcFrame::prevPage()
{

}

void QcFrame::nextPage()
{

}

void QcFrame::prevData()
{

}

void QcFrame::nextData()
{

}
