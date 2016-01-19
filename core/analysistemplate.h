/**
* @file analysistemplate.h
* @brief 测试数据分析方法基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef ANALYSISTEMPLATE_H
#define ANALYSISTEMPLATE_H

#include <QObject>
#include <QStringList>
#include <QVector>

#include "core_global.h"
#include "global.h"
#include "regression.h"

class StandardCurve;
class AnalysisResult;
class AnalysisTemplateManager;
class Workbench;

struct DataCheck
{

};

class CORE_EXPORT AnalysisTemplate : public QObject
{
public:
    explicit AnalysisTemplate();
    ~AnalysisTemplate();

    AnalysisTemplateManager *manager() const;

    int id() const { return m_id; }
    QString name() const { return m_name; }
    void setId(int i) { m_id = i; }
    void setName(const QString &name) { m_name = name; }

private:
    int m_id;       // 方法id
    QString m_name; //名称

};

#endif // ANALYSISTEMPLATE_H
