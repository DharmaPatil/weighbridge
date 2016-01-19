/**
* @file staticcalibrationwidget.h
* @brief 静态标定窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-14
*/
#ifndef STATICCALIBRATIONWIDGET_H
#define STATICCALIBRATIONWIDGET_H

#include <QWidget>
#include "global.h"

class Workbench;
class QLineEdit;

class StaticCalibrationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StaticCalibrationWidget(Workbench *wb, QWidget *parent = 0);
    ~StaticCalibrationWidget();

    Workbench *workbench() const { return m_workbench; }

signals:

public slots:
    void onUpdateSampleData(QList<int> &);

private:
    void setupUi();
    void init();
    void saveABResult();
    void saveBCResult();
    void resetPlantform(int i);
    void adjustFactor(int i);
    void correctWeight(int i);

private:
    Workbench *m_workbench;

    QLineEdit *m_abWeightLdt;
    QLineEdit *m_bcWeightLdt;

    QLineEdit *n_plantformLdt[weighbridge::PlantformNum];
    QLineEdit *m_gradFactor[weighbridge::PlantformNum];

    QLineEdit *m_balance[weighbridge::ChannelCnt];

    QPushButton *m_saveABBtn;
    QPushButton *m_saveBCBtn;
    QPushButton *m_zeroBtn[weighbridge::PlantformNum];
    QPushButton *m_factorBtn[weighbridge::PlantformNum];
//    QPushButton *m_balanceBtn[weighbridge::PlantformNum];

    QList<int> *m_factors[weighbridge::PlantformNum];

    int m_currIndex;
    int m_channelOnPlantform;
    int m_channelNum;
    int m_plantformNum;

    QStringList m_plTitle;
    QStringList m_sensorTitle;
};

#endif // STATICCALIBRATIONWIDGET_H
