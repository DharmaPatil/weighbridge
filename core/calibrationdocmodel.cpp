/**
* @file calibrationdocmodel.cpp
* @brief 标定测量文档model类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-04
*/
#include "calibrationdocmodel.h"
#include "calibrationdoc.h"
#include "profile.h"
#include "workbench.h"


CalibrationDocModel::CalibrationDocModel(CalibrationDoc *doc, QObject *parent)
    : AbstractAnalysisDocModel(doc, parent)
{
    m_headerTitle << tr("序号") << tr("车型") << tr("车号") << tr("重量") << tr("速度");
}

CalibrationDocModel::~CalibrationDocModel()
{

}

QVariant CalibrationDocModel::data(const QModelIndex &index, int role) const
{
	return QVariant();
}
