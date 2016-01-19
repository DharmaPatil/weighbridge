/**
* @file weightdoc.h
* @brief 称重测量文档类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef WEIGHTDCO_H
#define WEIGHTDCO_H

#include "abstractanalysisdoc.h"
#include "analysistemplate.h"

class Workbench;
class AnalysisTemplate;

class WeightDoc : public AbstractAnalysisDoc
{
    Q_OBJECT

public:
    WeightDoc(Workbench *parent = 0);
    ~WeightDoc();

    bool canClose(bool &prompted);

private slots:
    void onStarted();
    void onStopped();
};

#endif // WEIGHTDCO_H
