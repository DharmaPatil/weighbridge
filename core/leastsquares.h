/**
* @file leastsquares.h
* @brief 高斯消去法解最小二乘系数算法类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef LEASTSQUARES_H
#define LEASTSQUARES_H

#include "pointdata.h"
#include <math.h>
#include <float.h>

template<class T>
struct epsilon
{
	static const T value;
};

template<> const double epsilon<double>::value = DBL_EPSILON;
template<> const float epsilon<float>::value = FLT_EPSILON;

// 用高斯消去法解齐次方程, 结果存在b中
//	   方程形如 Ax=B
// 其中A为系数矩阵
template<class T>
bool Gauss(T *b, const T* A, const T* B, int n, int stride = 0)
{
	if (!stride)
		stride = n;

	int *js,l,k,i,j,is,p,q;
	T d,t;

	memcpy(b, B, n * sizeof(T));
	T *a = new T[n * n];
	for(i=0; i<n; ++i)
		memcpy(a + i * n, A + i * stride, n * sizeof(T));

	js=new int[n];
	l=1;
	for (k=0;k<=n-2;k++)
	{
		d=0.0;
		for (i=k;i<=n-1;i++)
		{
			for (j=k;j<=n-1;j++)
			{
				t=fabs(a[i*n+j]);
				if (t>d)
				{
					d=t; js[k]=j; is=i;
				}
			}
		}

		if (fabs(d) < epsilon<T>::value)
			l=0;
		else
		{
			if (js[k]!=k)
			{
				for (i=0;i<=n-1;i++)
				{
					p=i*n+k; q=i*n+js[k];
					t=a[p]; a[p]=a[q]; a[q]=t;
				}
			}
			if (is!=k)
			{
				for (j=k;j<=n-1;j++)
				{
					p=k*n+j; q=is*n+j;
					t=a[p]; a[p]=a[q]; a[q]=t;
				}
				t=b[k]; b[k]=b[is]; b[is]=t;
			}
		}

		if (l==0)
		{
			delete []js;
			delete []a;
			return false;
		}

		d=a[k*n+k];
		for (j=k+1;j<=n-1;j++)
		{
			p=k*n+j; a[p]=a[p]/d;
		}
		b[k]=b[k]/d;
		for (i=k+1;i<=n-1;i++)
		{
			for (j=k+1;j<=n-1;j++)
			{
				p=i*n+j;
				a[p]=a[p]-a[i*n+k]*a[k*n+j];
			}
			b[i]=b[i]-a[i*n+k]*b[k];
		}
	}
	d=a[(n-1)*n+n-1];
	if (fabs(d) < epsilon<T>::value)
	{
		delete []js;
		delete []a;
		return false;
	}
	b[n-1]=b[n-1]/d;
	for (i=n-2;i>=0;i--)
	{
		t=0.0;
		for (j=i+1;j<=n-1;j++)
			t=t+a[i*n+j]*b[j];
		b[i]=b[i]-t;
	}
	js[n-1]=n-1;
	for (k=n-1;k>=0;k--)
	{
		if (js[k]!=k)
		{
			t=b[k]; b[k]=b[js[k]]; b[js[k]]=t;
		}
	}
	delete []js;
	delete []a;

	return true;
}

// 计算多项式
//	 y = a[0] + a[1]*x + a[2]*x^2 + ... + a[n-1]*x^n-1在x处的值
template<class TYPE>
TYPE polynomial(const TYPE* a, int n, TYPE x)
{
	TYPE y = 0;
	for (int i=n-1; i>=0; i--)
		y = y*x+a[i];

	return y;
}

// 已知n个点, (x[i], y[i]), i{0, 1, ..., n-1}
// 用最小二乘法拟和m-1次曲线:
//		y = a[0]+a[1]*x^1+a[2]*x^2+a[m-1]*x^m-1
// 参数形式 x 一个数组, y 为一数组
template<int M>
class LeastSquares
{
public:
	LeastSquares()
	{
		reset();
	}

	bool isValid() const { return m_valid; }
	double coefficient(int i) const { return m_coeficient[i]; }
	const double *coefficient() const { return m_coeficient; }

	void reset()
	{
		m_valid = false;
		memset(A, 0, sizeof(A));
		memset(B, 0, sizeof(B));
		memset(m_coeficient, 0, sizeof(m_coeficient));
	}

	void update(double x, double y, double w = 1)
	{
		for (int i=0; i<M; ++i)
		{
			for (int j=0; j<=i; ++j)
			{
				int k = i*M+j;
				int e = i+j;
				A[k] += (e==0) ? w : w * pow(x, e);
				A[j*M+i] = A[k];
			}
			B[i] += (i==0) ? w * y : w * (y * pow(x, i));
		}
	}

	bool update()
	{
		m_valid = Gauss(m_coeficient, A, B, M);
		return m_valid;
	}

	double y(double x) const
	{
		double r = 0;
		for(int i=M-1; i>=0; --i)
			r += r * x + coefficient(i);
		return r;
	}

private:
	double A[M*M];
	double B[M];
	double m_coeficient[M];
	bool m_valid;
};

typedef LeastSquares<2> LinearLeastSquares;


// 已知n个点, (x[i], y[i]), i{0, 1, ..., n-1}
// 用最小二乘法拟和m-1次曲线:
//		y = a[0]+a[1]*x^1+a[2]*x^2+a[m-1]*x^m-1
// 参数形式 x 一个数组, y 为一数组
template<class TYPE>
bool LeastSqs(const TYPE* x, const TYPE* y, int n, TYPE* a, int m)
{
	if (m <= 0)
		return false;

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
				_a[k] += (e==0) ? 1 : pow(x[l], e);
			_a[j*m+i] = _a[k];
		}
		_b[i] = 0;
		for (j=0; j<n; j++)
			_b[i] += (i==0) ? y[j] : (y[j]*pow(x[j], i));
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


// 已知n个点, (x[i], y[i]), i{0, 1, ..., n-1}
// 用最小二乘法拟和m-1次曲线:
//		y = a[0]+a[1]*x^1+a[2]*x^2+a[m-1]*x^m-1
// 参数形式 xy 在同一数组里, 这个数组中第一个为x,
// 第二个为y, 共n个点
template<class TYPE>
bool LeastSqs(const TYPE* xy, int n, TYPE* a, int m)
{
	if (m <= 0)
		return false;

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
				_a[k] += (e==0) ? 1 : pow(xy[2*l], e);
			_a[j*m+i] = _a[k];
		}
		_b[i] = 0;
		for (j=0; j<n; j++)
			_b[i] += (i==0) ? xy[2*j+1] : (xy[2*j+1]*pow(xy[2*j], i));
	}

	bool bOk;

	if (bOk = Gauss(_a, _b, m))
	{
		for (i=0; i<m; i++)
			a[i] = (TYPE)_b[i];
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

bool LeastSqs(const PointData *points, double* a, int m);

#endif // LEASTSQUARES_H
