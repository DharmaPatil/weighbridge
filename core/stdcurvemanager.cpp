/**
* @file stdcurvemanager.cpp
* @brief 标准曲线管理类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/

#include <QtDebug>
#include <QVariant>
#include <QtSql>
#include <QString>

#include "stdcurvemanager.h"
#include "standardcurve.h"
#include "global.h"
#include "workbench.h"
#include "calibrationmanager.h"
#include "calibrationparam.h"
#include "profile.h"
#include "instrument.h"
#include "wbapp.h"

StdCurveManager::StdCurveManager(Workbench *parent)
    : QObject(parent)
{
    load();
}

StdCurveManager::~StdCurveManager()
{
    save();
}

Workbench *StdCurveManager::workbench() const
{
    return static_cast<Workbench*>(parent());
}

StandardCurve *StdCurveManager::curve(const QString &name) const
{
    return m_curves.value(name).current;
}

StandardCurve *StdCurveManager::newCurve(const QString &name)
{
    return m_curves.value(name).recent;
}

StdCurveManager::Status StdCurveManager::curveStatus(const QString &name) const
{
    return m_curves.value(name).status;
}

QString StdCurveManager::curveStatusString(const QString &name) const
{
    Status s = curveStatus(name);
    switch(s)
    {
    case Calibrating:
        return tr("正在标定");
    case ToBeValidated:
        return tr("确认");
    case Validated:
        return tr("完成");
    default:
        return QString();
    }
}

void StdCurveManager::setCurve(const QString &assay, StandardCurve *curve)
{
    Curves &c = m_curves[assay];
    c.current = curve;
}

void StdCurveManager::setNewCurve(const QString &assay, StandardCurve *curve)
{
    Curves &c = m_curves[assay];
    c.recent = curve;
}

void StdCurveManager::setCurveStatus(const QString &name, StdCurveManager::Status &s)
{
    Curves &curve = m_curves[name];
    if (curve.status == s)
        return;

    curve.status = s;
    emit curveStatusChanged(name, s);
}

void StdCurveManager::clear()
{
    for(CurveMap::iterator it = m_curves.begin(); it != m_curves.end(); ++it)
    {
        if (it->current)
            it->current->deleteLater();
        if (it->recent)
            it->recent->deleteLater();
    }
    m_curves.clear();
}

StandardCurve *StdCurveManager::newCalibration(const QString &calibration)
{
    StandardCurve *cv = createCurve(calibration, this);
    return cv;
}

void StdCurveManager::finishCalibration(const QString &calibration)
{
    Q_UNUSED(calibration)
}

void StdCurveManager::cancleCalibration(const QString &calibration)
{
    Q_UNUSED(calibration)
}

void StdCurveManager::validate(const QString &calibration)
{
    Q_UNUSED(calibration)
}

bool StdCurveManager::canClose(bool &prompted)
{
    prompted = prompted;
    return true;
}

void StdCurveManager::save()
{

}

void StdCurveManager::load()
{

}

void StdCurveManager::load(const QString &uuid)
{
    Q_UNUSED(uuid)
}

void StdCurveManager::save(const QString &uuid, bool force)
{
    Q_UNUSED(uuid)
    Q_UNUSED(force)
}

StandardCurve *StdCurveManager::createCurve(const QString &caliName, QObject *parent)
{
    return new StandardCurve(caliName, parent);
}
