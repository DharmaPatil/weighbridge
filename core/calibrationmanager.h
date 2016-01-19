/**
* @file calibrationmanager.h
* @brief 轨道衡标定过程参数管理类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef CALIBRATIONMANAGER_H
#define CALIBRATIONMANAGER_H

#include <QObject>
#include <QMap>

class CalibrationParam;

class CalibrationManager : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationManager(QObject *parent = 0);
    ~CalibrationManager();

    CalibrationParam parameter(const QString &calibration) const;
    void setParameter(const QString &calibration, const CalibrationParam &p);

    bool canClose(bool &prompted);

private:
    CalibrationParam initParam(const QString &calibration) const;
    void save();
    void load();

private:
    QMap<QString, CalibrationParam> m_param;
};

#endif // CALIBRATIONMANAGER_H
