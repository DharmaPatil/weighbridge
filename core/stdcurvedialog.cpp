#include <QDebug>
#include <QPen>
#include <QSet>
#include <QMenu>
#include <QAction>

#include "stdcurvedialog.h"
#include "workbench.h"
#include "analysistemplate.h"
#include "standardcurve.h"
#include "standardcurvemodel.h"
#include "stdcurvemanager.h"
#include "profile.h"
#include "stdcurveplot.h"

StdCurveDialog::StdCurveDialog(Workbench *wb, QWidget *parent)
    : QDialog(parent)
    , m_workbench(wb)
    , m_assayTemplate(0)
    , m_currentCurve(0)
    , m_newCurve(0)
    , m_viewFlags(CV_Both)
{
    ui.setupUi(this);
    ui.bothCurve->setChecked(true);

	m_curveModel = new StandardCurveModel(this);
	ui.curve->setModel(m_curveModel);
	ui.curve->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

StdCurveDialog::~StdCurveDialog()
{

}

void StdCurveDialog::setCurve(const QString &curve)
{

}

void StdCurveDialog::setViewFlags(int flags)
{

}

void StdCurveDialog::modifyCurve()
{

}

void StdCurveDialog::showBoth()
{

}

void StdCurveDialog::showCurrent()
{

}

void StdCurveDialog::showNew()
{

}

void StdCurveDialog::onCurveStatusChanged(const QString &curve)
{

}

void StdCurveDialog::refresh(int viewFlags)
{

}

void StdCurveDialog::onCurveDestroyed(QObject *)
{

}

void StdCurveDialog::updateStatistic()
{

}

void StdCurveDialog::onViewClicked(const QModelIndex &index)
{

}

void StdCurveDialog::popupCurveMenu(const QPoint &pos)
{

}

void StdCurveDialog::on_x_textChanged(const QString &text)
{

}
