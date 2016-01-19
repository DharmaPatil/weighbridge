/**
* @file analysistablewidget.h
* @brief 检测数据显示窗口类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>
//#include <qwt_plot.h>
#include <qwt_polygon.h>

class Workbench;
class AnalysisDoc;
class AnalysisDocModel;
class QTableWidget;
class QTableView;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class SampleCurvePlot;
class QLabel;

class AnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisWidget(Workbench *wb, QWidget *parent = 0);
    ~AnalysisWidget();

    Workbench *workbench() const { return m_workbench; }
	void debugCurve();

protected:

private slots:
    void clean();
    void selectFile();

    void enableZoomMode(bool);
    void moved(const QPoint &);
    void selected(const QwtPolygon &);


private:
    void setupUi();
    void init();
    void showInfo(QString text = QString::null);

private:
    Workbench *m_workbench;

    AnalysisDoc *m_doc;
    AnalysisDocModel *m_docModel;

    QTableWidget *m_dataTableWidget;

    QString m_fileName;

    QCheckBox *m_synthesis;
    QCheckBox *m_filterMax;
    QCheckBox *m_smoother;
    QCheckBox *m_fiveAxis;
    QGroupBox *m_itemGbx;

    QPushButton *m_selectBtn;
    QPushButton *m_zoomBtn;
    QPushButton *m_cleanBtn;
    QGroupBox *m_funcGbx;

    QLabel *m_selectFilename;
    SampleCurvePlot *m_curvePlot;

    QwtPlotZoomer *m_zoomer[2];
    QwtPlotPicker *m_picker;
    QwtPlotPanner *m_panner;
};

#endif // ANALYSISWIDGET_H
