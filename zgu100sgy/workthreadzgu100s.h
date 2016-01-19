/**
* @file workthreadzgu100s.h
* @brief 断轨双台面轨道衡工作线程类定义
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#ifndef WORKTHREADZGU100S_H
#define WORKTHREADZGU100S_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QVector>

#include "zgu100sgy.h"
#include "workbench.h"

class WorkthreadZGU100S : public QThread
{
    Q_OBJECT

public:
    struct SampleData
    {
        quint32 id;
        QVector<qint16> samp;
    };

    WorkthreadZGU100S(Zgu100sgy *instr, QObject *parent = 0);
    ~WorkthreadZGU100S();

    Workbench *workbench() const;

    void start();
    void pause();
    void stop();


    using QThread::sleep;
    using QThread::msleep;

signals:
    void updateSampleData(QVector<qint16> &samp);

protected:
    virtual void run();
    void fetchSample();
    void checkResult();

private:
    Zgu100sgy *m_instrument;
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_resume;
    bool m_abort;
};

#endif // WORKTHREADZGU100S_H
