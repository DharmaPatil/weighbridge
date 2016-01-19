/**
* @file samplecurveplot.cpp
* @brief 采样数据波形显示类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#include <qwt_scale_div.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_text.h>
#include <qwt_symbol.h>

#include "samplecurveplot.h"

//////////////////////////////////////////////////////////
/// \brief The SampleCurve class
///
class SampleCurve : public QwtPlotCurve
{
public:
    SampleCurve(const QString &title)
        : QwtPlotCurve(title)
    {
        setRenderHint(QwtPlotItem::RenderAntialiased);
    }

    void setColor(const QColor &color)
    {
        QColor c = color;
        c.setAlpha(150);

        setPen(c);
        setBrush(c);
    }
};


///////////////////////////////////////////////////////////
/// \brief SampleCurvePlot class
///
SampleCurvePlot::SampleCurvePlot(QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);
    setTitle("Sensor Curve");
    setCanvasBackground(QColor(Qt::darkBlue));

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(this);

    // axis
    setAxisTitle(QwtPlot::xBottom, "Time (ms)");
    setAxisScale(QwtPlot::xBottom, 0, 2048);
    setAxisTitle(QwtPlot::yLeft, "Sample Point");
    setAxisScale(QwtPlot::yLeft, -12000, 60000, 6000);

    // curve
    SampleCurve *curve;
    for (int i = 0; i < ChannelMax + 1; ++i)
    {
        curve = new SampleCurve(QString("Ch%1").arg(i + 1, 2, 10, QLatin1Char('0')));
        curve->setColor(QColor(255 - i * 10, 200 - i * 5, 150 - i * 5));
        curve->attach(this);
        sampleData[i].curve = curve;
        showCurve(sampleData[i].curve, false);
    }
    showCurve(sampleData[0].curve, true);

    // marker
    d_mrk1 = new QwtPlotMarker();
    d_mrk1->setValue(0.0, 0.0);
    d_mrk1->setLineStyle(QwtPlotMarker::VLine);
    d_mrk1->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_mrk1->setLinePen(QPen(Qt::green, 0, Qt::DashDotLine));
    d_mrk1->attach(this);

    d_mrk2 = new QwtPlotMarker();
    d_mrk2->setLineStyle(QwtPlotMarker::HLine);
    d_mrk2->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    d_mrk2->setLinePen(QPen(QColor(200,150,0), 0, Qt::DashDotLine));
    d_mrk2->setSymbol( QwtSymbol(QwtSymbol::Diamond,
        QColor(Qt::yellow), QColor(Qt::green), QSize(7,7)));
    d_mrk2->attach(this);

    connect(this, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            SLOT(showCurve(QwtPlotItem*,bool)));

    init();

    setAutoReplot(true);
}

void SampleCurvePlot::init()
{
    QVector<double> data;
    data << 0.0;

    for (int ch = 0; ch < ChannelMax + 1; ++ch)
        setData(ch, data);
}

SampleCurvePlot::~SampleCurvePlot()
{

}

void SampleCurvePlot::setData(int ch, QVector<double> &data)
{
    QVector<double> xData;

    for (int i = 0; i < data.count(); ++i)
        xData.append(i);

    sampleData[ch].data = data;
    sampleData[ch].curve->setData(xData, data);
}

void SampleCurvePlot::setFile(QString &filename)
{

}

void SampleCurvePlot::showPeak(double x, double y)
{
    QString label;
    label.sprintf("Peak: %.2g", y);

    QwtText text(label);
    text.setFont(QFont("Helvetica", 10, QFont::Bold));
    text.setColor(QColor(200,150,0));

    d_mrk2->setValue(x, y);
    d_mrk2->setLabel(text);
}

void SampleCurvePlot::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);

    replot();
}
