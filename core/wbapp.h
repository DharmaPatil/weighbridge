/**
* @file wbapp.h
* @brief 轨道衡应用程序类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef WBAPP_H
#define WBAPP_H

#include "core_global.h"

#include <QCoreApplication>
#include <QApplication>
#include <QList>
#include <QDir>
#include <QMap>
#include <QVariant>

#undef qApp
#define qApp (static_cast<WBApp*>(QCoreApplication::instance()))

class Workbench;
class PlaySoundThread;
class QSettings;
class QReadWriteLock;
class QMainWindow;
class CalibrationManager;
class DataStorage;
class WBIcons;

/**
* @brief 轨道衡应用程序
*/
class CORE_EXPORT WBApp : public QApplication
{
    Q_OBJECT

public:
    enum SoundType {
        General,		// 通用
        SensorExp,		// 传感器异常
    };

    enum UserLevel
    {
        Level1,
        Level2,
        Level3,
        Level4,
        Level5,

        Level10 = 10,
    };

    WBApp(int &argc, char **argv);
    ~WBApp();

    QList<Workbench *> workbenches() const { return m_workbenches; }
    QDir workdir() const { return m_workdir; }

    bool initialize();
    static void beep(int freq, int duration);

    void playSound(int freq, int duration);
    void playSound(int freq, int duration, int sleep, int repeat);
    void playSound(SoundType type = General);
    void stopSound();

    QVariant value(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setValue(const QString &key, const QVariant &value);
    template<class T> T value(const QString &key) const { return value(key).value<T>(); }

    QVariant tmpValue(const QString &key, const QVariant &defaultValue=QVariant()) const;
    void setTmpValue(const QString &key, const QVariant &value);
    template<class T> T tmpValue(const QString &key) const { return tmpValue(key).value<T>(); }

    WBIcons *icons() const { return m_icons; }
    QIcon icon(const QString &name);

    bool isSystemLogin() const;
    CalibrationManager *calibrationManager() const { return m_caliManager; }
    DataStorage *printer() const { return m_printer; }

public slots:
    bool login();
    void about();

signals:
    void initialized();
    void userLogin(bool system);
    void systemLogin();
    void systemLogout();

private:
    void createMainWindow();

    QList<Workbench *> m_workbenches;
    PlaySoundThread *m_playThread;
    QDir m_workdir;
    QSettings *m_settings;
    QMap<QString, QVariant> m_tmp;
    mutable QReadWriteLock *m_readWriteLock;
    WBIcons *m_icons;
    CalibrationManager *m_caliManager;
    DataStorage *m_printer;

    QMainWindow *m_mainWindow;
};

#endif // WBAPP_H
