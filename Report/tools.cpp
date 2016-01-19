#include "tools.h"
#include "report.h"
#include "profile.h"
#include <QPen>
#include <QTextDocument>
#include <QPixmap>
#include <QFileDialog>
#include <QImageReader>
#include <QGraphicsView>

///////////////////////////////////////////////////////////////////////////////
// FieldItemFactory

ReportItem *FieldItemFactory::create(const QString &key)
{
	static const QString prefix("field:/");
	if (!key.startsWith(prefix))
		return 0;

	QStringList items = key.mid(prefix.length()).split(QChar('/'));
	if (items.size() != 2)
		return 0;

	return new ReportFieldItem(items[0], items[1]);
}

///////////////////////////////////////////////////////////////////////////////
// ProfileItemFactory

ReportItem *ProfileItemFactory::create(const QString &key)
{
	static const QString prefix("profile:/");
	if (!key.startsWith(prefix))
		return 0;

	QString name = key.mid(prefix.length());

	return new ReportProfileItem(name);
}

///////////////////////////////////////////////////////////////////////////////
// PropItemFactory

ReportItem *PropItemFactory::create(const QString &key)
{
	static const QString prefix("prop:/");
	if (!key.startsWith(prefix))
		return 0;

	QString type = key.mid(prefix.length());

	return new ReportPropItem(type);
}

///////////////////////////////////////////////////////////////////////////////
// GeneralRectCreator

GeneralRectCreator::GeneralRectCreator(const QString &iconFileName, 
									   const QString &group, 
									   QObject *parent)
	: GeneralCreator(iconFileName, RectSelection, group, parent)
{
}

GeneralRectCreator::GeneralRectCreator(const QString &iconFileName, QObject *parent)
	: GeneralCreator(iconFileName, RectSelection, QString(), parent)
{
}

GeneralRectCreator::~GeneralRectCreator()
{
}

void GeneralRectCreator::append(const QPointF &point)
{
	GeneralCreator::append(point);

	if (selection().size() < 2)
		return;

	if (!reportItem())
		prepare(create());

	updateGeometry(selection());
}

void GeneralRectCreator::move(const QPointF &point)
{
	GeneralCreator::move(point);

	if (reportItem())
		updateGeometry(selection());
}

bool GeneralRectCreator::end(bool ok)
{
	ok = GeneralCreator::end(ok);
	if (ok)
		accept();
	else
		reject();

	return ok;
}

void GeneralRectCreator::updateGeometry(const QPolygonF &b)
{
	reportItem()->updateGeometry(b);
}

///////////////////////////////////////////////////////////////////////////////
// ArrowCreator

QString ArrowCreator::key()
{
	return QObject::tr("指针");
}

void ArrowCreator::enter(QGraphicsView *view)
{
	Q_ASSERT(view);
	GeneralCreator::enter(view);
	view->setDragMode(QGraphicsView::RubberBandDrag);
}

void ArrowCreator::leave()
{
	Q_ASSERT(view());
	view()->setDragMode(QGraphicsView::NoDrag);
}

ReportItem *ArrowCreator::create()
{
	Q_ASSERT(false);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// LineCreator

QString LineCreator::key()
{
	return QObject::tr("直线");
}

ReportItem *LineCreator::create()
{
	return new ReportLineItem();
}

///////////////////////////////////////////////////////////////////////////////
// RectCreator

QString RectCreator::key()
{
	return QObject::tr("矩形");
}

ReportItem *RectCreator::create()
{
	return new ReportRectItem();
}

///////////////////////////////////////////////////////////////////////////////
// TextCreator

QString TextCreator::key()
{
	return QObject::tr("文本");
}

ReportItem *TextCreator::create()
{
	ReportTextItem *item = new ReportTextItem();
	return item;
}

void TextCreator::updateGeometry(const QPolygonF &b)
{
	GeneralRectCreator::updateGeometry(b);

	ReportTextItem *item = static_cast<ReportTextItem *>(reportItem());
	item->setSelected(true);
	if (item->flags() & QGraphicsItem::ItemIsFocusable)
		item->setTextInteractionFlags(Qt::TextEditorInteraction);
	if (!item->hasFocus() && (item->flags() & QGraphicsItem::ItemIsFocusable) )
		item->setFocus();
}

///////////////////////////////////////////////////////////////////////////////
// FieldCreator

FieldCreator::FieldCreator(const QString &group, const QString &table, const QString &field, QObject *parent)
	: TextCreator(group, parent)
	, m_key(QString("field:/%1/%2").arg(table).arg(field))
	, m_table(table)
	, m_field(field)
{
	m_name = profile.value(QString("%1/%2").arg(table).arg(field), m_field).toString();
}

ReportItem *FieldCreator::create()
{
	ReportTextItem *item = new ReportFieldItem(m_table, m_field);
	return item;
}

///////////////////////////////////////////////////////////////////////////////
// ImageCreator

QString ImageCreator::key()
{
	return QObject::tr("图像");
}

ReportItem *ImageCreator::create()
{
	return new ReportImageItem();
}

void ImageCreator::enter(QGraphicsView *view)
{
	GeneralRectCreator::enter(view);

	QStringList filter;
	foreach(QByteArray a, QImageReader::supportedImageFormats())
		filter.append("*." + a);

	QString file = QFileDialog::getOpenFileName(view->topLevelWidget(),
		QString(), QString(), tr("所有格式(%1);;所有文件(*.*)").arg(filter.join(";")));
	if (file.isEmpty())
		finish();
	else
	{
		ReportImageItem *item = new ReportImageItem();
		prepare(item);
		QPixmap pixmap(file);
		item->setPixmap(pixmap);
		item->scale((qreal)pixmap.widthMM()/pixmap.width(),
			(qreal)pixmap.heightMM()/pixmap.height());
		accept();
	}
}

///////////////////////////////////////////////////////////////////////////////
// TableCreator

QString TableCreator::key()
{
	return QObject::tr("结果");
}

ReportItem *TableCreator::create()
{
	return new ReportTableItem();
}

void TableCreator::updateGeometry(const QPolygonF &b)
{
	GeneralRectCreator::updateGeometry(b);

	reportItem()->graphicsItem()->setSelected(true);
}
