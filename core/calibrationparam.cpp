/**
* @file calibrationparam.cpp
* @brief 定标参数设置类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/

#include "calibrationparam.h"

CalibrationParam::CalibrationParam()
    : d(new Data)
{
    d->ref = 1;
}

CalibrationParam::CalibrationParam(const CalibrationParam &other)
    : d(other.d)
{
    d->ref.ref();
}

CalibrationParam& CalibrationParam::operator = (const CalibrationParam &other)
{
    Data *dd = d;

    d = other.d;
    d->ref.ref();

    if (!dd->ref.deref())
        delete dd;

    return *this;
}

CalibrationParam::~CalibrationParam()
{
    if (!d->ref.deref())
        delete d;
}

QString CalibrationParam::calibratorPosition(const QString &c) const
{
    foreach(const Calibrator &cr, calibrators())
        if (cr.name == c)
            return cr.position;
    return QString();
}

void CalibrationParam::detach()
{
    if (d->ref != 1)
    {
        Data *dd = d;

        d = new Data(*dd);
        d->ref = 1;

        dd->ref.deref();
    }
}
