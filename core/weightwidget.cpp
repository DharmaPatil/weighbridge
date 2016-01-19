/**
* @file weightwidget.cpp
* @brief 轨道衡称重主界面类实现
* @ingroup ui
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include <QtGui>
#include <QDebug>
#include <QHeaderView>

#include "weightdoc.h"
#include "weightdocmodel.h"
#include "workbench.h"
#include "profile.h"
#include "instrument.h"
#include "wbapp.h"
#include "weightwidget.h"
#include "statusplot.h"


///////////////////////////////////////////////////////////////////////
/// \brief WeightWidget class
///
WeightWidget::WeightWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
{
    setupUi();
    init();

}

WeightWidget::~WeightWidget()
{

}

void WeightWidget::setupUi()
{
    QGridLayout *grid = new QGridLayout(this);
    for (int i = 0; i < ChannelMax; ++i)
    {
        m_refPoint[i] = new QLineEdit;
        m_refPoint[i]->setMinimumSize(50, 30);
        m_refPoint[i]->setText(QString());
        grid->addWidget(m_refPoint[i], i / 6, i % 6);
    }

    QGroupBox *refGbx = new QGroupBox(tr("传 感 器 零 点"));
    refGbx->setAlignment(Qt::AlignHCenter);
    refGbx->setLayout(grid);

    m_plot = new StatusPlot;

    QHBoxLayout *statusLayout = new QHBoxLayout;
    statusLayout->addWidget(refGbx);
    statusLayout->addWidget(m_plot);

    m_doc = workbench()->weightDoc();
    m_model = new WeightDocModel(m_doc, this);
 //   m_model->setDoc(m_doc);
    connect(m_doc, SIGNAL(insertResult(QStringList&)),
            m_model, SLOT(insertData(QStringList&)));
    connect(m_doc, SIGNAL(insertSampleData(QList<int>&,QList<int>&)),
            m_model, SLOT(onAddSample(QList<int>&, QList<int>&)));

    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    QHBoxLayout *resultLayout = new QHBoxLayout;
    resultLayout->addWidget(m_view);

    QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(statusLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(resultLayout);

    setLayout(mainLayout);

    connect(m_doc, SIGNAL(insertSynthsisData(QList<int> &)),
            m_plot, SLOT(setData(QList<int>&)));

}

void WeightWidget::init()
{

}
