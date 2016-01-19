/**
* @file calibrationdocmodel.h
* @brief 标定测量文档model类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-04
*/
#ifndef CALIBRATIONDOCMODEL_H
#define CALIBRATIONDOCMODEL_H

#include "abstractanalysisdocmodel.h"
#include "calibrationdoc.h"

class CalibrationDocModel : public AbstractAnalysisDocModel
{
    Q_OBJECT

public:
    CalibrationDocModel(CalibrationDoc *doc, QObject *parent = 0);
    ~CalibrationDocModel();

    QVariant data(const QModelIndex &index, int role) const;

private:
//    QList<
};

#endif // CALIBRATIONDOCMODEL_H
