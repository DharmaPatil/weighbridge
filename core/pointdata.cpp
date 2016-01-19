/**
* @file pointdata.cpp
* @brief 曲线点类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include "pointdata.h"

////////////////////////////////////////////////////////////////////////////////
// PointData
QRectF PointData::boundingRect() const
{
	if (!size())
		return QRectF();

	double xmin, xmax, ymin, ymax;
	xmin = xmax = x(0);
	ymin = ymax = y(0);
	for(int i=1; i<size(); ++i)
	{
		double xi = x(i);
		if (xmin > xi)
			xmin = xi;
		else if (xmax < xi)
			xmax = xi;

		double yi = y(i);
		if (ymin > yi)
			ymin = yi;
		else if (ymax < yi)
			ymax = yi;
	}

	return QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

double PointData::sumY() const
{
	double s = 0;
	for(int i=0; i<size(); ++i)
		s += y(i);
	return s;
}

double PointData::sumX() const
{
	double s = 0;
	for(int i=0; i<size(); ++i)
		s += x(i);
	return s;
}
