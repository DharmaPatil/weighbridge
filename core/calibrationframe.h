/**
* @file calibrationwidget.h
* @brief 曲线标定类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef CALIBRATIONFRAME_H
#define CALIBRATIONFRAME_H

#include "mainwindowbase.h"

class Workbench;
class StdCurveWidget;
class QStackedWidget;
class DynamicCalibrationWidget;
class StaticCalibrationWidget;


class CalibrationFrame : public MainWindowBase
{
    Q_OBJECT
public:
    CalibrationFrame(Workbench *wb, QWidget *parent = 0);
    ~CalibrationFrame();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void changeCaliMethod(int i);
    void changeCurve(QString &name);
    void saveCurve();
    void resetCurve();
    void modifyCurve();

private:
    Workbench *m_workbench;
    StdCurveWidget *m_curveWidget;
    ToolBar *m_toolbar;
    QStackedWidget *m_caliWidget;
    DynamicCalibrationWidget *m_dynamicWidget;
    StaticCalibrationWidget *m_staticWidget;
};

#endif // CALIBRATIONFRAME_H
