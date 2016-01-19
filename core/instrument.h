/**
* @file workbench.h
* @brief 轨道衡仪器类定义，包括采集、数据分析、车型判读等
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QObject>
#include <QMutex>
#include <QStringList>
#include <QList>
#include <QAtomicInt>
#include <QUuid>
#include <QReadWriteLock>

#include "core_global.h"
#include "global.h"
#include "optionframe.h"

//class Worklist;
class Workbench;
class InstrParamHeader;
class QMainWindow;
class Sample;

class CORE_EXPORT Instrument : public QObject, public QMutex
{
    Q_OBJECT
public:
    struct Message
    {
        int code;
        int type;
        QString text;

        Message() : code(0), type(QtDebugMsg) {}
        QString messageText() const;
        void report() const;
    };

    enum MessageCode
    {
        CodeOk            = 0x00000000, //没有错误
        CodeBaseException = 0x800c8001, //零点异常
        CodeBuffException = 0x800c8002, //缓存异常
    };

    enum MessageChannel
    {
        ChannelSerial = 0x10000000, //串口通信报警
        ChannelNetport,              //网络通信报警
        ChannelSensor,               //传感器报警
        ChannelBuffer,               //缓存报警
    };

    enum PauseReason
    {
        ReasonCrash,   //紧急停止
        ReasonRestart, //重新称重
        ReasonOnWeigh, //长时间停称
        ReasonOther,   //其他原因
    };

    enum InstrumentState
    {
        StatusWaiting,
        StatusReady,
        StatusIdle,
        StatusBogieDown,
        StatusBogieOneAxisOn,
        StatusBogieTwoAxisOn,
        StatusBogieThreeAxisOn,
        StatusBogieThreeAxisDn,
        StatusBogieTwoAxisDn,
        StatusBogieOneAxisDn,
        StatusTrainOn,
        StatusTrainOff,
        StatusError,
        StatusPowerOff,
        StatusUpdateDevice,
    };

    enum Info
    {
        InfoSampChannels ,		// 采样通道数
        InfoPlantform,          // 台面数
        InfoChannelPerPlantform,// 每台面通道数
    };

    enum Feature
    {
        ManualTest,
        SimulateWeight,
        TestSensor,
    };

    enum Flag
    {
        FlagWeight,
        FlagSimulate,
        FlagTestSensor,
    };

    explicit Instrument(Workbench *parent = 0);
    ~Instrument();

    void setControlType(weighbridge::ControlMode mode);
    weighbridge::ControlMode getControlType() { return m_controlMode; }

//   void setWorklist(Worklist *wl);
//   Worklist *worklist() const { return m_worklist; }

    Workbench *workbench() const { return m_workbench; }
    weighbridge::WeighBridgeType weightbridgeType() const { return m_instrType; }
    virtual QVariant instrumentInfo(Instrument::Info info) const;
    virtual QString about() const;

    InstrumentState state() const { return m_state; }
    bool isConnected() const;
    bool isRunning() const { return m_isRunning; }

    virtual void connectToHost();
    virtual void disconnectFromHost();
    virtual void waitForConnected();

    //仪器参数
    virtual const InstrParamHeader *param() const = 0;
    virtual void setParam(const InstrParamHeader *para) = 0;
    bool isValidParam() const;

    virtual QIODevice *openParamFile(QIODevice::OpenMode mode) = 0;

    bool loadParam(QIODevice *device);
    bool saveParam(QIODevice *device) const;

    bool loadParam(const QString &filename);
    bool saveParam(const QString &filename) const;

    bool loadParam();
    bool saveParam() const;

    virtual void addOptionPage(OptionFrame *frame) {}
    virtual void initMainWindow(QMainWindow *window) {}
    virtual void manualTest() {}
    virtual QWidget *initDebugger() { return 0; }
    virtual void debugCurve(int channel, const QVector<ushort> &data, const QString &assay);
    void debugCurve(int channel, const QString &filename);
    void debugCurve(const QVector< QVector<ushort> > &curves, QStringList fields);
    void debugCurve(const QString &filename);
    bool enableDebugCurve(bool e = true) { bool b = m_enableDebugCurve; m_enableDebugCurve = e; return b; }
    bool isDebugCurveEnabled() const { return m_enableDebugCurve; }
    virtual void statisticCurve() {}
    virtual bool canClose(bool &prompted) { prompted = prompted; return true; }

    int setStatusCode(int code, int channel = 0, const QString &detail = QString());
    void setStatusText(const QString &text, int type = QtWarningMsg, int channel = 0);
    Instrument::Message findMessage(int code) const;
    void reportMessage(int code) const;

    QUuid uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid);

signals:
    void connected();
    void disconnected();
    void stateChanged(InstrumentState state);
    void fatalError(int code, const QString &info);
    void uuidChanged();
    void runningStateChanged(bool running);
    void updateRefPoint(QVector<qint16> &ref);
    void updateSamplePoint(qint16 &samp);

public slots:
    virtual bool start() = 0;
    virtual void pause() = 0;
    virtual void end();
    virtual void demo();
    virtual bool reset();

protected slots:
    void setRunning(bool running = true);
    void setStopped();

protected:
    void setState(InstrumentState state);
    void addMessage(int code, int type, const QString &text);

    enum { MaxChunkSize = 4096 };
    static QByteArray readChunk(QIODevice *device);
    static bool writeChunk(const QByteArray &chunk, QIODevice *device);

private:
    void loadMessage(const QString &fileName, int type);
    int selectCurve(int type) const;
    bool loadDebugCurve(const QString &filename, QVector<QVector<ushort> > &curves, QStringList &fields) const;

private:
    Workbench *m_workbench;     //工作平台
    InstrumentState m_state;    //仪器状态
    Sample *m_sample;           //采样数据
    bool m_isRunning;
    bool m_enableDebugCurve;
    QMap<int, Message> m_code;
    QMap<int, Message> m_codeTable;
    const QUuid m_uuid;

    weighbridge::ControlMode m_controlMode;   //运行模式
    weighbridge::WeighBridgeType m_instrType;  //仪器类型
};

#endif // INSTRUMENT_H
