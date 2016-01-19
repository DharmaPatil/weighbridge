#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "report_global.h"
#include <QObject>
#include <QMap>
#include <QList>

class ReportItem;
class ReportItemCreator;
class CreatorFactory;

class REPORT_EXPORT ItemFactory : public QObject
{
	Q_OBJECT

public:
	explicit ItemFactory(QObject *parent = 0);
	~ItemFactory();

	static ItemFactory *instance();

	void registerCreator(ReportItemCreator *creator);
	void registerFactory(CreatorFactory *factory);
	ReportItemCreator *creator(const QString &key) const;
	QList<ReportItemCreator*> creators() const { return m_creators; }

	ReportItem *create(const QString &key) const;

private:
	QMap<QString, ReportItemCreator*> m_factory;
	QList<ReportItemCreator*> m_creators;
	QList<CreatorFactory*> m_creatorFactory;
};

#endif // ITEMFACTORY_H
