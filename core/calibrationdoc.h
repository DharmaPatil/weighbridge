/**
* @file calibrationdoc.h
* @brief 轨道衡标定文档类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef CALIBRATIONDOC_H
#define CALIBRATIONDOC_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include "abstractanalysisdoc.h"

class Workbench;
class Instrument;
class StandardCurve;

class CalibrationDoc : public AbstractAnalysisDoc
{
    Q_OBJECT

public:
    CalibrationDoc(Workbench *parent = 0);
    ~CalibrationDoc();

    void add(const QString &calibration);
    void remove(const QString &calibration);

    bool contains(const QString &calibration) const;
    weighbridge::AnalysisStatus status(const QString &calibration) const;
    QStringList calibrations() const;

    bool canClose(bool &prompted);

signals:
    void finished(const QString &calibration);

private slots:
    void registerCalibration();
    void onStarted();
    void onStopped();
    void onCurveDestroyed();
    void onTaskDestroyed();

private:
    struct CaliData
    {
        QString calibration;		// 定标项目名称
        StandardCurve *curve;		// 新定标曲线
        int finished;
        weighbridge::AnalysisStatus status;

        CaliData(const QString &a);
    };
    typedef QList<CaliData> Data;

    Data m_data;
};

#endif // CALIBRATIONDOC_H
