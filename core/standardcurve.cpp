#include <QtDebug>
#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <numeric>
#include <float.h>

#include "standardcurve.h"
#include "linearregression.h"
#include "analysistemplate.h"
#include "workbench.h"
#include "wbapp.h"


/**
*
* 计算相关系数.
*
* 参见: http://en.wikipedia.org/wiki/Correlation
*
*                        n * SUM(x[i]*y[i]) - SUM(x[i])*SUM(y[i])
*  r = ----------------------------------------------------------------------------------
*          SQRT( n * SUM(x[i]^2) - SUM(x[i])^2 ) * SQRT( n * SUM(y[i]^2) - SUM(y[i])^2 )
*
**/
template<typename T> T correlation(const T *px, const T *py, int n)
{
    T xy = 0, x = 0, y = 0, x2 = 0, y2 = 0;

    for(int i=0; i<n; i++)
    {
        xy += *px * *py;
        x += *px;
        y += *py;
        x2 += square(*px);
        y2 += square(*py);

        ++px;
        ++py;
    }

    return (n*xy - x*y) /
        sqrt( (n*x2 - square(x)) * (n*y2 - square(y)) );
}

///////////////////////////////////////////////////////////////////////////
/// \brief StandardCurve class implementation
///
StandardCurve::StandardCurve(QObject *parent)
    : QObject(parent)
    , m_date(QDate::currentDate())
    , m_modified(false)
    , m_regression(0)
    , m_valid(false)
{
    setMethod(weighbridge::RegLinear);
}

StandardCurve::StandardCurve(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_date(QDate::currentDate())
    , m_modified(false)
    , m_regression(0)
    , m_valid(false)
{
    setMethod(weighbridge::RegLinear);
}

StandardCurve::~StandardCurve()
{
    qDelete(m_regression);
}

bool StandardCurve::isValid() const
{
    return m_valid && m_regression && m_regression->isValid();
}

void StandardCurve::setValid(bool valid)
{
    m_valid = valid;
    if (m_regression)
        m_regression->setValid(valid);
}

void StandardCurve::setMethod(weighbridge::RegressionMethod method)
{
    m_method = method;

    Regression *reg = 0;
    switch(m_method)
    {
    case weighbridge::RegLinear:
        reg = new LinearRegression(static_cast<weighbridge::Function>(xFun()),
                                   static_cast<weighbridge::Function>(yFun()));
        break;
    case weighbridge::Reg4PL:
        break;
    default:
        Q_ASSERT(!"invalid method");
        break;
    }

    qDelete(m_regression);
    m_regression = reg;
}

void StandardCurve::setXFun(weighbridge::Function xFun)
{
    m_xFun = xFun;
    if (m_regression)
        m_regression->setXFun(xFun);
}

void StandardCurve::setYFun(weighbridge::Function yFun)
{
    m_yFun = yFun;
    if (m_regression)
        m_regression->setYFun(yFun);
}

int StandardCurve::coefficientCount() const
{
    return m_regression ? m_regression->coefficientCount() : 0;
}

void StandardCurve::setCoefficient(const QVector<double> &c)
{
    m_coef = c;
    if (m_regression)
        m_regression->setCoefficient(c);
}

void StandardCurve::addPoint(const StdPoint &point)
{
    m_points.append(point);
    setValid(false);
}

void StandardCurve::removePoint(int i)
{
    m_points.remove(i);
    setValid(false);
}


void StandardCurve::update()
{
    if (isValid())
        return;

    static const double epsilon = 1e-300;

    // 收集有效数据
    QVector<double> x, y;
    for(int i=0; i < m_points.size(); ++i)
    {
        const StdPoint &p = m_points[i];

        if (p.isNull())
            continue;

        if (xFun() == weighbridge::FunLog && p.x() < 0 ||
                yFun() == weighbridge::FunLog && p.y() <= 0)
            continue;

        if (xFun() == weighbridge::FunLog && p.x() < epsilon)
            x.append(m_regression->transformX(epsilon));
        else
            x.append(m_regression->transformX(p.x()));

        if (yFun() == weighbridge::FunLog && p.y() < epsilon)
            y.append(m_regression->transformY(epsilon));
        else
            y.append(m_regression->transformY(p.y()));
    }

    if (x.size() >= 2)
    {
        m_correlation = qAbs(::correlation(x.constData(), y.constData(), x.size()));
        if (_isnan(m_correlation))
            m_correlation = 0;
    }

    m_regression->constructFromTransformed(x.constData(), y.constData(), x.size());
    m_coef = m_regression->coefficient();
    m_valid = m_regression->isValid();

    if (!isValid())
        return;

    for(int i=0; i < m_points.size(); i++)
    {
        if (!m_points[i].isNull())
            m_points[i].setZ(this->y(m_points[i].x()));
    }
}

double StandardCurve::x(double y) const
{
    return m_regression->x(y);
}

double StandardCurve::y(double x) const
{
    return m_regression->y(x);
}

double StandardCurve::minDisplayX() const
{
    if (xFun() == weighbridge::FunLog)
        return 0.001;
    else
        return -DBL_MAX;
}

double StandardCurve::minDisplayY() const
{
    if (yFun() == weighbridge::FunLog)
        return 0.001;
    else
        return -DBL_MAX;
}

double StandardCurve::boundX(double x) const
{
    return qMax(minDisplayX(), x);
}

double StandardCurve::boundY(double y) const
{
    return qMax(minDisplayY(), y);
}

void StandardCurve::addResult()
{

}

// 所有标定项目测试完成后调用
void StandardCurve::finish()
{
    update();
}

// 曲线被确认
void StandardCurve::validate()
{
}

QVariant StandardCurve::var(const QString &name) const
{
    return m_vars.value(name);
}

void StandardCurve::setVar(const QString &name, const QVariant &val)
{
    m_vars[name] = val;
}

QStringList StandardCurve::vars() const
{
    return m_vars.keys();
}
