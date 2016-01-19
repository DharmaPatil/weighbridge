/**
* @file calibrationparam.h
* @brief 定标参数设置类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef CALIBRATIONPARAM_H
#define CALIBRATIONPARAM_H

#include <QAtomicInt>
#include <QString>
#include <QList>

class CalibrationParam
{
public:
    //标准品
    struct Calibrator
    {
        QString name;
        double value;
        QString position;
    };
    typedef QList<Calibrator> Calibrators;

    //定标参数
    struct Setting
    {
        QString name;
        double value;
        double factor;
    };
    typedef QList<Setting> Settings;

    CalibrationParam();
    CalibrationParam(const CalibrationParam &other);
    CalibrationParam &operator=(const CalibrationParam &other);
    ~CalibrationParam();

    QString calibrationName() const { return d->calibrationName; }
    QString xUnit() const { return d->xunit; }
    QString yUnit() const { return d->yunit; }
    Calibrators calibrators() const { return d->calibrators; }
    Settings settings() const { return d->settings; }

    void setCalibrationName(const QString &name) { detach(); d->calibrationName = name; }
    void setXUnit(const QString &unit) { detach(); d->xunit = unit; }
    void setYUnit(const QString &unit) { detach(); d->yunit = unit; }
    void setCalibrators(const Calibrators &c) { detach(); d->calibrators = c; }
    void setSettings(const Settings &settings) { detach(); d->settings = settings; }

    void insertCalibrator(int i, const Calibrator &c) { detach(); d->calibrators.insert(i, c); }
    void setCalibratorName(int i, const QString &s) { detach(); d->calibrators[i].name = s; }
    void setCalibratorValue(int i, double v) { detach(); d->calibrators[i].value = v; }
    void setCalibratorPosition(int i, const QString &pos) { detach(); d->calibrators[i].position = pos; }

    void insertSetting(int i, const Setting &s) { detach(); d->settings.insert(i, s); }
    void setSettingName(int i, const QString &s) { detach(); d->settings[i].name = s; }
    void setSettingValue(int i, double v) { detach(); d->settings[i].value = v; }
    void setSettingFactor(int i, double f) { detach(); d->settings[i].factor = f; }

    QString calibratorPosition(const QString &c) const;

private:
    struct Data
    {
        QAtomicInt ref;

        QString calibrationName;	// 标定名称
        QString xunit;				// X轴单位
        QString yunit;				// 标准品单位

        Calibrators calibrators;	// 标准品列表
        Settings settings;			// 标定设置
    };

    Data *d;

    void detach();
};

#endif // CALIBRATIONPARAM_H
