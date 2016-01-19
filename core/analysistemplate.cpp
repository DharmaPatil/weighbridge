/**
* @file analysistemplate.cpp
* @brief 测试数据分析方法基类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include <QDebug>
#include <QVariant>
#include <QScriptEngine>
#include <QStack>
#include <QBitArray>
#include <QtSql>
#include <float.h>

#include "analysistemplate.h"
#include "analysistemplatemanager.h"
#include "standardcurve.h"
//#include "standardcurvemanager.h"
#include "workbench.h"
#include "instrument.h"
#include "wbapp.h"
#include "global.h"

AnalysisTemplate::AnalysisTemplate()
    : QObject()
    , m_id(0)
{

}

AnalysisTemplate::~AnalysisTemplate()
{

}

AnalysisTemplateManager *AnalysisTemplate::manager() const
{
    return static_cast<AnalysisTemplateManager *>(parent());
}
