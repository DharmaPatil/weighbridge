/**
* @file linearregression.h
* @brief 线性回归算法类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include "core_global.h"
#include "regression.h"

class CORE_EXPORT LinearRegression : public Regression
{
public:
	enum { Coefficient = 2 };

    LinearRegression(Function xFun = weighbridge::FunLinear, Function yFun = weighbridge::FunLinear);

	double y(double x) const;
	double x(double y) const;
	bool constructFromTransformed(const double *x, const double *y, int n);
	int coefficientCount() const { return Coefficient; }
};

#endif // LINEARREGRESSION_H
