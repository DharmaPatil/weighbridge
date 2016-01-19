#ifndef STDCURVEWIDGET_H
#define STDCURVEWIDGET_H

#include <QDialog>
#include "ui_stdcurvewidget.h"

class Workbench;
class StandardCurve;
class AnalysisTemplate;
class StandardCurveModel;

class StdCurveDialog : public QDialog
{
    Q_OBJECT
public:
    enum CurveViewFlag
    {
        CV_New = 0x01,
        CV_Current = 0x02,
        CV_Both = CV_New | CV_Current,
    };

    explicit StdCurveDialog(Workbench *wb, QWidget *parent = 0);
    ~StdCurveDialog();

    QString curve() const { return m_curve; }
    void setCurve(const QString &curve);

    Workbench *workbench() const { return m_workbench; }

    void setViewFlags(int flags);
    void modifyCurve();

signals:

private slots:
    void showBoth();
    void showCurrent();
    void showNew();
    void onCurveStatusChanged(const QString &curve);
    void refresh(int viewFlags = 0);
    void onCurveDestroyed(QObject *);
    void updateStatistic();
    void onViewClicked(const QModelIndex &index);
    void popupCurveMenu(const QPoint &pos);
    void on_x_textChanged(const QString &text);

private:
    Ui::StdCurveWidgetClass ui;

    QString m_curve;
    Workbench *m_workbench;
    AnalysisTemplate *m_assayTemplate;
    StandardCurve *m_currentCurve, *m_newCurve;
	StandardCurveModel *m_curveModel;
    int m_viewFlags;
};

#endif // STDCURVEWIDGET_H
