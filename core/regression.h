/**
* @file regression.h
* @brief 多项式表达式表达式基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef REGRESSION_H
#define REGRESSION_H

#include "core_global.h"
#include "global.h"
#include <QPointF>

class CORE_EXPORT Regression
{
public:
    class FunctionPtr
    {
    public:
        virtual double value(double x) = 0;
        virtual double inverter(double y) = 0;
        virtual double min() const;
        virtual double max() const;
        virtual double epsilon() const;
        virtual ~FunctionPtr() {}
    };
    typedef weighbridge::Function Function;

    Regression(Function xFunc, Function yFunc);
    virtual ~Regression();

    bool isValid() const;
    void setValid(bool v)	{ m_valid = v; }

    void setXFun(Function fun);
    void setYFun(Function fun);

    Function xFun() const	{ return m_xFun; }
    Function yFun() const	{ return m_yFun; }

    bool construct(const double *x, const double *y, int n);
    bool construct(const double *xy, int n);
    bool construct(const QPolygonF &polygon);

    virtual double y(double x) const = 0;
    virtual double x(double y) const = 0;
    virtual bool constructFromTransformed(const double *x, const double *y, int n) = 0;
    virtual int coefficientCount() const = 0;

    void setCoefficient(const QVector<double> &c);
    QVector<double> coefficient() const;
    double coefficient(int i) const;

    double transformX(double x) const { return m_x->value(x); }
    double transformY(double y) const { return m_y->value(y); }
    double invTransformX(double x) const { return m_x->inverter(x); }
    double invTransformY(double y) const { return m_y->inverter(y); }

    FunctionPtr *xFunPtr() const { return m_x; }
    FunctionPtr *yFunPtr() const { return m_y; }

private:
    static FunctionPtr *function(Function f);

    Function m_xFun, m_yFun;
    FunctionPtr *m_x, *m_y;
    QVector<double> m_a;
    bool m_valid;
};

#endif // REGRESSION_H
