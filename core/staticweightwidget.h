/**
* @file staticweightwidget.h
* @brief 静态称重窗口类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-04
*/
#ifndef STATICWEIGHTWIDGET_H
#define STATICWEIGHTWIDGET_H

#include "mainwindowbase.h"
#include "global.h"

class Workbench;
class QPushButton;
class QGroupBox;
class QLabel;
class QRadioButton;
class QHBoxLayout;
class QVBoxLayout;
class QTableWidget;
class QLineEdit;
class WBAction;

class StaticWeightWidget : public MainWindowBase
{
    Q_OBJECT

public:
    StaticWeightWidget(Workbench *wb, QWidget *parent = 0);
    ~StaticWeightWidget();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void onSaveResult();
    void onPrintResult();
    void onQueryResult();
    void onRecordABWeight();
    void onRecordBCWeight();
    void onPlantformAZero();
    void onPlantformBZero();
    void onPlantformCZero();

private:
    void createWidget();
    void setupUi();
    void init();

private:
    enum {PlantformChannel = 4, MaxPlantformNum = 3 };

    Workbench *m_workbench;
    ToolBar *m_toolbar;
    WBAction *m_actions;

    QWidget *m_smWidget;
    QPushButton *m_recABBtn;
    QPushButton *m_recBCBtn;
    QPushButton *m_plantformBtn[MaxPlantformNum];
    QPushButton *m_saveResult;

    QRadioButton *m_leftRBtn;
    QRadioButton *m_rightRBtn;

    QLineEdit *m_abLdt;
    QLineEdit *m_bcLdt;
    QLineEdit *m_pfmLdt[MaxPlantformNum];

    QTableWidget *m_resultWidget;
    QLineEdit *m_refLdt[weighbridge::ChannelCnt];

    QLabel *m_plantformABTitle;
    QLabel *m_plantformBCTitle;
    QLabel *m_unit[5];
    QLabel *m_plantformTitle[MaxPlantformNum];

};

#endif // STATICWEIGHTWIDGET_H
