/**
* @file workbench.h
* @brief 轨道衡仪器操作平台类定义，包括采集、测试、标定及质控等
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QObject>
#include <QPointer>
#include <QVariant>
#include <QMap>
#include <QMessageBox>
#include <QReadWriteLock>

#include "core_global.h"
#include "mainwindow.h"
#include "instrument.h"

//class Instrument;
//class MainWindow;
class WBAction;
class StdCurveManager;
class CalibrationManager;
class AnalysisTemplateManager;
class WeightDoc;
class CalibrationDoc;
class AnalysisDoc;
class QcDoc;
class DataStorage;
class MessageWidget;
class QSettings;
class QAction;
class QMenu;
class QDeclarativeView;

class CORE_EXPORT Workbench : public QObject
{
    Q_OBJECT
public:
    explicit Workbench(const QVariantMap &instrMap, QObject *parent = 0);
    ~Workbench();

    WBAction *actionManager() const { return d->actionManager; }
    MainWindow *mainWindow() const { return d->mainWindow; }
    CalibrationManager *calibrationManager() const { return d->caliManager; }
    StdCurveManager *curveManager() const { return d->curveManager; }
    WeightDoc *weightDoc() const { return d->weightDoc; }
    CalibrationDoc *calibrationDoc() const { return d->caliDoc; }
    AnalysisDoc *analysisDoc() const { return d->analysisDoc; }
    QcDoc *qcDoc() const { return d->qcDoc; }
    Instrument *instrument() const { return d->instrument; }
    DataStorage *printer() const { return d->printer; }
    QVariantMap instrInfo() const { return d->instrInfo; }
    QVariant instrInfo(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setInstrInfo(const QString &key, const QVariant &value);

    QVariant value(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    template<class T> T value(const QString &key) const { return value(key).value<T>(); }

    QVariant tmpValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void setTmpValue(const QString &key, const QVariant &value);
    template<class T> T tmpValue(const QString &key) const { return tmpValue(key).value<T>(); }

    bool canClose();
    bool isSystemLogin() const;
    QString about() const;

    QWidget *safeParentWindow() const;

    QMessageBox::StandardButton
    information(const QString &text,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    question(const QString &text,
             QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
             QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    warning(const QString &text,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok,
            QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

    QMessageBox::StandardButton
    critical(const QString &text,
             QMessageBox::StandardButtons buttons = QMessageBox::Ok,
             QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

signals:
    void userLogin(bool system);
    void systemLogin();
    void systemLogout();

public slots:
    void showWeight();
    void showStaticWeight();
    void showDataResult();
    void showCalibration();
    void showQc();
    void showDatabase();
    void showAnalysis();
    void showLog();
    void showSystem();
    void showDebugger();
    void showInformation(const QString &text, bool show = true);
    void showWarning(const QString &text, bool show = true);
    void showCritical(const QString &text, bool show = true);
    void showFatal(const QString &text, bool show = true);
    void showMessage(int channel, int type, const QString &text, bool show = true);
    void showMessage(int type, const QString &text, bool show = true);

    void showMessageBox(QMessageBox::StandardButton *response,
                        QMessageBox::Icon icon,
                        const QString &text,
                        QMessageBox::StandardButtons buttons,
                        QMessageBox::StandardButton defaultButton);
    void showStatusMessage(const QString &text, int timeout = 2000);


private:
    void initInstrument();
    void initMainWindow();

    template<typename T>  T *showWidget();

    struct Data{
        WBAction				*actionManager;
        CalibrationManager		*caliManager;
        AnalysisTemplateManager *atManager;
        StdCurveManager			*curveManager;
        WeightDoc               *weightDoc;
        CalibrationDoc			*caliDoc;
        AnalysisDoc				*analysisDoc;
        QcDoc					*qcDoc;
        DataStorage				*printer;
        Instrument				*instrument;
        QPointer<QWidget>		debugger;
        QPointer<MainWindow>    mainWindow;
        QVariantMap				instrInfo;
        QPointer<MessageWidget> messageWidget;
    };

    Data *d;

    QMap<QString, QVariant> m_tmp;
    mutable QReadWriteLock m_readWriteLock;
};

QVariant load_script(const QString &fileName);
QList<QVariantMap> load_instr_info();
QVariantMap find_instr_info(const QList<QVariantMap> &map, const QString &name);

Q_DECLARE_METATYPE(QMessageBox::StandardButton)
Q_DECLARE_METATYPE(QMessageBox::StandardButton*)
Q_DECLARE_METATYPE(QMessageBox::StandardButtons)
Q_DECLARE_METATYPE(QMessageBox::Icon)
Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(Workbench*)

#endif // WORKBENCH_H
