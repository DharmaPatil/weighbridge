/**
* @file analysistemplatemanager.h
* @brief 测试数据分析方法管理类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef ANALYSISTEMPLATEMANAGER_H
#define ANALYSISTEMPLATEMANAGER_H

#include <QObject>
#include <QList>

#include "analysistemplate.h"

class AnalysisTemplate;

class CORE_EXPORT AnalysisTemplateManager : public QObject
{
    Q_OBJECT
public:
    explicit AnalysisTemplateManager(QObject *parent = 0);
    ~AnalysisTemplateManager();

    void add(AnalysisTemplate *at);
    QList<AnalysisTemplate*> methodes() const;
    AnalysisTemplate *method(int i) const;
    AnalysisTemplate *method(QString &name) const;
    void remove(int i);
    int count() const;

    void save();
    void load();

signals:

public slots:

private:
    QList<AnalysisTemplate*> m_itemList;
};

#endif // ANALYSISTEMPLATEMANAGER_H
