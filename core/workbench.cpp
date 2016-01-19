/**
* @file workbench.cpp
* @brief 轨道衡仪器操作平台类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QDebug>
#include <QToolBar>
#include <QSettings>
#include <QApplication>
#include <QHBoxLayout>
#include <QFile>
#include <QScriptEngine>
#include <QInputDialog>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QBuffer>
#include <QTextStream>
#include <QStatusBar>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QThread>

#include "global.h"
#include "workbench.h"
#include "calibrationdoc.h"
#include "calibrationmanager.h"
#include "analysistemplatemanager.h"
#include "stdcurvemanager.h"
#include "instrument.h"
#include "instrumentfactory.h"
#include "weightdoc.h"
#include "analysisdoc.h"
#include "qcdoc.h"
#include "qcframe.h"
#include "mainwindowbase.h"
#include "mainwindow.h"
#include "machineconfig.h"
#include "wbaction.h"
#include "wbapp.h"
#include "calibrationframe.h"
#include "weightframe.h"
#include "staticweightwidget.h"
#include "dataresultframe.h"
#include "databasemaintainframe.h"
#include "analysisframe.h"
#include "logdataframe.h"
//#include "systemconfigframe.h"
#include "optionframe.h"
#include "messagewidget.h"

typedef QList<QAction *> ActionList;


static inline void addActionsToMenu(QMenu *m, const ActionList &al)
{
    const ActionList::const_iterator cend = al.constEnd();
    for (ActionList::const_iterator it = al.constBegin(); it != cend; ++it)
        m->addAction(*it);
}

static inline QMenu *addMenu(QMenuBar *mb, const QString &title, const ActionList &al)
{
    QMenu *rc = mb->addMenu(title);
    addActionsToMenu(rc, al);
    return rc;
}

static void initGlobal()
{

}

Workbench::Workbench(const QVariantMap &instrMap, QObject *parent)
    : QObject(parent)
    , d(new Data)
{
    initGlobal();

    connect(qApp, SIGNAL(userLogin(bool)), this, SIGNAL(userLogin(bool)));
    connect(qApp, SIGNAL(systemLogin()),   this, SIGNAL(systemLogin()));
    connect(qApp, SIGNAL(systemLogout()),  this, SIGNAL(systemLogout()));

    setTmpValue("instr", instrMap.value("name").toString());

    d->instrInfo = instrMap;
    d->instrument = 0;
    d->mainWindow = 0;
    d->debugger = 0;
    d->actionManager = new WBAction(this);
    d->caliManager = qApp->calibrationManager();
    d->curveManager = new StdCurveManager(this);
    d->weightDoc = new WeightDoc(this);
    d->caliDoc = new CalibrationDoc(this);
    d->analysisDoc = new AnalysisDoc(this);
    d->qcDoc = new QcDoc(this);
    d->printer = qApp->printer();

    initInstrument();
    if (!d->instrument)
        return;

    initMainWindow();
    d->instrument->initMainWindow(d->mainWindow);

    d->instrument->connectToHost();
    d->actionManager->showWeightAction->setChecked(true);
    connect(qApp, SIGNAL(initialized()), this, SLOT(showWeight()));

    // 显示视图
    connect(d->actionManager->showWeightAction, SIGNAL(triggered(bool)),
            this, SLOT(showWeight()));
    connect(d->actionManager->showStaticWeightAction, SIGNAL(triggered(bool)),
            this, SLOT(showStaticWeight()));
    connect(d->actionManager->showDataAction, SIGNAL(triggered(bool)),
            this, SLOT(showDataResult()));
    connect(d->actionManager->showCalibrateAction, SIGNAL(triggered(bool)),
            this, SLOT(showCalibration()));
    connect(d->actionManager->showQcAction, SIGNAL(triggered(bool)),
            this, SLOT(showQc()));
    connect(d->actionManager->showDatabaseAction, SIGNAL(triggered(bool)),
            this, SLOT(showDatabase()));
    connect(d->actionManager->showAnalysisAction, SIGNAL(triggered(bool)),
            this, SLOT(showAnalysis()));
    connect(d->actionManager->showLogAction, SIGNAL(triggered(bool)),
            this, SLOT(showLog()));
    connect(d->actionManager->showSystemAction, SIGNAL(triggered(bool)),
            this, SLOT(showSystem()));

    // 关于
    connect(d->actionManager->aboutAction, SIGNAL(triggered()), qApp, SLOT(about()));

    // 退出
    connect(d->actionManager->exitAction, SIGNAL(triggered()),
            d->mainWindow, SLOT(close()));

}

Workbench::~Workbench()
{
    qDelete(d->instrument);
    if (d->mainWindow)
        delete d->mainWindow;
    if (d->debugger)
        delete d->debugger;
    delete d;
}

void Workbench::initInstrument()
{
    if (d->instrInfo.isEmpty())
        return;

    d->instrument = InstrumentFactory::create(d->instrInfo.value("dll").toString(), this);
    if (!d->instrument)
        return;

    QString instrName = instrInfo().value("name").toString();

    d->debugger = d->instrument->initDebugger();

}

void Workbench::initMainWindow()
{
    d->mainWindow = new MainWindow(this);
    d->mainWindow->setWindowTitle(instrInfo().value("title", qApp->applicationName()).toString());
    d->mainWindow->setAttribute(Qt::WA_DeleteOnClose);

	WBAction *actionManager = d->actionManager;
//	QMenuBar *menuBar = d->mainWindow->menuBar();
//	addMenu(menuBar, tr("称重"), actionManager->weightActions->actions());

    QToolBar *toolBar = d->mainWindow->toolBar();
    toolBar->setWindowTitle("标准");
    toolBar->toggleViewAction()->setEnabled(false);

    QWidget* w = new QWidget;
    w->setMinimumHeight(40);
    toolBar->addWidget(w);

    toolBar->addActions(actionManager->viewActions->actions());
    toolBar->addAction(actionManager->aboutAction);

    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    toolBar->addWidget(widget);

    toolBar->addAction(actionManager->exitAction);

    d->messageWidget = d->mainWindow->messageWidget();
}

template<typename T>
T *Workbench::showWidget()
{
    T *w = qobject_cast<T*>(mainWindow()->centralWidget());
    if (!w)
    {
        if (mainWindow()->centralWidget())
            delete mainWindow()->centralWidget();

        w = new T(this, mainWindow());
        mainWindow()->setCentralWidget(w);
    }
    return w;
}

void Workbench::showWeight()
{
    showWidget<WeightFrame>();
}

void Workbench::showStaticWeight()
{
    showWidget<StaticWeightWidget>();
}

void Workbench::showDataResult()
{
    showWidget<DataResultFrame>();
}

void Workbench::showCalibration()
{
    showWidget<CalibrationFrame>();
}

void Workbench::showQc()
{
    showWidget<QcFrame>();
}

void Workbench::showDatabase()
{
    showWidget<DatabaseMaintainFrame>();
}

void Workbench::showAnalysis()
{
    showWidget<AnalysisFrame>();
}

void Workbench::showLog()
{
    showWidget<LogDataFrame>();
}

void Workbench::showSystem()
{
   // showWidget<SystemConfigFrame>();
    QDialog dlg(mainWindow());
    dlg.setWindowTitle(tr("系统设置"));
    dlg.setWindowModality(Qt::WindowModal);

    OptionFrame *frame = new OptionFrame(this, &dlg);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(frame);
    dlg.setLayout(layout);
    dlg.exec();
}

QVariant Workbench::instrInfo(const QString &key, const QVariant &defaultValue) const
{
    return d->instrInfo.value(key, defaultValue);
}

void Workbench::setInstrInfo(const QString &key, const QVariant &value)
{
    d->instrInfo[key] = value;
}

QList<QVariantMap> load_instr_info()
{
    QFile file(MachineConfig::getConfigurePath() + "/instr.js");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString script = in.readAll();

    QScriptEngine engine;
    QVariant var = engine.evaluate(script, file.fileName()).toVariant();
    if (engine.hasUncaughtException())
    {
        critical(engine.uncaughtException().toString() + "\n" +
                 engine.uncaughtExceptionBacktrace().join("\n"));
        return QList<QVariantMap>();
    }

    QList<QVariant> list = var.toList();
    QList<QVariantMap> map;
    foreach(QVariant v, list)
    {
        QVariantMap vm = v.toMap();
        if (!vm.isEmpty())
             map.append(vm);
#ifndef QT_NO_DEBUG
        else
            qDebug() << "Can't change variant to QVariantMap: " << v;
#endif
    }

	if (map.isEmpty())
		critical(Workbench::tr("无效的配置文件, 不包含任何仪器信息."));

    return map;
}

QVariantMap find_instr_info(const QList<QVariantMap> &map, const QString &name)
{
    foreach(QVariantMap m, map)
        if (m.value("name").toString() == name)
            return m;
    return QVariantMap();
}

void Workbench::showDebugger()
{
//    if (d->debugger)
//    {
//        d->debugger->show();
//        d->debugger->raise();
//    }
}

bool Workbench::canClose()
{
    bool prompted = false;
    if (d->caliManager && !d->caliManager->canClose(prompted))
        return false;

    if (d->curveManager && !d->curveManager->canClose(prompted))
        return false;

    if (d->caliDoc && !d->caliDoc->canClose(prompted))
        return false;

    if (d->weightDoc && !d->weightDoc->canClose(prompted))
        return false;

    if (d->instrument && !d->instrument->canClose(prompted))
        return false;

    if (!prompted && question(tr("确认退出?")) == QMessageBox::No)
        return false;

    if (d->debugger)
        d->debugger->close();

    return true;
}

bool Workbench::isSystemLogin() const
{
    return qApp->isSystemLogin();
}

QString Workbench::about() const
{
    return d->instrument->about();
}

QVariant Workbench::value(const QString &key, const QVariant &defaultValue) const
{
    return qApp->value(key, defaultValue);
}

void Workbench::setValue(const QString &key, const QVariant &value)
{
    qApp->setValue(key, value);
}

QVariant Workbench::tmpValue(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(&m_readWriteLock);
    return m_tmp.value(key, defaultValue);
}

void Workbench::setTmpValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(&m_readWriteLock);
    m_tmp[key] = value;
}

QWidget *Workbench::safeParentWindow() const
{
    QWidget *parent = qApp->activeModalWidget();
    if (!parent)
        parent = d->mainWindow;
    if (parent && parent->window())
        parent = parent->window();

    return parent;
}

void Workbench::showMessageBox(QMessageBox::StandardButton *response,
                               QMessageBox::Icon icon,
                               const QString &text,
                               QMessageBox::StandardButtons buttons,
                               QMessageBox::StandardButton defaultButton)
{
    WaitCursor arrow(Qt::ArrowCursor);
    QWidget *parent = safeParentWindow();
    QMessageBox box(icon,
                    d->mainWindow ? d->mainWindow->windowTitle()
                                  : qApp->applicationName(),
                    text,
                    buttons,
                    parent);

    if (defaultButton != QMessageBox::NoButton)
        box.setDefaultButton(defaultButton);

    *response = (QMessageBox::StandardButton)box.exec();
}

QMessageBox::StandardButton
Workbench::information(const QString &text,
                       QMessageBox::StandardButtons buttons,
                       QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this,  "showMessageBox", type,
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Information),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::question(const QString &text,
                    QMessageBox::StandardButtons buttons,
                    QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Question),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::warning(const QString &text,
                   QMessageBox::StandardButtons buttons,
                   QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Warning),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

QMessageBox::StandardButton
Workbench::critical(const QString &text,
                    QMessageBox::StandardButtons buttons,
                    QMessageBox::StandardButton defaultButton)
{
    QMessageBox::StandardButton button = QMessageBox::NoButton;
    Qt::ConnectionType type = QThread::currentThread() == thread()
            ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(this, "showMessageBox", type,
                              //Q_RETURN_ARG(QMessageBox::StandardButton, button),
                              Q_ARG(QMessageBox::StandardButton *, &button),
                              Q_ARG(QMessageBox::Icon, QMessageBox::Critical),
                              Q_ARG(QString, text),
                              Q_ARG(QMessageBox::StandardButtons, buttons),
                              Q_ARG(QMessageBox::StandardButton, defaultButton));
    return button;
}

void Workbench::showInformation(const QString &text, bool show)
{
    showMessage(QtDebugMsg, text, show);
}

void Workbench::showWarning(const QString &text, bool show)
{
    showMessage(QtWarningMsg, text, show);
}

void Workbench::showCritical(const QString &text, bool show)
{
    showMessage(QtCriticalMsg, text, show);
}

void Workbench::showFatal(const QString &text, bool show)
{
    showMessage(QtFatalMsg, text, show);
}

void Workbench::showMessage(int type, const QString &text, bool show)
{
    showMessage(0, type, text, show);
}

void Workbench::showMessage(int channel, int type, const QString &text, bool show)
{
    QMetaObject::invokeMethod(
                d->messageWidget,
                "showMessage",
                Q_ARG(Workbench*, this),
                Q_ARG(int, channel),
                Q_ARG(int, type),
                Q_ARG(QString, text),
                Q_ARG(bool, show));
}

void Workbench::showStatusMessage(const QString &text, int timeout)
{
    QMetaObject::invokeMethod(
                d->mainWindow->statusBar(),
                "showMessage",
                Q_ARG(QString, text),
                Q_ARG(int, timeout));
}
