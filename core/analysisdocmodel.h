/**
* @file analysisdocmodel.h
* @brief 称重分析文档的model类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef ANALYSISDOCMODEL_H
#define ANALYSISDOCMODEL_H

#include "abstractanalysisdocmodel.h"
#include "analysisdoc.h"

class AnalysisDocModel : public AbstractAnalysisDocModel
{
    Q_OBJECT

public:
    AnalysisDocModel(AnalysisDoc *doc, QObject *parent = 0);
    ~AnalysisDocModel();
};

#endif // ANALYSISDOCMODEL_H
