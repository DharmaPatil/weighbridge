/**
* @file abstractanalysisdoc.h
* @brief 轨道衡测量分析结果数据组织基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef ABSTRACTANALYSISDOC_H
#define ABSTRACTANALYSISDOC_H

#include <QObject>
#include <QSet>
#include <QMap>

#include "core_global.h"
#include "global.h"

class Workbench;
class QMessageBox;


///////////////////////////////////////////////////////////
/// \brief The AnalysisResultItem class
///
class CORE_EXPORT AnalysisResultItem
{
public:
    explicit AnalysisResultItem(QString &num = QString(),
                                QString &trainType = QString(),
                                QString &trainCode = QString(),
                                QString &speed = QString(),
                                QString &weight = QString(),
                                QString &diff = QString(),
                                QString &offset = QString(),
                                QString &leftw = QString(),
                                QString &rightw = QString())
        : m_num(num)
        , m_trainType(trainType)
        , m_trainCode(trainCode)
        , m_speed(speed)
        , m_weight(weight)
        , m_diff(diff)
        , m_offset(offset)
        , m_leftw(leftw)
        , m_rightw(rightw) {}

public:
    QString m_num;          //序号
    QString m_trainType;    //车型
    QString m_trainCode;    //车号
    QString m_speed;        //速度
    QString m_weight;       //重量
    QString m_diff;         //前后差
    QString m_offset;       //左右偏
    QString m_leftw;        //左轮重
    QString m_rightw;       //右轮重

};


////////////////////////////////////////////////////////////
/// \brief The AbstractAnalysisDoc class
///
class CORE_EXPORT AbstractAnalysisDoc : public QObject
{
    Q_OBJECT

public:
    explicit AbstractAnalysisDoc(Workbench *parent = 0);
    virtual ~AbstractAnalysisDoc();

    Workbench *workbench() const { return m_workbench; }

    bool isRunning() const { return false; }
    virtual void load(QString &filename = QString()) { m_filename = filename; }
    virtual void save(QString &filename = QString()) {}
    QString filename() const { return m_filename; }

signals:
    void insertResult(QStringList &);
    void insertSampleData(QList<int> &raw, QList<int> &flt);
    void insertSynthsisData(QList<int> &data);

public slots:
    virtual void onAddSampleData(QList<int> &);
    virtual void start(bool showLayout = true);
    virtual void pause();

protected slots:
    virtual void onTaskStatusChanged(weighbridge::AnalysisStatus s);
    virtual void onTaskFinished();
    void onSaveSampleData(QString &filename = QDate::currentDate().toString());

private:
    int filter(QList<int> &data, int index);
    int diff(QList<int> data, int index);

private:
    QMessageBox *m_msgBox;
    Workbench *m_workbench;
    QString m_filename;
};

#endif // ABSTRACTANALYSISDOC_H
