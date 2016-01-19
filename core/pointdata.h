/**
* @file pointdata.h
* @brief 曲线点类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef POINTDATA_H
#define POINTDATA_H

#include "core_global.h"
#include <QList>
#include <QRectF>

class CORE_EXPORT PointData
{
public:
	virtual ~PointData() {}
	virtual double x(int i) const = 0;
	virtual double y(int i) const = 0;
	virtual int size() const = 0;
	virtual PointData *clone() const = 0;

	virtual QRectF boundingRect() const;
	virtual double sumY() const;
	virtual double sumX() const;
};

template<class T = double>
class ConstYData : public PointData
{
public:
        ConstYData(const T *y, int count, double period, double offset)
                : _y(y), _count(count), _period(period), _offset(offset)
        {}

        double x(int i) const { return _offset + i * _period; }
        double y(int i) const { return _y[i]; }
        int size() const { return _count; }
        PointData *clone() const { return new ConstYData(_y, _count, _period, _offset); }

private:
	const T *_y;
	int _count;
	double _period;
	double _offset;
};

class SubPointData : public PointData
{
public:
        SubPointData(const PointData *source, const QList<int> &index)
		: _source(source), _idx(index)
	{}

	PointData *clone() const { return new SubPointData(_source, _idx); }
	double x(int i) const { return _source->x(_idx[i]); }
	double y(int i) const { return _source->y(_idx[i]); }
	int size() const { return _idx.size(); }

	void setSubIndex(const QList<int> &index) { _idx = index; }

private:
	const PointData *_source;
	QList<int> _idx;
};

#endif // POINTDATA_H
