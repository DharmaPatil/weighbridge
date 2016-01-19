#pragma once

#include "ReportItemCreator.h"
#include "reportitem.h"
#include <QIcon>

class REPORT_EXPORT CreatorFactory
{
public:
	virtual ~CreatorFactory() {}
	virtual ReportItem *create(const QString &key) = 0;
};

class FieldItemFactory : public CreatorFactory
{
public:
	ReportItem *create(const QString &key);
};

class ProfileItemFactory : public CreatorFactory
{
public:
	ReportItem *create(const QString &key);
};

class PropItemFactory : public CreatorFactory
{
public:
	ReportItem *create(const QString &key);
};

class REPORT_EXPORT GeneralCreator : public ReportItemCreator
{
public:
	GeneralCreator(const QString &iconFileName, SelectionMode mode, 
		const QString &group = QString(), QObject *parent = 0)
		: ReportItemCreator(mode, parent)
		, m_icon(QString(":/Resources/%1.png").arg(iconFileName))
		, m_group(group)
	{
	}

	QString name() { return key(); }
	QString group() { return m_group; }
	QIcon icon() { return m_icon; }

private:
	QIcon m_icon;
	QString m_group;
};

class REPORT_EXPORT ArrowCreator : public GeneralCreator
{
public :
	explicit ArrowCreator(QObject *parent = 0) : GeneralCreator("toolArrow", RectSelection) {}
	QString key();

	virtual void enter(QGraphicsView *view);
	virtual void leave();
	virtual bool eventFilter(QObject *watched, QEvent *e) { return false; }
	virtual ReportItem *create();
};

class REPORT_EXPORT GeneralRectCreator : public GeneralCreator
{
public:
	GeneralRectCreator(const QString &iconFileName, const QString &group, QObject *parent = 0);
	GeneralRectCreator(const QString &iconFileName, QObject *parent = 0);
	~GeneralRectCreator();

protected:
	virtual void updateGeometry(const QPolygonF &b);

	void append(const QPointF &point);
	void move(const QPointF &point);
	bool end(bool ok);
};

class REPORT_EXPORT LineCreator : public GeneralRectCreator
{
public:
	explicit LineCreator(QObject *parent = 0) : GeneralRectCreator("toolLine", parent) {}
	QString key();

protected:
	virtual ReportItem *create();
};

class REPORT_EXPORT RectCreator : public GeneralRectCreator
{
public:
	explicit RectCreator(QObject *parent = 0) : GeneralRectCreator("toolRect", parent) {}
	QString key();

protected:
	virtual ReportItem *create();
};

class REPORT_EXPORT TextCreator : public GeneralRectCreator
{
public:
	explicit TextCreator(QObject *parent = 0) : GeneralRectCreator("toolText", parent) {}
	TextCreator(const QString &group, QObject *parent = 0) 
		: GeneralRectCreator("toolText", group, parent) {}

	QString key();

protected:
	virtual ReportItem *create();
	virtual void updateGeometry(const QPolygonF &b);
};

class REPORT_EXPORT FieldCreator : public TextCreator
{
public:
	FieldCreator(const QString &group, const QString &table, 
		const QString &field, QObject *parent = 0);
	QString key() { return m_key; }
	QString name() { return m_name; }

protected:
	virtual ReportItem *create();

private:
	QString m_key;
	QString m_name;
	QString m_table;
	QString m_field;
};

class REPORT_EXPORT ImageCreator : public GeneralRectCreator
{
public:
	explicit ImageCreator(QObject *parent = 0) : GeneralRectCreator("toolImage", parent) {}
	QString key();

	virtual void enter(QGraphicsView *view);

protected:
	virtual ReportItem *create();
};

class REPORT_EXPORT TableCreator : public GeneralRectCreator
{
public:
	explicit TableCreator(const QString &group, QObject *parent = 0) 
		: GeneralRectCreator("toolText", group, parent) {}

	QString key();

protected:
	virtual ReportItem *create();
	virtual void updateGeometry(const QPolygonF &b);
};

class REPORT_EXPORT FactoryProxyCreator : public GeneralRectCreator
{
public:
	FactoryProxyCreator(CreatorFactory *factory, const QString &group, 
		const QString &key, const QString &name, QObject *parent = 0) 
		: GeneralRectCreator("toolText", group, parent)
		, m_factory(factory), m_key(key), m_name(name)
	{}

	QString key() { return m_key; }
	QString name() { return m_name; }

protected:
	virtual ReportItem *create() { return m_factory->create(m_key); }

	CreatorFactory *m_factory;
	QString m_key;
	QString m_name;
};
