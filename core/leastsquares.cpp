/**
* @file leastsquares.cpp
* @brief 高斯消去法解最小二乘系数算法
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include "leastsquares.h"


// 已知n个点, (x[i], y[i]), i{0, 1, ..., n-1}
// 用最小二乘法拟和m-1次曲线:
//		y = a[0]+a[1]*x^1+a[2]*x^2+a[m-1]*x^m-1
// 参数形式 xy 在同一数组里, 这个数组中第一个为x,
// 第二个为y, 共n个点
bool LeastSqs(const PointData *points, double* a, int m)
{
	if (m <= 0)
		return false;

	const int n = points->size();
	double* _a = new double[m*m];
	double* _b = new double[m];
	int i, j;

	for (i=0; i<m; i++)
	{
		for (j=0; j<=i; j++)
		{
			int k = i*m+j;
			int e = i+j;
			_a[k] = 0;
			for (int l=0; l<n; l++)
				_a[k] += (e==0) ? 1 : pow(points->x(l), e);
			_a[j*m+i] = _a[k];
		}
		_b[i] = 0;
		for (j=0; j<n; j++)
			_b[i] += (i==0) ? points->y(j) : (points->y(j)*pow(points->x(j), i));
	}

	bool bOk;

	if (bOk = Gauss(a, _a, _b, m))
	{
	}
	else
	{
		for (i=0; i<m; i++)
			a[i] = 0;
	}

	delete []_a;
	delete []_b;

	return bOk;
}

