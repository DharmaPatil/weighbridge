/**
* @file dynamiccalibrationwidget.cpp
* @brief 动态标定窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-14
*/
#include <QtGui>
#include <QtDebug>

#include "dynamiccalibrationwidget.h"
#include "calibrationdocmodel.h"
#include "calibrationdoc.h"
#include "profile.h"
#include "global.h"
#include "workbench.h"

DynamicCalibrationWidget::DynamicCalibrationWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
{
    m_dynDoc = workbench()->calibrationDoc();
    m_dynDocModel = new CalibrationDocModel(m_dynDoc, this);

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_dynDocModel);

    QHBoxLayout *mainLayout  = new QHBoxLayout;
    mainLayout->addWidget(m_tableView);
    setLayout(mainLayout);
}

DynamicCalibrationWidget::~DynamicCalibrationWidget()
{

}

