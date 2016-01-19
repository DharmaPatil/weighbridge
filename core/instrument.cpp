/**
* @file workbench.cpp
* @brief 轨道衡仪器类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QBuffer>

#include "global.h"
#include "instrparamheader.h"
#include "instrument.h"
#include "workbench.h"
#include "standardcurve.h"
#include "stdcurvemanager.h"
#include "profile.h"

QString Instrument::Message::messageText() const
{
    return QString("0x%1: %2").arg(code, 8, 16, QChar('0')).arg(text);
}

void Instrument::Message::report() const
{
    information(messageText());
}

Instrument::Instrument(Workbench *parent)
    : QObject(parent)
    , QMutex(Recursive)
    , m_state(StatusWaiting)
    , m_isRunning(false)
    , m_enableDebugCurve(true)
{

}

Instrument::~Instrument()
{

}

void Instrument::setControlType(weighbridge::ControlMode mode)
{
    m_controlMode = mode;
}

QVariant Instrument::instrumentInfo(Instrument::Info info) const
{
    Q_UNUSED(info)
    return QVariant();
}

QString Instrument::about() const
{
    return QString();
}

bool Instrument::isConnected() const
{
    return false;
}

void Instrument::connectToHost()
{

}

void Instrument::disconnectFromHost()
{

}

void Instrument::waitForConnected()
{

}

bool Instrument::isValidParam() const
{
    return false;
}

bool Instrument::loadParam(QIODevice *device)
{
    device = device;
    return true;
}

bool Instrument::saveParam(QIODevice *device) const
{
    device = device;
    return true;
}

bool Instrument::loadParam(const QString &filename)
{
    Q_UNUSED(filename)
    return true;
}

bool Instrument::saveParam(const QString &filename) const
{
    Q_UNUSED(filename)
    return true;
}

bool Instrument::loadParam()
{
    return true;
}

bool Instrument::saveParam() const
{
    return true;
}

void Instrument::debugCurve(int channel, const QString &filename)
{
    channel = channel;
    Q_UNUSED(filename)
}

void Instrument::debugCurve(int channel, const QVector<ushort> &data, const QString &assay)
{
    Q_UNUSED(channel)
    Q_UNUSED(data)
    Q_UNUSED(assay)
}

void Instrument::debugCurve(const QVector< QVector<ushort> > &curves, QStringList fields)
{
    Q_UNUSED(curves)
    Q_UNUSED(fields)
}

void Instrument::debugCurve(const QString &filename)
{
    Q_UNUSED(filename)
}

int Instrument::setStatusCode(int code, int channel, const QString &detail)
{
    code = code;channel = channel;
    Q_UNUSED(detail)
    return 0;
}

void Instrument::setStatusText(const QString &text, int type, int channel)
{
    Q_UNUSED(text)
    type = type; channel = channel;
}

Instrument::Message Instrument::findMessage(int code) const
{
    return m_codeTable.value(code);
}

void Instrument::reportMessage(int code) const
{
    code = code;
}

void Instrument::setUuid(const QUuid &uuid)
{
    Q_UNUSED(uuid)
}

void Instrument::end()
{

}

void Instrument::demo()
{

}

bool Instrument::reset()
{
    return true;
}

void Instrument::setRunning(bool running)
{
    running = running;
}

void Instrument::setStopped()
{

}

void Instrument::setState(InstrumentState state)
{
    Q_UNUSED(state)
}

void Instrument::addMessage(int code, int type, const QString &text)
{
    code = code; type = type;
    Q_UNUSED(text)
}

QByteArray Instrument::readChunk(QIODevice *device)
{
    device = device;
    return QByteArray();
}

bool Instrument::writeChunk(const QByteArray &chunk, QIODevice *device)
{
    Q_UNUSED(chunk)
    Q_UNUSED(device)
    return true;
}

void Instrument::loadMessage(const QString &fileName, int type)
{
    Q_UNUSED(fileName)
    type = type;
}

int Instrument::selectCurve(int type) const
{
    return type;
}

bool Instrument::loadDebugCurve(const QString &filename, QVector<QVector<ushort> > &curves, QStringList &fields) const
{
    Q_UNUSED(filename)
    Q_UNUSED(curves)
    Q_UNUSED(fields)
    return true;
}
