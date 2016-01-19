/**
* @file calibrationdoc.cpp
* @brief 轨道衡标定文档类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include "calibrationdoc.h"
#include "workbench.h"

CalibrationDoc::CalibrationDoc(Workbench *parent)
    : AbstractAnalysisDoc(parent)
{

}

CalibrationDoc::~CalibrationDoc()
{

}

void CalibrationDoc::add(const QString &calibration)
{
    Q_UNUSED(calibration)
}

void CalibrationDoc::remove(const QString &calibration)
{
    Q_UNUSED(calibration)
}

bool CalibrationDoc::contains(const QString &calibration) const
{
    Q_UNUSED(calibration)
    return true;
}

weighbridge::AnalysisStatus CalibrationDoc::status(const QString &calibration) const
{
    Q_UNUSED(calibration)
    return (weighbridge::AnalysisStatus::Ready);
}

QStringList CalibrationDoc::calibrations() const
{
    QStringList list;
    return list;
}

bool CalibrationDoc::canClose(bool &prompted)
{
    prompted = prompted;
    return true;
}

void CalibrationDoc::registerCalibration()
{

}

void CalibrationDoc::onStarted()
{

}

void CalibrationDoc::onStopped()
{

}

void CalibrationDoc::onCurveDestroyed()
{

}

void CalibrationDoc::onTaskDestroyed()
{

}
