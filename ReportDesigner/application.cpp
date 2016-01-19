#include "application.h"
#include "workbench.h"
#include <QTextCodec>
#include <QLibraryInfo>
#include <QTranslator>
#include <QStringList>
#include <QLocale>
#include <QSettings>

static const char *config_file="reportdesigner.ini";

static bool loadTranslator(QTranslator *translator, const QStringList &dir, const QString &filename)
{
	foreach(const QString &path, dir)
		if (translator->load(filename, path))
			return true;
	return false;
}


Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
	, m_settings(new QSettings(config_file, QSettings::IniFormat, this))
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	setOrganizationName(tr("北京东方瑞威"));
	setApplicationName(tr("ReportDesigner"));

	initialize();
}

Application::~Application()
{
	delete m_settings;
}

void Application::initialize()
{
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

	m_workbench = new Workbench(this);

	emit initialized();
}

QVariant Application::value(const QString &key, const QVariant &defaultValue) const
{
	return m_settings->value(key, defaultValue);
}

void Application::setValue(const QString &key, const QVariant &value)
{
	m_settings->setValue(key, value);
}

QVariant Application::tmpValue(const QString &key, const QVariant &defaultValue) const
{
	return m_tmp.value(key, defaultValue);
}

void Application::setTmpValue(const QString &key, const QVariant &value)
{
	m_tmp[key] = value;
}
