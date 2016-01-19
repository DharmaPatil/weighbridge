#include "itemfactory.h"
#include "itemloader.h"
#include "tools.h"
#include "profile.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QtSql>

ItemFactory::ItemFactory(QObject *parent)
	: QObject(parent)
{
	CreatorFactory *profileFactory = new ProfileItemFactory();
	CreatorFactory *propFactory = new PropItemFactory();

	registerFactory(new FieldItemFactory());
	registerFactory(profileFactory);
	registerFactory(propFactory);

	registerCreator(new ArrowCreator(this));
	registerCreator(new LineCreator(this));
	registerCreator(new RectCreator(this));
	registerCreator(new TextCreator(this));
	registerCreator(new ImageCreator(this));

	// 通用信息
	{
		const QString group(tr("通用信息"));
		registerCreator(new FactoryProxyCreator(profileFactory, group, "profile:/corporation", tr("用户名称")));

		registerCreator(new FactoryProxyCreator(propFactory, group, "prop:/date", tr("打印日期")));
		registerCreator(new FactoryProxyCreator(propFactory, group, "prop:/time", tr("打印时间")));
	}

	// 用户信息
	{
		QSqlDriver *driver = profile.dataDatabase().driver();
		QString tableName("corporation");
		QSqlRecord table = driver->record(tableName);
		const QString group(tr("用户信息"));
		for (int i=0; i<table.count(); i++)
		{
			QVariant var = profile.value(QString("%1/%2").arg(tableName).arg(table.fieldName(i)));
			if (var.isNull())
				continue;

			registerCreator(new FieldCreator(group, tableName, table.fieldName(i), this));
		}
		registerCreator(new TableCreator(group, this));
	}
}

ItemFactory::~ItemFactory()
{
	qDeleteAll(m_creatorFactory);
}

ItemFactory *ItemFactory::instance()
{
	static bool init = false;
	static ItemFactory instance;

	if (!init)
	{
		init = true;
	}

	return &instance;
}

void ItemFactory::registerCreator(ReportItemCreator *creator)
{
	if (!creator->parent())
		creator->setParent(this);
	m_factory[creator->key()] = creator;
	m_creators.append(creator);
}

void ItemFactory::registerFactory(CreatorFactory *factory)
{
	m_creatorFactory.append(factory);
}

ReportItemCreator *ItemFactory::creator(const QString &key) const
{
	return m_factory.value(key);
}

ReportItem *ItemFactory::create(const QString &key) const
{
	ReportItemCreator *ctor = creator(key);
	if (ctor)
		return ctor->create();

	foreach(CreatorFactory *factory, m_creatorFactory)
	{
		ReportItem *item = factory->create(key);
		if (item)
			return item;
	}

	return 0;
}
