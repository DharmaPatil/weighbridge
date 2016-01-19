/**
* @file regression.cpp
* @brief 多项式表达式类缺省实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include "regression.h"
#include "leastsquares.h"
#include "pointdata.h"
#include <memory.h>
#include <limits.h>
#include <float.h>
#include <valarray>
#include <QPolygonF>

double Regression::FunctionPtr::min() const
{
    return -DBL_MAX;
}

double Regression::FunctionPtr::max() const
{
    return DBL_MAX;
}

double Regression::FunctionPtr::epsilon() const
{
    return DBL_EPSILON;
}

class LinearFunction : public Regression::FunctionPtr
{
public:
    double value(double x) { return x; }
    double inverter(double y) { return y; }
};

class LogFunction : public Regression::FunctionPtr
{
public:
    double value(double x) { return log10(x); }
    double inverter(double y) { return pow(10, y); }
    double min() const { return DBL_MIN; }
};

///////////////////////////////////////////////////////////////
/// \brief Regression class
///
Regression::Regression(weighbridge::Function xFun, weighbridge::Function yFun)
    : m_valid(false)
    , m_x(0)
    , m_y(0)
{
    setXFun(xFun);
    setYFun(yFun);
}

Regression::~Regression()
{
    qDelete(m_x);
    qDelete(m_y);
}

Regression::FunctionPtr *Regression::function(weighbridge::Function f)
{
    switch(f)
    {
    case weighbridge::FunLog:
        return new LogFunction;

    case weighbridge::FunLinear:
    default:
        return new LinearFunction;
    }
}

void Regression::setXFun(weighbridge::Function fun)
{
    m_xFun = fun;
    qDelete(m_x);
    m_x = function(m_xFun);
}

void Regression::setYFun(weighbridge::Function fun)
{
    m_yFun = fun;
    qDelete(m_y);
    m_y = function(m_yFun);
}

bool Regression::isValid() const
{
    return m_valid && m_a.size() == coefficientCount();
}

bool Regression::construct(const double *x, const double *y, int n)
{
    std::valarray<double> xa(n), ya(n);

    for(int i=0; i<n; i++)
    {
        xa[i] = transformX(*x++);
        ya[i] = transformY(*y++);
    }

    return constructFromTransformed(&xa[0], &ya[0], n);
}

bool Regression::construct(const double *xy, int n)
{
    std::valarray<double> xa(n), ya(n);

    for(int i=0; i<n; i++)
    {
        xa[i] = transformX(xy[0]);
        ya[i] = transformY(xy[1]);
        ++xy;
    }

    return constructFromTransformed(&xa[0], &ya[0], n);
}

bool Regression::construct(const QPolygonF &polygon)
{
    int n = polygon.size();
    std::valarray<double> xa(n), ya(n);

    for(int i=0; i<n; i++)
    {
        xa[i] = transformX(polygon[i].x());
        ya[i] = transformY(polygon[i].y());
    }

    return constructFromTransformed(&xa[0], &ya[0], n);
}


void Regression::setCoefficient(const QVector<double> &c)
{
    m_a = c;
    if (m_a.size() > coefficientCount())
        m_a.resize(coefficientCount());
}

QVector<double> Regression::coefficient() const
{
    return m_a;
}

double Regression::coefficient(int i) const
{
    return m_a.value(i);
}
