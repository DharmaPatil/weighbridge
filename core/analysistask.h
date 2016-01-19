/**
* @file workbench.h
* @brief 轨道衡测试任务类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef ANALYSISTASK_H
#define ANALYSISTASK_H

#include <QObject>
#include <QList>
#include <QVector>

#include "analysisresult.h"

class Workbench;

class AnalysisTask : public QObject
{
    Q_OBJECT

public:
    explicit AnalysisTask(Workbench *wb, QObject *parent = 0);
    AnalysisTask(Workbench *wb, const QString &sample,  int repeat = 1, QObject *parent = 0);
    ~AnalysisTask();

//    int repeat() const { return m_repeat; }
//    void setRepeat(int n) { m_repeat = n; }

//    ResultMap results() const { return m_results; }
//    void setItemResult(const QString &itemFullName, AnalysisResult result);

signals:

public slots:

private:
    Workbench *m_workbench;
    QString m_sample;
    int m_repeat;
};

#endif // ANALYSISTASK_H
