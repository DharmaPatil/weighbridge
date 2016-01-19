/**
* @file analysisresult.h
* @brief 轨道衡测量结果分析类定义
* @ingroup common
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include <QVariant>
#include <QAtomicInt>
#include <QMap>

#include "global.h"

/** @brief 测量曲线 */
class COMMON_EXPORT MeasureCurve
{

};
Q_DECLARE_METATYPE(MeasureCurve)

/** @brief 测量结果 */
class COMMON_EXPORT AnalysisResult
{
public:
    AnalysisResult();
    ~AnalysisResult();
};
Q_DECLARE_METATYPE(AnalysisResult)

#endif // ANALYSISRESULT_H
