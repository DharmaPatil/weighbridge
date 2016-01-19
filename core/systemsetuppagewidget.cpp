/**
* @file systemsetuppagewidget.cpp
* @brief 系统配置类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "systemsetuppagewidget.h"
#include "profile.h"

SystemSetupPageWidget::SystemSetupPageWidget(Workbench *wb, QWidget *parent)
    : OptionWidget(wb, parent)
{
    setWindowTitle(tr("系统通用设置"));
    setWindowModality(Qt::NonModal);

    machineLayout();
    softwareLayout();
    portLayout();
    plantformLayout();

    // 左侧布局
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(machine);
    leftLayout->addWidget(software);
    leftLayout->addStretch();


    //右侧布局
    chLayout = new QVBoxLayout;
    chLayout->addLayout(grid3);
    chLayout->addWidget(plantform[0]);
    chLayout->addWidget(plantform[1]);
    chLayout->addWidget(plantform[2]);

    QGroupBox *instr = new QGroupBox;
    instr->setTitle(tr("通道、仪表相关"));
    instr->setLayout(chLayout);

    // 窗体布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(instr);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

SystemSetupPageWidget::~SystemSetupPageWidget()
{

}

void SystemSetupPageWidget::machineLayout(QWidget *parent)
{
    dbVal.clear();
    range.clear();

    QLabel *plantformNum = new QLabel(parent);
    plantformNum->setText(tr("台面数量："));
    dbVal = profile.value("configure/general/machine/plantform/num").toString();
    curIndex = getConfigIndexRange(dbVal, range);
    m_plantformNum = new QComboBox(parent);
    m_plantformNum->addItems(range);
    m_plantformNum->setCurrentIndex(curIndex);

    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        QString key = QString("configure/general/machine/plantform%1/len").arg(i + 1);
        m_plantformLen[i] = new QLineEdit(parent);
        m_plantformLen[i]->setText(profile.value(key).toString());
    }

    QLabel *plantformTitle[MaxPlantformNum], *unit[MaxPlantformNum];
    QStringList titleName;
    titleName << tr("左台面长度：") << tr("中台面长度：") << tr("右台面长度：");
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        plantformTitle[i] = new QLabel(parent);
        plantformTitle[i]->setText(titleName.at(i));
        unit[i] = new QLabel(parent);
        unit[i]->setText("mm");
    }

    m_plantformType = new QCheckBox(parent);
    m_plantformType->setChecked(profile.value("configure/general/machine/plantformType").toBool());
    m_plantformType->setText(tr("断轨台面"));

    // 机械相关布局
    grid1 = new QGridLayout(parent);
    grid1->addWidget(plantformNum, 0, 0);
    grid1->addWidget(m_plantformNum, 0, 1);
    grid1->addWidget(plantformTitle[0], 1, 0);
    grid1->addWidget(m_plantformLen[0], 1, 1);
    grid1->addWidget(unit[0], 1, 2);
    grid1->addWidget(plantformTitle[1], 2, 0);
    grid1->addWidget(m_plantformLen[1], 2, 1);
    grid1->addWidget(unit[1], 2, 2);
    grid1->addWidget(plantformTitle[2], 3, 0);
    grid1->addWidget(m_plantformLen[2], 3, 1);
    grid1->addWidget(unit[2], 3, 2);
    grid1->addWidget(m_plantformType, 4, 1, 1, 3);

    machine = new QGroupBox(tr("机械相关"), parent);
    machine->setLayout(grid1);
}

void SystemSetupPageWidget::softwareLayout(QWidget *parent)
{
    m_chSynMode[0] = new QCheckBox(tr("传感器信号硬件合成"), parent);
    m_chSynMode[1] = new QCheckBox(tr("传感器信号软件合成"), parent);
    m_swSyn[0] = new QRadioButton(tr("二力合成，软件判别"), parent);
    m_swSyn[1] = new QRadioButton(tr("二力合成，剪力判别"), parent);
    m_swSyn[2] = new QRadioButton(tr("压力称重，剪力判别"), parent);
    m_swSyn[3] = new QRadioButton(tr("压力称重，弃用剪力"), parent);

    QStringList tmp = profile.value("configure/general/software/signal/hs").toString().split(";");
    int index = tmp.at(0).toInt();
    m_chSynMode[index]->setChecked(true);

    tmp.clear();
    tmp = profile.value("configure/general/software/signal/ss").toString().split(";");
    index = tmp.at(0).toInt();
    m_swSyn[index]->setChecked(true);

    QLabel *deep = new QLabel(tr("采样深度"), parent);
    m_sampleDeep = new QLineEdit(parent);
    m_sampleDeep->setText(profile.value("configure/general/software/sample/deep").toString());

    QLabel *maxWeight = new QLabel(tr("最大称重"), parent);
    m_maxWeight = new QLineEdit(parent);
    m_maxWeight->setText(profile.value("configure/general/software/maxrange").toString());
    QLabel *weightUnit = new QLabel(tr("kg"), parent);

    QLabel *minRes = new QLabel(parent);
    minRes->setText(tr("最小分度数"));
    m_minResolution = new QComboBox(parent);
    QLabel *resUnit = new QLabel(parent);
    resUnit->setText(tr("kg"));
    dbVal.clear();
    range.clear();
    dbVal = profile.value("configure/general/software/minresolution").toString();
    curIndex = getConfigIndexRange(dbVal, range);
    m_minResolution->addItems(range);
    m_minResolution->setCurrentIndex(curIndex);

    // 软件其他相关布局
    grid2 = new QGridLayout(parent);
    grid2->addWidget(m_chSynMode[0], 0, 0, 1, 2);
    grid2->addWidget(m_swSyn[0], 1, 0, 1, 2);
    grid2->addWidget(m_swSyn[1], 2, 0, 1, 2);
    grid2->addWidget(m_swSyn[2], 3, 0, 1, 2);
    grid2->addWidget(m_swSyn[3], 4, 0, 1, 2);
    grid2->addWidget(m_chSynMode[1], 5, 0, 1, 2);
    grid2->addWidget(deep, 6, 0);
    grid2->addWidget(m_sampleDeep, 6, 1);
    grid2->addWidget(maxWeight, 7, 0);
    grid2->addWidget(m_maxWeight, 7, 1);
    grid2->addWidget(weightUnit, 7, 2);
    grid2->addWidget(minRes, 8, 0);
    grid2->addWidget(m_minResolution, 8, 1);
    grid2->addWidget(resUnit, 8, 2);

    software = new QGroupBox(tr("软件、其他相关"), parent);
    software->setLayout(grid2);
}

void SystemSetupPageWidget::portLayout(QWidget *parent)
{
    // 通道
    QLabel *chType = new QLabel(parent);
    chType->setText(tr("通道A/D类型"));

    m_adType = new QComboBox(parent);
    dbVal.clear();
    range.clear();
    dbVal = profile.value("configure/general/hardware/adtype").toString();
    curIndex = getConfigIndexRange(dbVal, range);
    m_adType->addItems(range);
    m_adType->setCurrentIndex(curIndex);

    // 车号
    QLabel *tcPort = new QLabel(parent);
    tcPort->setText(tr("车号使用端口"));

    m_traincodePort = new QComboBox(parent);
    dbVal.clear();
    range.clear();
    dbVal = profile.value("configure/general/hardware/serial/traincode").toString();
    curIndex = getConfigIndexRange(dbVal, range);
    m_traincodePort->addItems(range);
    m_traincodePort->setCurrentIndex(curIndex);

    // 仪器
    QLabel *instrPort = new QLabel(parent);
    instrPort->setText(tr("仪表使用端口"));
    m_instrPort = new QComboBox(parent);
    dbVal.clear();
    range.clear();
    dbVal = profile.value("configure/general/hardware/serial/instr").toString();
    curIndex = getConfigIndexRange(dbVal, range);
    m_instrPort->addItems(range);
    m_instrPort->setCurrentIndex(curIndex);

    // 通道数
    QLabel *chNum = new QLabel(parent);
    chNum->setText(tr("总通道数"));
    m_totalChannel = new QLineEdit(parent);
    m_totalChannel->setText(profile.value("configure/general/hardware/channel/perplantfrom").toString());

    // 车号端口属性
    QLabel *tnPortAttr = new QLabel(parent);
    tnPortAttr->setText(tr("车号端口属性"));
    m_traincodeAttr = new QLineEdit(parent);
    m_traincodeAttr->setText(profile.value("configure/general/hardware/serial/traincode/attr").toString());

    // 仪器端口属性
    QLabel *instrPortAttr = new QLabel(parent);
    instrPortAttr->setText(tr("仪器端口属性"));
    m_instrAttr = new QLineEdit(parent);
    m_instrAttr->setText(profile.value("configure/general/hardware/serial/instr/attr").toString());

    // 端口布局
    grid3 = new QGridLayout(parent);
    grid3->addWidget(chType, 0, 0);
    grid3->addWidget(m_adType, 0, 1);
    grid3->addWidget(tcPort, 0, 2);
    grid3->addWidget(m_traincodePort, 0, 3);
    grid3->addWidget(instrPort, 0, 4);
    grid3->addWidget(m_instrPort, 0, 5);
    grid3->addWidget(chNum, 1, 0);
    grid3->addWidget(m_totalChannel, 1, 1);
    grid3->addWidget(tnPortAttr, 1, 2);
    grid3->addWidget(m_traincodeAttr, 1, 3);
    grid3->addWidget(instrPortAttr, 1, 4);
    grid3->addWidget(m_instrAttr, 1, 5);
}

void SystemSetupPageWidget::plantformLayout(QWidget *parent)
{
    QStringList chNo;
    chNo << "1" << "2" << "3" << "4";
    QLabel *chPos[MaxPlantformNum][MaxChannelNum];
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        m_chlayout[i] = new QLabel(parent);
        m_chlayout[i]->setPixmap(QPixmap(":/Resources/sensorlayout.png"));

        for (int j = 0; j < MaxChannelNum; ++j)
        {
            m_ch[i][j] = new QComboBox(parent);
            m_ch[i][j]->addItems(chNo);
            m_ch[i][j]->setCurrentIndex(j);

            chPos[i][j] = new QLabel(QString("CH%1-").arg(j + 1), parent);
        }
        for (int j = 0; j < ChannleFuncNum; ++j)
            m_chSynMethodSel[i][j] = new QRadioButton(parent);
    }

    QStringList selTitles;
    selTitles << tr("压力单边合成") << tr("压力同侧合并") << tr("压力单独使用") << tr("压力全部合成");
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        m_plantformEn[i] = new QCheckBox(parent);
        m_plantformEn[i]->setText(tr("使用此台面"));
        for (int j = 0; j < ChannleFuncNum; ++j)
        {
            m_chSynMethodSel[i][j] = new QRadioButton(parent);
            m_chSynMethodSel[i][j]->setText(selTitles.at(j));
        }

        dbVal.clear();
        range.clear();
        QString key = QString("configure/general/hardware/channel/syn/plantform%1").arg(i + 1);
        dbVal = profile.value(key).toString();
        curIndex = getConfigIndexRange(dbVal, range);
        m_chSynMethodSel[i][curIndex]->setChecked(true);
    }

    QVBoxLayout *methodSel[MaxPlantformNum];
    QGroupBox *chPosGbx[MaxPlantformNum];
    QGridLayout *chPosLayout[MaxPlantformNum];
    QHBoxLayout *plantformLayout[MaxPlantformNum];
    for (int i = 0; i < MaxPlantformNum; ++i)
    {
        methodSel[i] = new QVBoxLayout(parent);
        plantform[i] = new QGroupBox(parent);
        plantform[i]->setTitle(QString(tr("台面%1")).arg(i + 1));
        plantformLayout[i] = new QHBoxLayout(parent);
        methodSel[i]->addWidget(m_plantformEn[i]);
        for (int j = 0; j < ChannleFuncNum; ++j)
            methodSel[i]->addWidget(m_chSynMethodSel[i][j]);

        chPosLayout[i] = new QGridLayout(parent);
        chPosGbx[i] = new QGroupBox(parent);
        chPosGbx[i]->setTitle(tr("对应通道"));
        for (int j = 0; j < ChannleFuncNum; ++j)
        {
            chPosLayout[i]->addWidget(chPos[i][j], j, 0);
            chPosLayout[i]->addWidget(m_ch[i][j], j, 1);
        }
        chPosGbx[i]->setLayout(chPosLayout[i]);

        plantformLayout[i]->addWidget(m_chlayout[i]);
        plantformLayout[i]->addWidget(chPosGbx[i]);
        plantformLayout[i]->addSpacing(20);
        plantformLayout[i]->addLayout(methodSel[i]);

        plantform[i]->setLayout(plantformLayout[i]);
    }
}

int SystemSetupPageWidget::getConfigIndexRange(const QString configValue, QStringList &range)
{
    QStringList val = configValue.split(";");
    QString cur = val.at(0);
    QStringList strRange = val.at(1).split("/");
    range.append(strRange);
    return strRange.indexOf(cur);
}

void SystemSetupPageWidget::save()
{

}
