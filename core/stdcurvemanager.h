/**
* @file stdcurvemanager.h
* @brief 标准曲线管理类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef STDCURVEMANAGER_H
#define STDCURVEMANAGER_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include "global.h"

class StandardCurve;
class Workbench;

class StdCurveManager : public QObject
{
    Q_OBJECT
public:
    enum Status { Calibrating, ToBeValidated, Validated };

    explicit StdCurveManager(Workbench *parent = 0);
    ~StdCurveManager();

    Workbench *workbench() const;

    StandardCurve *curve(const QString &name) const;
    StandardCurve *newCurve(const QString &name);
    Status curveStatus(const QString &name) const;
    QString curveStatusString(const QString &name) const;
    void setCurve(const QString &name, StandardCurve *curve);
    void setNewCurve(const QString &name, StandardCurve *curve);
    void setCurveStatus(const QString &name, StdCurveManager::Status &s);
    void clear();

    StandardCurve *newCalibration(const QString &calibration);
    void finishCalibration(const QString &calibration);
    void cancleCalibration(const QString &calibration);
    void validate(const QString &calibration);

    bool canClose(bool &prompted);

    static StandardCurve *createCurve(const QString &caliName, QObject *parent = 0);

signals:
    void curveStatusChanged(const QString &assay, StdCurveManager::Status status);

public slots:
    void save();
    void load();

private:
    void load(const QString &uuid);
    void save(const QString &uuid, bool force = false);

    struct Curves {
        StandardCurve *current, *recent;	// 当前曲线和新标定(未确认)曲线
        Status status;
        bool dirty;
        Curves() : current(0), recent(0), status(Validated), dirty(false) {}
    };
    typedef QMap<QString, Curves> CurveMap;

    CurveMap m_curves;
    QUuid m_uuid;
};

#endif // STDCURVEMANAGER_H
