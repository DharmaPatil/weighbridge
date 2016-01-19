/**
* @file stdcurveplot.cpp
* @brief 标准曲线坐标类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/

#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QRectF>
#include <QFont>
#include <qwt_scale_engine.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_map.h>
#include <qwt_scale_div.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>

#include "global.h"
#include "standardcurve.h"
#include "analysistemplate.h"
#include "regression.h"
#include "workbench.h"
#include "stdcurveplot.h"

//////////////////////////////////////////////////////////
/// \brief The CurveData class
///
class CurveData : public QwtData
{
public:
    CurveData(StandardCurve *curve = 0) : m_curve(curve), m_boundingRect(1, 1, -2, -2) {}
    QwtData *copy() const { return new CurveData(m_curve); }
    size_t size() const { return m_curve ? m_curve->pointCount() : 0; }
    double x(size_t i) const { return m_curve->boundX(m_curve->point((int)i).x()); }
    double y(size_t i) const { return m_curve->boundY(m_curve->point((int)i).y()); }
    QwtDoubleRect boundingRect() const
    {
        if (!m_boundingRect.isValid())
            m_boundingRect = QwtData::boundingRect();
        return m_boundingRect;
    }

private:
    StandardCurve *m_curve;
    mutable QwtDoubleRect m_boundingRect;
};


///////////////////////////////////////////////
///// \brief The PlotCurve class
/////
class PlotCurve : public QwtPlotCurve
{
public:
    PlotCurve(StandardCurve *curve);
    void showSymbol(bool show = true);
    void setCurve(StandardCurve *curve);
    QwtDoubleRect boundingRect() const;

    StandardCurve *curve() const { return m_curve; }

protected:
    void drawCurve(QPainter *p, int style,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        int from, int to) const;

private:
    bool m_showSymbol;
    StandardCurve *m_curve;
};

PlotCurve::PlotCurve(StandardCurve *curve)
{
    setXAxis(StdCurvePlot::xDefault);
    setYAxis(StdCurvePlot::yDefault);

    setCurve(curve);
    showSymbol(false);
}

void PlotCurve::showSymbol(bool show)
{
    m_showSymbol = show;
    QwtSymbol::Style st = show ? QwtSymbol::Rect : QwtSymbol::NoSymbol;
    setSymbol(QwtSymbol(st, QColor(Qt::gray), QColor(Qt::darkGray), QSize(8, 8)));
}

void PlotCurve::setCurve(StandardCurve *curve)
{
    m_curve = curve;
    setData(CurveData(curve));
}

QwtDoubleRect PlotCurve::boundingRect() const
{
    if (!m_curve || m_curve->pointCount() == 0)
        return QwtDoubleRect(1.0, 1.0, -2.0, -2.0); // invalid

    QwtDoubleRect rect(m_curve->point(0).x(), m_curve->point(0).y(), 0, 0);
    for(int i=1; i<m_curve->pointCount(); ++i)
    {
        if (rect.left() > m_curve->point(i).x())
            rect.setLeft(m_curve->point(i).x());
        else if (rect.right() < m_curve->point(i).x())
            rect.setRight(m_curve->point(i).x());

        if (rect.top() > m_curve->point(i).y())
            rect.setTop(m_curve->point(i).y());
        else if (rect.bottom() < m_curve->point(i).y())
            rect.setBottom(m_curve->point(i).y());
    }

    if (rect.left() < m_curve->minDisplayX())
        rect.setLeft(m_curve->minDisplayX());
    if (rect.top() < m_curve->minDisplayY())
        rect.setTop(m_curve->minDisplayY());

    return rect;
}

void PlotCurve::drawCurve(QPainter *p,
                          int style,
                          const QwtScaleMap &xMap,
                          const QwtScaleMap &yMap,
                          int from,
                          int to) const
{
    if (!m_curve || !m_curve->isValid())
        return;

    const QwtScaleDiv *div = plot()->axisScaleDiv(xAxis());
    if (m_curve->method() == weighbridge::RegLinear)
    {
        QPoint p1, p2;

        p1.setX(xMap.transform(div->lowerBound()));
        p1.setY(yMap.transform(m_curve->y(div->lowerBound())));
        p2.setX(xMap.transform(div->upperBound()));
        p2.setY(yMap.transform(m_curve->y(div->upperBound())));

        p->setPen(pen());
        p->drawLine(p1, p2);
    }
    else
    {
        const int step = 5;
        QPolygonF points;

        from = xMap.transform(qMax(div->lowerBound(), m_curve->minDisplayX()));
        to = xMap.transform(qMax(div->upperBound(), m_curve->minDisplayX()));
        points.resize((to - from) / step + 1);

        for(int i=0; i<points.size(); ++i, from += step)
        {
            QPointF &p = points[i];
            p.setX(from);
            p.setY(yMap.xTransform(m_curve->y(xMap.invTransform(from))));
        }

        p->setPen(pen());
        p->drawPolyline(points);
    }
}

class PlotGrid : public QwtPlotGrid
{
public:
    PlotGrid()
    {
        //setItemAttribute(AutoScale);
    }

    virtual QRectF boundingRect() const
    {
        return QRectF(0.0, 0.0, 1.0, 1.0);
    }
};


/////////////////////////////////////////////////////////////////
/// \brief StdCurvePlot class
///
StdCurvePlot::StdCurvePlot(QWidget *parent)
    : QwtPlot(parent)
    , m_xFun(weighbridge::FunLinear)
    , m_yFun(weighbridge::FunLinear)
{
    enableAxis(yRight, true);
    enableAxis(yLeft, false);
    setCanvasBackground(Qt::white);

    {
        QwtPlotGrid *grid = new PlotGrid();
        grid->setAxis(xDefault, yDefault);
        QPen pen(Qt::DotLine);
        pen.setColor(Qt::darkGray);
        grid->setPen(pen);
        grid->enableXMin(true);
        grid->enableYMin(true);
        grid->attach(this);
    }

    {
        QwtLegend *legend = new QwtLegend;
        legend->setItemMode(QwtLegend::ReadOnlyItem);
        insertLegend(legend, QwtPlot::BottomLegend);
    }

    //setAxisScale(xDefault, 1, 100);
    //setAxisScale(yDefault, 1, 100);

    setAxisFont(xDefault, QFont());
    setAxisFont(yDefault, QFont());
}

StdCurvePlot::~StdCurvePlot()
{

}

void StdCurvePlot::setXFun(StdCurvePlot::Function fun)
{
    if (m_xFun == fun)
        return;

    m_xFun = fun;
    setAxisScaleEngine(xDefault, createScaleEngine(fun));
}

void StdCurvePlot::setYFun(StdCurvePlot::Function fun)
{
    if (m_yFun == fun)
        return;

    m_yFun = fun;
    setAxisScaleEngine(yDefault, createScaleEngine(fun));
}

void StdCurvePlot::cleanupCurve(QObject *obj)
{
    StandardCurve *curve = static_cast<StandardCurve*>(obj);
    remove(curve);
}

void StdCurvePlot::add(StandardCurve *curve, const QString &title)
{
    if (m_curves.contains(curve))
        return;

    PlotCurve *pc = new PlotCurve(curve);
    pc->setAxis(xDefault, yDefault);
    pc->setTitle(title);
    pc->attach(this);
    m_curves[curve] = pc;
    connect(curve, SIGNAL(destroyed(QObject*)), this, SLOT(cleanupCurve(QObject*)));

    replot();
}

void StdCurvePlot::remove(StandardCurve *curve)
{
    PlotCurve *pc = m_curves.value(curve);
    if (pc)
    {
        pc->detach();
        delete pc;
    }
    m_curves.remove(curve);

    replot();
}

void StdCurvePlot::setCurvePen(StandardCurve *curve, const QPen &pen)
{
    PlotCurve *pc = m_curves[curve];
    pc->setPen(pen);
    canvas()->replot();
}

void StdCurvePlot::showCurveSymbol(StandardCurve *curve, bool show)
{
    PlotCurve *pc = m_curves[curve];
    pc->showSymbol(show);
    canvas()->replot();
}

void StdCurvePlot::setCurveTitle(StandardCurve *curve, const QString &title)
{
    PlotCurve *pc = m_curves[curve];
    pc->setTitle(title);
}

void StdCurvePlot::clearCurve()
{
    foreach(PlotCurve *c, m_curves)
    {
        if (c->curve())
            c->curve()->disconnect(this);
        delete c;
    }
    m_curves.clear();
    canvas()->replot();
}

QwtScaleEngine *StdCurvePlot::createScaleEngine(Function fun)
{
    switch(fun)
    {
    case weighbridge::FunLog:
        return new QwtLog10ScaleEngine();

    default:
        return new QwtLinearScaleEngine();
    }
}

void StdCurvePlot::setCurve(const QString &curve)
{

}
