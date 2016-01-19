/**
* @file calibrationmanager.h
* @brief 轨道衡标定过程参数管理类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/

#include <QtDebug>
#include <QtSql>

#include "global.h"
#include "calibrationmanager.h"
#include "calibrationparam.h"
#include "workbench.h"
#include "profile.h"
#include "wbapp.h"

CalibrationManager::CalibrationManager(QObject *parent)
    : QObject(parent)
{
    load();
}

CalibrationManager::~CalibrationManager()
{
    save();
}

CalibrationParam CalibrationManager::parameter(const QString &calibration) const
{
    QMap<QString, CalibrationParam>::const_iterator it = m_param.find(calibration);
    if (it == m_param.end())
        return initParam(calibration);
    else
        return *it;
}

void CalibrationManager::setParameter(const QString &calibration, const CalibrationParam &p)
{
    m_param[calibration] = p;
}

bool CalibrationManager::canClose(bool &prompted)
{
    prompted = prompted;
    return true;
}

CalibrationParam CalibrationManager::initParam(const QString &calibration) const
{
    CalibrationParam p;
    p.setCalibrationName(calibration);

    return p;
}

void CalibrationManager::save()
{

}

void CalibrationManager::load()
{

}
