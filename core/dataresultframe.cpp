#include <QtDebug>
#include <QTableView>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QSqlQuery>
#include <QSplitter>
#include <QSqlRecord>
#include <QHBoxLayout>
#include <QAction>
#include <QPushButton>

#include "global.h"
#include "profile.h"
#include "datastorage.h"
#include "instrument.h"
#include "wbaction.h"
#include "workbench.h"
#include "dataresultframe.h"

DataResultFrame::DataResultFrame(Workbench *workbench, QWidget *parent)
    : MainWindowBase(parent)
    , m_workbench(workbench)
{
    WBAction *actions = workbench->actionManager();
    ToolBar *toolbar = createFixedToolbar();
    toolbar->addActions(actions->dataActions->actions());

    m_briefModel = new QSqlTableModel(this);
    m_briefView = new QTableView(this);
    m_briefView->setModel(m_briefModel);

    m_detailModel = new QSqlTableModel(this);
    m_detailView = new QTableView(this);
    m_detailView->setModel(m_detailModel);

    m_splitter = new QSplitter(Qt::Vertical);
    m_splitter->addWidget(m_briefView);
    m_splitter->addWidget(m_detailView);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 2);

    setCentralWidget(m_splitter);
}

DataResultFrame::~DataResultFrame()
{

}

