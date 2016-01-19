/**
* @file wbapp.h
* @brief 轨道衡应用程序类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <windows.h>
#include <conio.h>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QStringList>
#include <QLocale>
#include <QSysInfo>
#include <QThread>
#include <QtConcurrentRun>
#include <QSettings>
#include <QReadWriteLock>
#include <QTimer>

#include "wbapp.h"
#include "version.h"
#include "global.h"
#include "workbench.h"
#include "mainwindow.h"
#include "reportprinter.h"
#include "calibrationmanager.h"
#include "wbaction.h"
#include "logindialog.h"
#include "instrtypedialog.h"
#include "appwindow.h"

void usage()
{
    const char *str =
            "nx [options...]\n"
            "\n"
            "-h               显示本信息\n"
            "-instr <type>    仪器类型\n"
            "-port <ports>    连接端口, 如果需要多个端口, 可用逗号(,)隔开\n"
            "-sim             模拟运行, 忽略连接端口\n"
            ;
    information(str);
}

/** @brief 加载翻译器 */
static bool loadTranslator(QTranslator *translator, const QStringList &dir, const QString &filename)
{
    foreach(const QString &path, dir)
        if (translator->load(filename, path))
            return true;
    return false;
}

static void (*fnBeep)(int, int);

static void beepLoop(int freq, int duration, int sleep, int repeat)
{
    while(repeat-- > 0)
    {
        fnBeep(freq, duration);
        Sleep(sleep);
    }
}

static void beep_winnt(int freq, int duration)
{
    Beep(freq, duration);
}

//////////////////////////////////////////////////////////////////
/// \brief The PlaySoundThread class
///
class PlaySoundThread : public QThread
{
public:
    PlaySoundThread(QObject *parent = 0) : QThread(parent) {}

    void start(int freq, int duration, int interval)
    {
        m_run = true;
        m_freq = freq;
        m_duration = duration;
        m_interval = interval;

        if (!isRunning())
            QThread::start();
    }

    void stop()
    {
        m_run = false;
    }

protected:
    virtual void run()
    {
        while(m_run)
        {
            WBApp::beep(m_freq, m_duration);
            msleep(m_interval);
        }
    }

    volatile bool m_run;
    int m_freq;
    int m_duration;
    int m_interval;
};

//////////////////////////////////////////////////////////////////
/// WBApp class

/**
* @brief 构造函数
* @param [I] argc
* @param [I] *argv
*/
WBApp::WBApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_playThread(new PlaySoundThread(this))
    , m_workdir(QDir::currentPath())
    , m_readWriteLock(new QReadWriteLock(QReadWriteLock::Recursive))
    , m_icons(0)
    , m_printer(0)
    , m_mainWindow(0)
    , m_settings(new QSettings("settings.ini", QSettings::IniFormat, this))
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

//    setStyleSheet("QPushButton{border:2pxsolidblue;border-radius:6px;background-color:#E3EAA5;min-width:80px;}"
//                        "QComboBox{border:1pxsolidgray;border-radius:3px;padding:1px18px1px3px;}"
//                        "QLineEdit{border:1pxsolidgray;border-radius:5px;padding:08px;selection-background-color:darkgray;}");

    fnBeep = beep_winnt;
    if (!initialize())
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}

WBApp::~WBApp()
{
    m_playThread->stop();
    m_playThread->wait();
}

bool WBApp::initialize()
{
    // 安装翻译器
    QStringList searchDir;
    searchDir << "." << ".." << QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    QSettings settings("settings.ini", QSettings::IniFormat);
    QStringList translations = settings.value("settings/translations", "qt_zh_cn").toStringList();
    foreach(QString translatorFileName, translations)
    {
        QTranslator *qtTranslator = new QTranslator(this);
        loadTranslator(qtTranslator, searchDir, translatorFileName);
        installTranslator(qtTranslator);
    }

    // 设置信息
    setOrganizationName(tr("北京东方瑞威"));
    setApplicationName(tr("三台面轨道衡计量软件"));
    setApplicationVersion(VERSION_STR);

    // 加载仪器类型, 命令行优先级最高
    setTmpValue("debug/sim", true);
    setTmpValue("instr", value("settings/instr"));
    setTmpValue("port", value("settings/port"));

    QStringList args = QApplication::arguments();
    for (int i = 0; i < args.count(); i++)
    {
        if (args[i] == "-h")
        {
            usage();
            return false;
        }
        else if (args[i] == "-instr")
        {
            if (++i < args.size())
                setTmpValue("instr", args[i]);
        }
        else if (args[i] == "-port")
        {
            if (++i < args.size())
                setTmpValue("port", args[i]);
        }
        else if (args[i] == "-sim")
        {
            setTmpValue("debug/sim", true);
        }
    }

    // 登陆
    if (!login())
        return false;

    // 加载配置信息
    QList<QVariantMap> map = load_instr_info();
    if (map.isEmpty())
        return false;

    // 选择仪器
    QString instrName = tmpValue("instr").toString();
    bool sim = tmpValue("debug/sim").toBool();
    QStringList ports = tmpValue("port").toStringList();

    if (!sim && map.size() == 1)
        instrName = map.front().value("name").toString();

    if (instrName.isEmpty() || instrName == "ask" || sim)
    {
        InstrTypeDialog dialog(map);
        if (dialog.exec() != QDialog::Accepted)
            return false;

        instrName = dialog.instrName();
        ports = dialog.ports();

        setValue("settings/instr", instrName);
        setValue("settings/port", ports);
        setTmpValue("instr", instrName);
        setTmpValue("port", ports);
    }

    QVariantMap instrInfo = find_instr_info(map, instrName);
    if (instrInfo.isEmpty())
    {
        information(tr("找不到指定的仪器类型 %1").arg(instrName));
        return false;
    }

    // 系统资源
    m_icons = new WBIcons;
    m_caliManager = new CalibrationManager(this);
    m_printer = new ReportPrinter(this);

    // 创建工作区
    QVariantMap instrMap = instrInfo;
    instrMap["port"] = ports.value(0);
    m_workbenches += new Workbench(instrMap, this);

    // 删除无效的工作区
    for(int i=0; i<m_workbenches.size(); ++i)
    {
        if (m_workbenches[i]->instrument())
            continue;

        delete m_workbenches[i];
        m_workbenches.removeAt(i--);
    }

    // 如果是多个工作区, 需要创建一个次窗口包含其它窗口
    if (m_workbenches.size() > 1)
        createMainWindow();

    if (!m_mainWindow)
        m_mainWindow = m_workbenches.front()->mainWindow();
    m_mainWindow->showMaximized();

    emit initialized();

    bool systemUser = isSystemLogin();
    emit userLogin(systemUser);
    if (systemUser)
        emit systemLogin();
    else
        emit systemLogout();

    return true;
}

void WBApp::beep(int freq, int duration)
{
    fnBeep(freq, duration);
}

void WBApp::playSound(int freq, int duration)
{
    QtConcurrent::run(fnBeep, freq, duration);
}

void WBApp::playSound(int freq, int duration, int sleep, int repeat)
{
    QtConcurrent::run(beepLoop, freq, duration, sleep, repeat);
}

void WBApp::playSound(SoundType type)
{
    Q_UNUSED(type)
    m_playThread->start(1000, 800, 200);
}

void WBApp::stopSound()
{
    m_playThread->stop();
}

QVariant WBApp::value(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(m_readWriteLock);
    return m_settings->value(key, defaultValue);
}

void WBApp::setValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(m_readWriteLock);
    if (value.isValid())
        m_settings->setValue(key, value);
    else
        m_settings->remove(key);
}

QVariant WBApp::tmpValue(const QString &key, const QVariant &defaultValue) const
{
    QReadLocker locker(m_readWriteLock);
    return m_tmp.value(key, defaultValue);
}

void WBApp::setTmpValue(const QString &key, const QVariant &value)
{
    QWriteLocker locker(m_readWriteLock);
    if (value.isValid())
        m_tmp[key] = value;
    else
        m_tmp.remove(key);
}

QIcon WBApp::icon(const QString &name)
{
    return m_icons->icon(name);
}

bool WBApp::isSystemLogin() const
{
    return tmpValue("user/level").toInt() > Level10;
}

bool WBApp::login()
{
    LoginDialog dlg;
    if (dlg.exec() != QDialog::Accepted)
        return false;

    bool systemUser = isSystemLogin();
    emit userLogin(systemUser);
    if (systemUser)
        emit systemLogin();
    else
        emit systemLogout();

    return true;
}

void WBApp::about()
{
    QString text = "<html>\n"
            "<style type=\"text/css\">\n"
            "body	{ font-family: Sans-serif }\n"
            "table	{ margin-left: 9px; margin-right: 9px }\n"
            "td		{ margin: 3px }\n"
            ".key	{ padding-right: 6px }\n"
            ".value { padding-right: 16px }\n"
            "</style>\n";

    text += "<body><h2>" + qApp->applicationName() + "</h2>\n";
    text += tr("版本: ") + qApp->applicationVersion() + "<br/>\n";

    foreach(Workbench *w, workbenches())
    {
        text += "<br/>\n";
        QString title = w->instrInfo("title").toString();
        if (title.isEmpty())
            title = w->instrInfo("name").toString();
        text += title;
        text += "<hr>";
        text += w->about();
    }

    text += "</body></html>";

    QMessageBox::about(m_mainWindow, tr("关于"), text);
}

void WBApp::createMainWindow()
{
    m_mainWindow = new AppWindow;
}
