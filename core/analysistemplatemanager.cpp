/**
* @file analysistemplatemanager.h
* @brief 测试数据分析方法管理类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/

#include "analysistemplatemanager.h"
#include "global.h"

AnalysisTemplateManager::AnalysisTemplateManager(QObject *parent) : QObject(parent)
{

}

AnalysisTemplateManager::~AnalysisTemplateManager()
{

}

void AnalysisTemplateManager::add(AnalysisTemplate *at)
{

}

QList<AnalysisTemplate*>AnalysisTemplateManager:: methodes() const
{
	return m_itemList;
}

AnalysisTemplate *AnalysisTemplateManager::method(int i) const
{
	return m_itemList[i];
}

AnalysisTemplate *AnalysisTemplateManager::method(QString &name) const
{
	foreach(AnalysisTemplate *t, m_itemList)
	{
        if (t->name() == name)
            return t;
	}
    return 0;
}

void AnalysisTemplateManager::remove(int i)
{
    AnalysisTemplate *t = m_itemList.takeAt(i);
    delete t;
}

int AnalysisTemplateManager::count() const
{
    return  m_itemList.count();
}

void AnalysisTemplateManager::save()
{

}

void AnalysisTemplateManager::load()
{

}
