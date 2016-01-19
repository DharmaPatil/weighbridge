/**
* @file calibrationwidget.cpp
* @brief 曲线标定类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QtDebug>
#include <QToolBar>
#include <QtGui>

#include "calibrationframe.h"
#include "stdcurvedialog.h"
#include "dynamiccalibrationwidget.h"
#include "staticcalibrationwidget.h"
#include "analysistemplate.h"
#include "stdcurvemanager.h"
#include "wbaction.h"
#include "workbench.h"

static void mapAction(QAction *action, QSignalMapper *mapper, int id)
{
    mapper->setMapping(action, id);
    QObject::connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
}

CalibrationFrame::CalibrationFrame(Workbench *wb, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(wb)
{
    m_toolbar = createFixedToolbar();

    WBAction *actions = m_workbench->actionManager();
    m_toolbar->addAction(actions->calibrationMethodAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->startCalibrateAction);
    m_toolbar->addAction(actions->pauseCalibrateAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->validateCalibrationAction);
    m_toolbar->addAction(actions->registerCalibrationAction);
    m_toolbar->addAction(actions->resetCalibrationAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->modifyCalibrationAction);

  //  m_curveWidget = new StdCurveWidget(m_workbench, this);

    QSignalMapper *mapper = new QSignalMapper(this);
    QMenu *menu = new QMenu;
    mapAction(menu->addAction(tr("动态标定")), mapper, 0);
    mapAction(menu->addAction(tr("静态标定")), mapper, 1);
    actions->calibrationMethodAction->setMenu(menu);
    m_toolbar->toolButtonForAction(actions->calibrationMethodAction)->setPopupMode(QToolButton::InstantPopup);


    m_caliWidget = new QStackedWidget;
    m_caliWidget->addWidget(new DynamicCalibrationWidget(wb, this));
    m_caliWidget->addWidget(new StaticCalibrationWidget(wb, this));
    m_caliWidget->setCurrentIndex(0);

    setCentralWidget(m_caliWidget);

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(changeCaliMethod(int)));
    connect(actions->modifyCalibrationAction, SIGNAL(triggered()), this, SLOT(modifyCurve()));
}

CalibrationFrame::~CalibrationFrame()
{

}

void CalibrationFrame::changeCurve(QString &name)
{

}

void CalibrationFrame::saveCurve()
{

}

void CalibrationFrame::resetCurve()
{

}

void CalibrationFrame::modifyCurve()
{
    StdCurveDialog dlg(workbench(), this);
    dlg.exec();
}

void CalibrationFrame::changeCaliMethod(int i)
{
    m_caliWidget->setCurrentIndex(i);
}
