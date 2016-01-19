/**
* @file linearregression.cpp
* @brief 线性回归算法类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include <memory.h>
#include <limits.h>
#include <float.h>
#include <valarray>
#include <QPolygonF>
#include "linearregression.h"
#include "leastsquares.h"
#include "pointdata.h"

LinearRegression::LinearRegression(weighbridge::Function xFun, weighbridge::Function yFun)
	: Regression(xFun, yFun)
{
}

bool LinearRegression::constructFromTransformed(const double *x, const double *y, int n)
{
	QVector<double> a(coefficientCount());
	bool valid = LeastSqs(x, y, n, a.data(), a.size());
	if (valid)
		setCoefficient(a);
	setValid(valid);

	return valid;
}

double LinearRegression::y(double x) const
{
	Q_ASSERT(isValid());
	double y = polynomial<double>(coefficient().constData(), coefficient().size(), transformX(x));
	return invTransformY(y);
}

double LinearRegression::x(double y) const
{
	Q_ASSERT(isValid());
	y = transformY(y);
	double x = (y - coefficient(0))/coefficient(1);
	return invTransformX(x);
}
