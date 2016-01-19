#pragma once

#include <QApplication>
#include <QVariant>
#include <QMap>


#undef qApp
#define qApp (static_cast<Application*>(QCoreApplication::instance()))


class QSettings;
class Workbench;


class Application : public QApplication
{
	Q_OBJECT

public:
	Application(int &argc, char **argv);
	~Application();

	Workbench *workbench() const { return m_workbench; }

	void initialize();

	QVariant value(const QString &key, const QVariant &defaultValue=QVariant()) const;
	void setValue(const QString &key, const QVariant &value);

	QVariant tmpValue(const QString &key, const QVariant &defaultValue=QVariant()) const;
	void setTmpValue(const QString &key, const QVariant &value);

signals:
	void initialized();

private:
	QSettings *m_settings;
	QMap<QString, QVariant> m_tmp;
	Workbench *m_workbench;
};

