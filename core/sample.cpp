/**
* @file sample.cpp
* @brief 采样数据的处理实现
* @author walterqin(walterqin@hotmail.com)
* @date 2015-09-07
* @ingroup core
*/

#include <QtCore>
#include "sample.h"

Sample::Sample(Instrument *instr, QObject *parent)
{

}

Sample::~Sample()
{

}

/** @brief 计算参考点*/
int Sample::calcRefPoint(QList<int> data, int datalen)
{
    const int NOISE_THRESHOLD = 1280;
    qint64 sum;
    qint32 maxRef, minRef, rawData;
    qint32 noise;

    sum = 0;
    maxRef = -32767;
    minRef = 32767;
    noise = 0;

    for (int i = 0; i < datalen; i++)
    {
        rawData = data.at(i);
        sum += rawData;
        if (maxRef < rawData)
            maxRef = rawData;
        if (minRef > rawData)
            minRef = rawData;
    }

    noise = qAbs(maxRef - minRef);
    if (noise >= NOISE_THRESHOLD)
        return -1;

    return (sum / datalen);
}

/** @brief 重新计算参考点 */
int Sample::renewRefPoint(QList<int> data, int datalen)
{
    int oldRef, newRef;

    newRef = 0;
    if ( (oldRef = calcRefPoint(data, datalen)) > 0)
    {
        newRef = (newRef * 500 + oldRef) / 501;
        if (qAbs(oldRef - newRef) > 3)
            return newRef;
        else
            return oldRef;
    }
    return oldRef;
}

/** @brief 判读是否上称 */
bool Sample::isTrainOn(QList<int> data, int datalen, int threshold)
{
    int counter = 0;

    for (int i = 0; i < datalen; i++)
    {
        if (data.at(i) > threshold)
            counter++;
        else
            counter = 0;

        if (counter > 10)
            return true;
    }
    return false;
}

void Sample::setCurrentSampleData(QVector<short> &data)
{
    Q_UNUSED(data)
}

void Sample::doSetStatus(quint8 status, quint8 err, quint8 index, quint8 recFlag)
{
    Q_UNUSED(index)

    m_lastStatus = m_curStatus;
    m_curStatus = status;
    m_errID = err;
    m_recovery = recFlag;

}

