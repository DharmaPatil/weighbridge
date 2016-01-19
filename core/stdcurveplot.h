/**
* @file stdcurveplot.h
* @brief 标准曲线坐标类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef STDCURVEPLOT_H
#define STDCURVEPLOT_H

#include <QMap>
#include <qwt_plot.h>
#include "core_global.h"
#include "global.h"

class StandardCurve;
class PlotCurve;

///////////////////////////////////////////////////
/// \brief The StdCurvePlot class 标准曲线控件
///
class CORE_EXPORT StdCurvePlot : public QwtPlot
{
    Q_OBJECT

public:
    enum DefaultAxis { xDefault = xBottom, yDefault = yRight };
    typedef weighbridge::Function Function;

    StdCurvePlot(QWidget *parent);
    ~StdCurvePlot();

    void setXFun(Function fun);
    void setYFun(Function fun);
    Function xFun() const	{ return m_xFun; }
    Function yFun() const	{ return m_yFun; }

    void add(StandardCurve *curve, const QString &title = QString());
    void remove(StandardCurve *curve);
    void setCurvePen(StandardCurve *curve, const QPen &pen);
    void showCurveSymbol(StandardCurve *curve, bool show);
    void setCurveTitle(StandardCurve *curve, const QString &title);
    void clearCurve();
    void setCurve(const QString &curve);

private slots:
    void cleanupCurve(QObject *obj);

private:
    QMap<StandardCurve*, PlotCurve*> m_curves;
    Function m_xFun, m_yFun;

    QwtScaleEngine *createScaleEngine(Function fun);
};

#endif // STDCURVEPLOT_H
