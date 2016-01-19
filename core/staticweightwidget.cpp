/**
* @file staticweightwidget.cpp
* @brief 静态称重窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-04
*/
#include <QtGui>
#include <QtDebug>

#include "global.h"
#include "profile.h"
#include "staticweightwidget.h"
#include "workbench.h"
#include "stdcurvemanager.h"
#include "wbaction.h"

StaticWeightWidget::StaticWeightWidget(Workbench *wb, QWidget *parent)
    : m_workbench(wb)
    , MainWindowBase(parent)
{
    m_actions = workbench()->actionManager();
    m_toolbar = createFixedToolbar();

    m_toolbar->addActions(m_actions->staticWeightActions->actions());

    setupUi();
    setCentralWidget(m_smWidget);

    connect(m_actions->saveResultAction, SIGNAL(triggered()),
            this, SLOT(onSaveResult()));
    connect(m_actions->printStaticResultAction, SIGNAL(triggered()),
            this, SLOT(onPrintResult()));
    connect(m_actions->queryStaticResultAction, SIGNAL(triggered()),
            this, SLOT(onQueryResult()));
    connect(m_recABBtn, SIGNAL(clicked()), this, SLOT(onRecordABWeight()));
    connect(m_recBCBtn, SIGNAL(clicked()), this, SLOT(onRecordBCWeight()));
    connect(m_plantformBtn[0], SIGNAL(clicked()), this, SLOT(onPlantformAZero()));
    connect(m_plantformBtn[1], SIGNAL(clicked()), this, SLOT(onPlantformBZero()));
    connect(m_plantformBtn[2], SIGNAL(clicked()), this, SLOT(onPlantformCZero()));

}

StaticWeightWidget::~StaticWeightWidget()
{

}

void StaticWeightWidget::createWidget()
{
    QString css = "font-size：32px";

    m_smWidget = new QWidget(this);

    m_recABBtn = new QPushButton(tr("AB台称重"));
    m_recABBtn->setFixedSize(75, 43);
    m_recABBtn->setStyleSheet(css);
    m_recBCBtn = new QPushButton(tr("BC台称重"));
    m_recBCBtn->setFixedSize(75, 43);
    m_recBCBtn->setStyleSheet(css);

    m_abLdt = new QLineEdit(QString());
    m_abLdt->setFixedSize(100, 40);
    m_bcLdt = new QLineEdit(QString());
    m_bcLdt->setFixedSize(100, 40);
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        m_pfmLdt[i] = new QLineEdit(QString());
        m_pfmLdt[i]->setFixedHeight(35);
    }

    m_leftRBtn = new QRadioButton(tr("左行"));
    m_leftRBtn->setFixedSize(100, 40);
    m_rightRBtn = new QRadioButton(tr("右行"));
    m_rightRBtn->setFixedSize(100, 40);
    m_leftRBtn->setChecked(true);

    for (int i = 0; i < weighbridge::ChannelCnt; ++i)
    {
        m_refLdt[i] = new QLineEdit(QString());
        m_refLdt[i]->setFixedHeight(30);
    }

    m_plantformABTitle = new QLabel;
    m_plantformABTitle->setText(tr("A台面+B台面重量"));
    m_plantformABTitle->setStyleSheet(css);
    m_plantformBCTitle = new QLabel;
    m_plantformBCTitle->setText(tr("B台面+C台面重量"));
    m_plantformBCTitle->setStyleSheet(css);

    QStringList header;
    header << tr("序号") << tr("车号") << tr("车型") << tr("重量") << tr("速度");
    m_resultWidget = new QTableWidget(1, header.count());
    m_resultWidget->setHorizontalHeaderLabels(header);
//    m_resultWidget->verticalHeader()->hide();

    for (int i = 0; i < 5; ++i)
    {
        m_unit[i] = new QLabel(tr("吨"));
        m_unit[i]->setStyleSheet(css);
    }

    header.clear();
    header << tr("A台面重量") << tr("B台面重量") << tr("C台面重量");
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        m_plantformTitle[i] = new QLabel(header.at(i));
        m_plantformBtn[i] = new QPushButton(tr("台面清零"));
        m_plantformBtn[i]->setFixedSize(75, 40);
    }
}

void StaticWeightWidget::setupUi()
{
    createWidget();

    QStringList pfmTitle;
    pfmTitle << tr("A台面") << tr("B台面") << tr("C台面");
    QGridLayout *grid1[MaxPlantformNum];
    QGroupBox *ref[MaxPlantformNum];
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        grid1[i] = new QGridLayout;
        ref[i] = new QGroupBox(pfmTitle[i]);
        for (int j = 0; j < PlantformChannel; ++j)
            grid1[i]->addWidget(m_refLdt[i * 4 + j], j / 2, j % 2);

        ref[i]->setLayout(grid1[i]);
    }

    QHBoxLayout *refLayout = new QHBoxLayout;
    for (int i = 0; i < MaxPlantformNum; ++i)
        refLayout->addWidget(ref[i]);

    QVBoxLayout *upLayout = new QVBoxLayout;
    upLayout->addLayout(refLayout);
//    upLayout->addStretch();

    QGridLayout *grid2 = new QGridLayout;
    grid2->addWidget(m_plantformABTitle, 0, 0);
    grid2->addWidget(m_abLdt, 0, 1);
    grid2->addWidget(m_unit[0], 0, 2);
    grid2->addWidget(m_recABBtn, 0, 3);
    grid2->addWidget(m_plantformBCTitle, 1, 0);
    grid2->addWidget(m_bcLdt, 1, 1);
    grid2->addWidget(m_unit[1], 1, 2);
    grid2->addWidget(m_recBCBtn, 1, 3);

    QGroupBox *weightLayout = new QGroupBox;
    weightLayout->setLayout(grid2);

    QGridLayout *grid3[MaxPlantformNum];
    QGroupBox *pfm[MaxPlantformNum];
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        grid3[i] = new QGridLayout;
        grid3[i]->addWidget(m_plantformTitle[i], 0, 0, 1, 2, Qt::AlignHCenter);
        grid3[i]->addWidget(m_pfmLdt[i], 1, 0);
        grid3[i]->addWidget(m_unit[2 + i], 1, 1);
        grid3[i]->addWidget(m_plantformBtn[i], 2, 0, 1, 2, Qt::AlignHCenter);

        pfm[i] = new QGroupBox(pfmTitle[i]);
        pfm[i]->setLayout(grid3[i]);
    }
    QHBoxLayout *pfmLayout = new QHBoxLayout;
    for (int i = 0; i < MaxPlantformNum; ++i)
        pfmLayout->addWidget(pfm[i]);

    QGridLayout *grid4 = new QGridLayout;
    QGroupBox *dir = new QGroupBox(tr("方向"));
    grid4->addWidget(m_leftRBtn, 0, 0);
    grid4->addWidget(m_rightRBtn, 0, 1);
    dir->setLayout(grid4);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(weightLayout);
    leftLayout->addLayout(pfmLayout);
    leftLayout->addWidget(dir);
    leftLayout->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_resultWidget);

    QHBoxLayout *btmLayout = new QHBoxLayout;
    btmLayout->addLayout(leftLayout);
    btmLayout->addSpacing(20);
    btmLayout->addLayout(rightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(upLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btmLayout);

    m_smWidget->setLayout(mainLayout);

}

void StaticWeightWidget::init()
{

}

void StaticWeightWidget::onSaveResult()
{

}

void StaticWeightWidget::onPrintResult()
{

}

void StaticWeightWidget::onQueryResult()
{

}

void StaticWeightWidget::onRecordABWeight()
{

}

void StaticWeightWidget::onRecordBCWeight()
{

}

void StaticWeightWidget::onPlantformAZero()
{
    qDebug() << "a";
}

void StaticWeightWidget::onPlantformBZero()
{
    qDebug() << "b";
}

void StaticWeightWidget::onPlantformCZero()
{
    qDebug() << "c";
}
