/**
* @file instrzgu100sgy.h
* @brief 断轨双台面轨道衡仪器类定义
* @ingroup zgu100sgy
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-29
*/
#ifndef ZGU100SGY_H
#define ZGU100SGY_H

#include "zgu100sgy_global.h"
#include "instrument.h"
#include "instrparamheader.h"

class WorkthreadZGU100S;
class Serial;
class Blink;
class MessageBuilder;

#define PLANTFORMNUM 3
#define CHANNELNUM_ON_PLANTFORM 4
#define TOTALCHANNEL 12

class ZGU100SGY_EXPORT Zgu100sgy : public Instrument
{
    Q_OBJECT

public:
    Zgu100sgy(Workbench *parent = 0);
    ~Zgu100sgy();

    QVariant instrumentInfo(Info info) const;
    QString about() const;

    void connectToHost();
    void disconnectFromHost();
    void waitForConnected();

    // 仪器参数, 每个仪器有一个从InstrParamHeader派生出来的参数实例
    const InstrParamHeader *param() const { return &m_instrParam; }
    void setParam(const InstrParamHeader *pa);

    QIODevice *openParamFile(QIODevice::OpenMode mode);

    void addOptionPage(OptionFrame *frame);
    void initMainWindow(QMainWindow *window);
    void manualTest();

    bool start();
    void pause();
    void demo();

    void setReady(bool ready);
    bool isReady() const { return m_isReady; }
    bool isSimulate() const;

    bool openPort(QString *portUsed);
    bool reOpenPort();
    void closePort();

    Serial *serial() const { return m_serial; }
    QMutex *serialMutex() const { return &m_serialMutex; }
    int talk(char *answer, int maxlen, const char *cmd, int len);
    int talk(MessageBuilder &mb);
    QByteArray loadPositionSettings(const QString &fileName);

    bool reset();
    void clean();

protected:
    Blink *blink() const { return m_blink; }

private:
    friend class WorkthreadZGU100S;

private:
    InstrParamHeader m_instrParam;
    WorkthreadZGU100S *m_workerThread;

#ifndef NO_SIM
    bool m_sim;
#endif

    bool m_isReady;
    Serial *m_serial;
    mutable QMutex m_serialMutex;
    Blink *m_blink;
};

#endif // ZGU100SGY_H
