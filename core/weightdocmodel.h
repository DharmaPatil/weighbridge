/**
* @file weightdocmodel.h
* @brief 称重测量文档model类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef WEIGHTMODEL_H
#define WEIGHTMODEL_H

#include "abstractanalysisdocmodel.h"
#include "weightdoc.h"


///////////////////////////////////////////////////////////////////////////
/// \brief The WeightDocModel class
///
class CORE_EXPORT WeightDocModel : public AbstractAnalysisDocModel
{
	Q_OBJECT

public:
    WeightDocModel(WeightDoc *doc, QObject *parent = 0);
    ~WeightDocModel();

    void insertResultData(QStringList &data, int row, const QModelIndex &parent = QModelIndex());

    QString filename() const { return m_filename; }

private slots:
    void load(const QString &filename=QString());
    void save(const QString &filename=QString());

private:
    QString m_filename;
};

#endif // WEIGHTMODEL_H
