#ifndef WEIGHTFRAME_H
#define WEIGHTFRAME_H

#include "../core/mainwindowbase.h"

class Workbench;
class QToolBar;
class WeightWidget;
class WBAction;

class WeightFrame : public MainWindowBase
{
    Q_OBJECT

public:
    WeightFrame(Workbench *wb, QWidget *parent = 0);
    ~WeightFrame();

    Workbench *workbench() const { return m_workbench; }

private slots:
    void onManualTest();
    void updateStatus();
    void onStartMeasure();
    void onPauseMeasure();
    void onReprodduce();

private:
    void btnRender();

private:
    Workbench *m_workbench;
    WeightWidget *m_weightWidget;
    QToolBar *m_toolbar;
    WBAction *actions;

    bool m_startFlag;
    bool m_pauseFlag;
    bool m_modeFlag;

    QString m_openPath;
};

#endif // WEIGHTFRAME_H
