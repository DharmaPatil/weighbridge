#include <QDebug>
#include <QHeaderView>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>

#include "weightframe.h"
#include "weightwidget.h"
#include "workbench.h"
#include "profile.h"
#include "wbaction.h"
#include "instrument.h"

WeightFrame::WeightFrame(Workbench *wb, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(wb)
    , m_modeFlag(true)
    , m_startFlag(false)
    , m_pauseFlag(false)
{
    m_toolbar = createFixedToolbar();

    actions = this->workbench()->actionManager();
    m_toolbar->addAction(actions->measureModeAction);
    m_toolbar->addAction(actions->startMeasureAction);
    m_toolbar->addAction(actions->pauseMeasureAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->reproduceAction);
    m_toolbar->addAction(actions->drawCurveAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(actions->printRefPointAction);
    m_toolbar->addAction(actions->printResultAction);

    m_weightWidget = new WeightWidget(wb, parent);
    setCentralWidget(m_weightWidget);

    connect(actions->measureModeAction, SIGNAL(triggered()),
            this, SLOT(onManualTest()));
    connect(actions->startMeasureAction, SIGNAL(triggered()),
            this, SLOT(onStartMeasure()));
    connect(actions->pauseMeasureAction, SIGNAL(triggered()),
            this, SLOT(onPauseMeasure()));
    connect(actions->reproduceAction, SIGNAL(triggered()),
            this, SLOT(onReprodduce()));

    btnRender();
}

WeightFrame::~WeightFrame()
{

}

void WeightFrame::btnRender()
{
    if (m_modeFlag)
    {
        m_startFlag = false;
        m_pauseFlag = false;
    }
    else
    {
        m_startFlag = true;
        m_pauseFlag = false;
    }
//    actions->measureModeAction->setEnabled(m_modeFlag);
    actions->startMeasureAction->setEnabled(m_startFlag);
    actions->pauseMeasureAction->setEnabled(m_pauseFlag);
}

void WeightFrame::onManualTest()
{
    m_modeFlag = !m_modeFlag;

    if (!m_modeFlag)
        actions->measureModeAction->setText(tr("手动"));
    else
        actions->measureModeAction->setText(tr("自动"));

    btnRender();
}

void WeightFrame::updateStatus()
{

}

void WeightFrame::onStartMeasure()
{
    if (m_pauseFlag)
    {
        m_pauseFlag = false;

    }
    m_startFlag = true;

    btnRender();
}

void WeightFrame::onPauseMeasure()
{

}

void WeightFrame::onReprodduce()
{
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("Open Weight Sample File"));
    fileDialog.setDirectory(".");
    fileDialog.setFilter(tr("Data Files(*.dat *.raw *.bin *.flt *.idx *.txt)"));
    if(fileDialog.exec() == QDialog::Accepted)
    {
        m_openPath = fileDialog.selectedFiles()[0];
    }
    else
    {

    }
}
