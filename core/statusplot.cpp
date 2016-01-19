/**
* @file statusplot.cpp
* @brief 状态波形显示类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#include <QTime>
#include <QPainter>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>

#include "statusplot.h"

//////////////////////////////////////////////////////////////
/// \brief The TimeScaleDraw class
///
class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime &base):
        baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        QTime upTime = baseTime.addSecs((int)v);
        return upTime.toString();
    }
private:
    QTime baseTime;
};

//////////////////////////////////////////////////////////////////
/// \brief The SampleCurve class
///
class SampleCurve: public QwtPlotCurve
{
public:
    SampleCurve(const QString &title):
        QwtPlotCurve(title)
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

/////////////////////////////////////////////////////////
/// \brief The Background class
///
class Background: public QwtPlotItem
{
public:
    Background()
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRect &rect) const
    {
        QColor c(Qt::white);
        QRect r = rect;

        for ( int i = 100; i > 0; i -= 10 )
        {
            r.setBottom(yMap.transform(i - 10));
            r.setTop(yMap.transform(i));
            painter->fillRect(r, c);

            c = c.dark(110);
        }
    }
};

StatusPlot::StatusPlot(QWidget *parent)
    : QwtPlot(parent)
{
    this->setMaximumHeight(120);
    setCanvasBackground(QColor(Qt::black));

    setAutoReplot(false);

    plotLayout()->setAlignCanvasToScales(true);

    setAxisScale(QwtPlot::xBottom, 0, MAXLEN, 100);
    setAxisScale(QwtPlot::yLeft, -12000, 60000);

    Background *bg = new Background();
    bg->attach(this);

    SampleCurve *curve = new SampleCurve(tr("Sample Curve"));
    curve->setColor(Qt::blue);
    curve->attach(this);
    data.curve = curve;

    showCurve(data.curve);

    for (int i = 0; i < MAXLEN; ++i)
        timeData[MAXLEN - 1 - i] = i;

}

void StatusPlot::showCurve(QwtPlotItem *item)
{
    item->setVisible(true);
    replot();
}

void StatusPlot::setData(QList<int> &data)
{

}

