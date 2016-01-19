/**
* @file AnalysisWidget.h
* @brief 检测数据显示窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QtDebug>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QClipboard>
#include <QSet>
#include <QFileDialog>

#include <qwt_counter.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

#include "analysiswidget.h"
#include "analysisdoc.h"
#include "analysisdocmodel.h"
#include "samplecurveplot.h"s
#include "workbench.h"
#include "instrument.h"

/////////////////////////////////////////////////////////////
/// \brief The Zoomer class
///
class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):
        QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern(QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier);
        setMousePattern(QwtEventPattern::MouseSelect3,
            Qt::RightButton);
    }
};


///////////////////////////////////////////////////////////////////////
/// \brief AnalysisWidget class
///
AnalysisWidget::AnalysisWidget(Workbench *wb, QWidget *parent)
    : m_workbench(wb)
    , QWidget(parent)
{
    setupUi();
    init();


}

AnalysisWidget::~AnalysisWidget()
{

}

void AnalysisWidget::setupUi()
{
    m_curvePlot = new SampleCurvePlot;

    m_selectFilename = new QLabel;
    m_selectFilename->setText(QString());

    QVBoxLayout *curveLayout = new QVBoxLayout;
    curveLayout->addWidget(m_selectFilename);
    curveLayout->addWidget(m_curvePlot);
    curveLayout->addSpacing(10);

    m_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        m_curvePlot->canvas());
    m_zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
    m_zoomer[0]->setRubberBandPen(QColor(Qt::green));
    m_zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    m_zoomer[0]->setTrackerPen(QColor(Qt::white));

    m_zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yLeft,
         m_curvePlot->canvas());

    m_panner = new QwtPlotPanner(m_curvePlot->canvas());
    m_panner->setMouseButton(Qt::MidButton);

    m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPicker::PointSelection | QwtPicker::DragSelection,
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
        m_curvePlot->canvas());
    m_picker->setRubberBandPen(QColor(Qt::green));
    m_picker->setRubberBand(QwtPicker::CrossRubberBand);
    m_picker->setTrackerPen(QColor(Qt::white));

    enableZoomMode(false);

    m_synthesis = new QCheckBox;
    m_synthesis->setText(tr("二力合成"));
    m_filterMax = new QCheckBox;
    m_filterMax->setText(tr("过滤大数"));
    m_smoother = new QCheckBox;
    m_smoother->setText(tr("数据平滑"));
    m_fiveAxis = new QCheckBox;
    m_fiveAxis->setText(tr("五轴车"));

    QVBoxLayout *setupLayout = new QVBoxLayout;
    setupLayout->addWidget(m_synthesis);
    setupLayout->addWidget(m_filterMax);
    setupLayout->addWidget(m_smoother);
    setupLayout->addWidget(m_fiveAxis);

    m_itemGbx = new QGroupBox(tr("设置操作"));
    m_itemGbx->setLayout(setupLayout);

    m_selectBtn = new QPushButton;
    m_selectBtn->setText(tr("选择文件"));
    m_zoomBtn = new QPushButton;
    m_zoomBtn->setText(tr("放大"));
    m_cleanBtn = new QPushButton;
    m_cleanBtn->setText(tr("清除"));

    connect(m_selectBtn, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(m_zoomBtn, SIGNAL(toggled(bool)), SLOT(enableZoomMode(bool)));
    connect(m_cleanBtn, SIGNAL(clicked()), this, SLOT(clean()));
    connect(m_picker, SIGNAL(moved(const QPoint &)),
            SLOT(moved(const QPoint &)));
    connect(m_picker, SIGNAL(selected(const QwtPolygon &)),
            SLOT(selected(const QwtPolygon &)));

    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->addWidget(m_selectBtn);
    btnLayout->addWidget(m_zoomBtn);
    btnLayout->addWidget(m_cleanBtn);

    m_funcGbx = new QGroupBox(tr("功能"));
    m_funcGbx->setLayout(btnLayout);

    m_dataTableWidget = new QTableWidget;
    m_dataTableWidget->setFixedSize(500, 125);

    QHBoxLayout *funcLayout = new QHBoxLayout;
    funcLayout->addSpacing(50);
    funcLayout->addWidget(m_itemGbx);
    funcLayout->addSpacing(20);
    funcLayout->addWidget(m_funcGbx);
    funcLayout->addSpacing(50);
    funcLayout->addWidget(m_dataTableWidget);
    funcLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(curveLayout);
    mainLayout->addLayout(funcLayout);

    setLayout(mainLayout);
}

void AnalysisWidget::init()
{

}

void AnalysisWidget::debugCurve()
{

}

void AnalysisWidget::enableZoomMode(bool on)
{
    qDebug() << "zoom";
    m_panner->setEnabled(on);

    m_zoomer[0]->setEnabled(on);
    m_zoomer[0]->zoom(0);

    m_zoomer[1]->setEnabled(on);
    m_zoomer[1]->zoom(0);

    m_picker->setEnabled(!on);

    showInfo();
}

void AnalysisWidget::clean()
{
    qDebug() << "clean";
}

void AnalysisWidget::selectFile()
{
    qDebug() << "select";
}

void AnalysisWidget::moved(const QPoint &pos)
{
    QString info;
    info.sprintf("x=%g, y=%g",
        m_curvePlot->invTransform(QwtPlot::xBottom, pos.x()),
        m_curvePlot->invTransform(QwtPlot::yLeft, pos.y())
    );
    showInfo(info);
}

void AnalysisWidget::selected(const QwtPolygon &)
{
    showInfo();
}

void AnalysisWidget::showInfo(QString text)
{
    if ( text == QString::null )
    {
        if ( m_picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }
}
