/**
* @file analysisdoc.h
* @brief 称重分析测量文档类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef ANALYSISDOC_H
#define ANALYSISDOC_H

#include "abstractanalysisdoc.h"
#include "global.h"
#include <QDate>
#include <QVector>

class CORE_EXPORT AnalysisDoc : public AbstractAnalysisDoc
{
    Q_OBJECT

public:
    AnalysisDoc(Workbench *parent = 0);
    ~AnalysisDoc();

    void load(QString &filename = QString());
    void save(QString &filename = QString());

public slots:

};

#endif // ANALYSISDOC_H
