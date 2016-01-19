/**
* @file staticcalibrationwidget.cpp
* @brief 静态标定窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-14
*/
#include <QtGui>
#include <QDebug>

#include "staticcalibrationwidget.h"
#include "global.h"
#include "workbench.h"
#include "profile.h"
#include "instrument.h"

StaticCalibrationWidget::StaticCalibrationWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
    , m_currIndex(0)
	, m_channelNum(weighbridge::ChannelCnt)
	, m_plantformNum( weighbridge::PlantformNum)
	, m_channelOnPlantform(4)
{

    init();
    setupUi();

}

StaticCalibrationWidget::~StaticCalibrationWidget()
{

}

void StaticCalibrationWidget::setupUi()
{
    m_abWeightLdt = new QLineEdit(QString());
    m_bcWeightLdt = new QLineEdit(QString());

	for (int i = 0; i < m_channelNum; ++i)
		n_plantformLdt[i] = new QLineEdit(QString());

    for (int i = 0; i < m_channelNum; ++i)
        m_balance[i] = new QLineEdit(QString());

    m_saveABBtn = new QPushButton;
    m_saveABBtn->setText(tr("保存AB台重量"));
    m_saveBCBtn = new QPushButton;
    m_saveBCBtn->setText(tr("保存BC台重量"));

    connect(m_saveABBtn, SIGNAL(clicked()), this, SLOT(saveABResult()));
    connect(m_saveBCBtn, SIGNAL(clicked()), this, SLOT(saveBCResult()));

    QSignalMapper *mapper1 = new QSignalMapper(this);
    QSignalMapper *mapper2 = new QSignalMapper(this);
    for (int i = 0; i < m_plantformNum; ++i)
    {
        m_zeroBtn[i] = new QPushButton;
        m_zeroBtn[i]->setText(tr("台面清零"));
        connect(m_zeroBtn[i], SIGNAL(clicked()), mapper1, SLOT(map()));
        mapper1->setMapping(m_zeroBtn[i], i);

        m_factorBtn[i] = new QPushButton;
        m_factorBtn[i]->setText(tr("系数调整"));
        connect(m_zeroBtn[i], SIGNAL(clicked()), mapper2, SLOT(map()));
        mapper2->setMapping(m_factorBtn[i], i);

        m_gradFactor[i] = new QLineEdit(QString());
    }
    connect(mapper1, SIGNAL(mapped(int)), this, SLOT(resetPlantform(int)));
    connect(mapper2, SIGNAL(mapped(int)), this, SLOT(adjustFactor(int)));

    QLabel *result1 = new QLabel(tr("A台面+B台面重量"));
    QLabel *unit1 = new QLabel(tr("吨"));
    QLabel *result2 = new QLabel(tr("C台面+B台面重量"));
    QLabel *unit2 = new QLabel(tr("吨"));
    QGroupBox *result = new QGroupBox(tr("结果"));

    QGridLayout *wLayout = new QGridLayout;
    wLayout->addWidget(result1, 0, 0);
    wLayout->addWidget(m_abWeightLdt, 0, 1);
    wLayout->addWidget(unit1, 0, 2);
    wLayout->addWidget(result2, 1, 0);
    wLayout->addWidget(m_bcWeightLdt, 1, 1);
    wLayout->addWidget(unit2, 1, 2);

    QVBoxLayout *wfuncLayout = new QVBoxLayout;
    wfuncLayout->addWidget(m_saveABBtn);
    wfuncLayout->addWidget(m_saveBCBtn);

    // 结果布局
    QHBoxLayout *resultLayout = new QHBoxLayout;
    resultLayout->addLayout(wLayout);
    resultLayout->addSpacing(20);
    resultLayout->addLayout(wfuncLayout);
    resultLayout->addStretch();
    result->setLayout(resultLayout);

    QGroupBox *pltTitle[weighbridge::PlantformNum];
    QLabel *pwTitle[weighbridge::PlantformNum];
    QLabel *pfTitle[weighbridge::PlantformNum];
    QGridLayout *pltParaLayout[weighbridge::PlantformNum];

    // 台面布局
    QGridLayout *pltLayout = new QGridLayout;
    for (int i = 0; i < m_plantformNum; ++i)
    {
        pwTitle[i] = new QLabel(tr("台面重量"));
        pfTitle[i] = new QLabel(tr("比例系数"));
        pltParaLayout[i] = new QGridLayout;
        pltParaLayout[i]->addWidget(pwTitle[i], 0, 0);
        pltParaLayout[i]->addWidget(n_plantformLdt[i], 0, 1);
        pltParaLayout[i]->addWidget(pfTitle[i], 1, 0);
        pltParaLayout[i]->addWidget(m_gradFactor[i], 1, 1);
        pltParaLayout[i]->addWidget(m_zeroBtn[i], 2, 0, 1, 2, Qt::AlignHCenter);
        pltParaLayout[i]->addWidget(m_factorBtn[i], 3, 0, 1, 2, Qt::AlignHCenter);

        pltTitle[i] = new QGroupBox(m_plTitle.at(i));
        pltTitle[i]->setLayout(pltParaLayout[i]);

        pltLayout->addWidget(pltTitle[i], 0, i);
    }

    QGridLayout *balanceLayout = new QGridLayout;
    QGridLayout *balanceOnPlt[weighbridge::PlantformNum];
    QLabel *balTitle[weighbridge::PlantformNum][4];
    QGroupBox *balGbx[weighbridge::PlantformNum];
    for (int i = 0; i < m_plantformNum; ++i)
    {
        balGbx[i] = new QGroupBox(m_plTitle.at(i));
        balanceOnPlt[i] = new QGridLayout;
        for (int j = 0; j < m_channelOnPlantform; ++j)
        {
            balTitle[i][j] = new QLabel(m_sensorTitle.at(i * m_channelOnPlantform + j));
        }

        for (int j = 0; j < m_channelOnPlantform; ++j)
        {
            QHBoxLayout *tmp = new QHBoxLayout;
            tmp->addWidget(balTitle[i][j]);
            tmp->addWidget(m_balance[i * m_channelOnPlantform + j]);

            balanceOnPlt[i]->addLayout(tmp, j % 2, j / 2);
        }
        balGbx[i]->setLayout(balanceOnPlt[i]);
    }

    // 平衡系数布局
    for (int i = 0; i < m_plantformNum; ++i)
        balanceLayout->addWidget(balGbx[i], 0, i);

    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->addWidget(result);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pltLayout);
    contentLayout->addLayout(balanceLayout);
	contentLayout->addSpacing(50);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(100);
    mainLayout->addLayout(contentLayout);
    mainLayout->addSpacing(400);

    setLayout(mainLayout);
}

void StaticCalibrationWidget::init()
{
    int tmp = -1;

    tmp = workbench()->instrument()->instrumentInfo(Instrument::InfoSampChannels).toInt();
    m_channelNum = tmp >= weighbridge::ChannelCnt ? weighbridge::ChannelCnt : tmp;

    tmp = workbench()->instrument()->instrumentInfo(Instrument::InfoPlantform).toInt();
    m_plantformNum = tmp >= weighbridge::PlantformNum ? weighbridge::PlantformNum : tmp;

    m_channelOnPlantform = workbench()->instrument()->instrumentInfo(Instrument::InfoChannelPerPlantform).toInt();

    m_plTitle << tr("A台面") << tr("B台面") << tr("C台面");

    for (int i = 0; i < m_channelNum; ++i)
        m_sensorTitle << QString("传感器%1").arg(i % m_channelOnPlantform + 1);
}

void StaticCalibrationWidget::saveABResult()
{
    qDebug() << "sss";
}

void StaticCalibrationWidget::saveBCResult()
{
    qDebug() << "eeee";
}

void StaticCalibrationWidget::resetPlantform(int i)
{
    qDebug() << i;
}

void StaticCalibrationWidget::adjustFactor(int i)
{
    qDebug() << i;
}

void StaticCalibrationWidget::onUpdateSampleData(QList<int> &)
{

}

void StaticCalibrationWidget::correctWeight(int i)
{

}
