/**
* @file analysisdoc.h
* @brief 称重分析文档model类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include <QtDebug>

#include "global.h"
#include "profile.h"
#include "abstractanalysisdoc.h"
#include "workbench.h"
#include "analysisdocmodel.h"

AnalysisDocModel::AnalysisDocModel(AnalysisDoc *doc, QObject *parent)
    : AbstractAnalysisDocModel(doc, parent)
{

}

AnalysisDocModel::~AnalysisDocModel()
{

}

