/**
* @file zgu100sgy.h
* @brief 断轨双台面轨道衡仪器类实现
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#include <QVarLengthArray>
#include <QtEndian>
#include <QLabel>
#include <QBoxLayout>
#include <QStatusBar>
#include <QMetaObject>

#include "zgu100sgy.h"
#include "workbench.h"
#include "profile.h"
#include "wbapp.h"
#include "workthreadzgu100s.h"
#include "messagebuilder.h"
#include "optionframe.h"
#include "optionwidget.h"
#include "connectionpage.h"
#include "blink.h"

Zgu100sgy::Zgu100sgy(Workbench *parent)
    : Instrument(parent)
{

}

Zgu100sgy::~Zgu100sgy()
{

}

QVariant Zgu100sgy::instrumentInfo(Info info) const
{
    switch(info)
    {
    case InfoSampChannels:
        return TOTALCHANNEL;
    case InfoPlantform:
        return PLANTFORMNUM;
    case InfoChannelPerPlantform:
        return CHANNELNUM_ON_PLANTFORM;
    default:
        return Instrument::instrumentInfo(info);
    }
}

QString Zgu100sgy::about() const
{
    return Instrument::about();
}

void Zgu100sgy::connectToHost()
{

}

void Zgu100sgy::disconnectFromHost()
{

}

void Zgu100sgy::waitForConnected()
{

}

void Zgu100sgy::setParam(const InstrParamHeader *pa)
{
    Q_UNUSED(pa)
}

QIODevice *Zgu100sgy::openParamFile(QIODevice::OpenMode mode)
{
    Q_UNUSED(mode)
    return 0;
}

void Zgu100sgy::addOptionPage(OptionFrame *frame)
{
    QTreeWidgetItem *parent = frame->addItem(OptionFrame::tr("仪器维护"));
    frame->addItem(new ConnectionPage(this), parent);
    if (workbench()->isSystemLogin())
    {
//        frame->addItem(new ParamPage(this), parent);
//        frame->addItem(new DebugPage(this), parent);
    }
}

void Zgu100sgy::initMainWindow(QMainWindow *window)
{
    Q_UNUSED(window)
}

void Zgu100sgy::manualTest()
{

}

bool Zgu100sgy::start()
{
    return true;
}

void Zgu100sgy::pause()
{

}

void Zgu100sgy::demo()
{
    information(tr("该仪器没有演示程序."));
}

bool Zgu100sgy::openPort(QString *portUsed)
{
    Q_UNUSED(portUsed)
    return true;
}

bool Zgu100sgy::reOpenPort()
{
    return true;
}

void Zgu100sgy::closePort()
{

}

int Zgu100sgy::talk(char *answer, int maxlen, const char *cmd, int len)
{
    Q_UNUSED(answer)
    Q_UNUSED(maxlen)
    Q_UNUSED(cmd)
    Q_UNUSED(len)
    return 0;
}

int Zgu100sgy::talk(MessageBuilder &mb)
{
    int ret = talk(mb.buffer.data(), mb.buffer.capacity(),
                   mb.buffer.constData(), mb.buffer.length());
    mb.buffer.resize(qMax(0, ret));
    mb.seek(0);
    return ret;
}

bool Zgu100sgy::reset()
{
    return true;
}

void Zgu100sgy::clean()
{

}

void Zgu100sgy::setReady(bool ready)
{
    if (m_isReady == ready)
        return;

    m_isReady = ready;
    QMetaObject::invokeMethod(m_blink, ready ? "stop" : "start");
}

bool Zgu100sgy::isSimulate() const
{
#ifndef NO_SIM
    return m_sim;
#else
    return false;
#endif
}
