#include "weightdoc.h"
#include "workbench.h"

WeightDoc::WeightDoc(Workbench *parent)
    : AbstractAnalysisDoc(parent)
{

}

WeightDoc::~WeightDoc()
{

}

bool WeightDoc::canClose(bool &prompted)
{
    if (isRunning())
    {
        prompted = true;
        if (workbench()->question("正在测试, 是否退出？") == QMessageBox::No)
            return false;
    }
    return true;
}

void WeightDoc::onStarted()
{

}

void WeightDoc::onStopped()
{

}
