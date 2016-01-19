/**
* @file weightdocmodel.cpp
* @brief 称重测量文档model类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include <QDebug>

#include "abstractanalysisdoc.h"
#include "abstractanalysisdocmodel.h"
#include "weightdocmodel.h"
#include "analysistemplate.h"
#include "analysistemplatemanager.h"
#include "profile.h"
#include "global.h"


////////////////////////////////////////////////////////////////////
/// \brief WeightDocModel class
///
WeightDocModel::WeightDocModel(WeightDoc *doc, QObject *parent)
    : AbstractAnalysisDocModel(doc, parent)

{
    m_headerTitle = profile.analysis_display_assay().split(';', QString::SkipEmptyParts);
}

WeightDocModel::~WeightDocModel()
{

}

void WeightDocModel::insertResultData(QStringList &data, int row, const QModelIndex &parent)
{

}

void WeightDocModel::load(const QString &filename)
{
    m_filename = filename;
}

void WeightDocModel::save(const QString &filename)
{

}

