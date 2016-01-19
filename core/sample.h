/**
* @file sample.h
* @brief 采样数据处理类定义
* @author walterqin(walterqin@hotmail.com
* @date 2015-09-07
* @ingroup core
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include <QObject>
#include <QMap>
#include "core_global.h"
#include "global.h"
#include "instrument.h"

/** 通讯错误显示信息 */
typedef struct
{
    int errCmdId;                  /**< 通讯错误时的命令ID */
    int errorRet;                  /**< 通讯返回错误 */
    int status;                    /**< 主控当前状态 */
} CommErrInfo;


////////////////////////////////////////////////////////
/// \brief The Sample class
///
class CORE_EXPORT Sample : QObject
{
    Q_OBJECT

public:
    explicit Sample(Instrument *instr, QObject *parent = 0);
    ~Sample();

    enum {  QueryTime = 100 };
    void enableQuery();

    int getCurStatus();
    int getErrID();

signals:
    void updateRefPoint(QMap<int, short> &ref);
    void updateSampleData(QMap<int, short> &ref);
    void trainOn();
    void trainOff();
    void updateMeasureStatus();

private slots:
    void setCurrentSampleData(QVector<short> &data);
    void doSetStatus(quint8 status, quint8 err, quint8 index, quint8 recFlag);

private:
    int calcRefPoint(QList<int> data, int datalen);
    int renewRefPoint(QList<int> data, int datalen);
    bool isTrainOn(QList<int> data, int datalen, int threshold);
    bool isTrainOff();

private:
	typedef QList<short> predata;
    Instrument *m_instr;
    QMap<int, predata> *m_readyData;
    struct ChannelData *m_sample;

    int m_lastStatus;                  /**< 上一次状态 */
    int m_curStatus;                    /**< 系统当前状态 */
    int m_errID;
    int m_recovery;                     /**< 复位主控标志 */
};

#endif // SAMPLE_H
