#include "reportitem.h"
#include "report.h"
#include "tracker.h"
#include "itemfactory.h"
#include "profile.h"
#include "analysisresult.h"
#include "global.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTextCursor>
#include <QCursor>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPixmap>
#include <QDataStream>
#include <QBuffer>
#include <QMessageBox>
#include <QTextBlockFormat>
#include <QTextDocument>
#include <QtSql>
#include <QTableWidget>


///////////////////////////////////////////////////////////////////////////////
// ReportItem

ReportItem::ReportItem()
{
}

ReportItem::~ReportItem()
{
}

Report *ReportItem::report()
{
	return static_cast<Report*>(graphicsItem()->scene());
}

QString ReportItem::key() const
{
	return const_cast<ReportItem*>(this)->graphicsItem()->data(ReportItemCreatorKey).toString();
}

void ReportItem::setKey(const QString &key)
{
	graphicsItem()->setData(ReportItemCreatorKey, key);
}

QPen ReportItem::pen()
{
	return QPen(); 
}

QBrush ReportItem::brush()
{
	return QBrush();
}

QFont ReportItem::font()
{
	return QFont();
}

QPolygonF ReportItem::geometry() const
{
	return QPolygonF();
}

void ReportItem::updateGeometry(const QPolygonF &polygon)
{
}

QDataStream & operator<<(QDataStream &stream, const ReportItem *item)
{
	Q_ASSERT(item);
	Q_ASSERT(!item->key().isEmpty());

	stream << item->key() 
		<< const_cast<ReportItem*>(item)->graphicsItem()->pos()
		<< const_cast<ReportItem*>(item)->graphicsItem()->transform();
	const_cast<ReportItem*>(item)->save(stream);
	return stream;
}

QDataStream & operator>>(QDataStream &stream, ReportItem *&item)
{
	QString key;
	QPointF pos;
	QTransform transform;
	stream >> key >> pos >> transform;
	Q_ASSERT(!key.isEmpty());

	item = ItemFactory::instance()->create(key);
	if (!item)
		throw QObject::tr("无法创建 %1 对象").arg(key);
	item->setKey(key);
	item->load(stream);
	item->graphicsItem()->setPos(pos);
	item->graphicsItem()->setTransform(transform);
	return stream;
}

///////////////////////////////////////////////////////////////////////////////
// ReportLineItem

ReportLineItem::ReportLineItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
}

QPen ReportLineItem::pen() 
{
	return QGraphicsLineItem::pen(); 
}

void ReportLineItem::setPen(const QPen &pen)
{
	QGraphicsLineItem::setPen(pen);
}

void ReportLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(pen());
	painter->drawLine(line());

	if (option->state & QStyle::State_Selected)
	{
		const qreal penWidth = 0; // cosmetic pen

		const QColor fgcolor = option->palette.windowText().color();
		const QColor bgcolor( // ensure good contrast against fgcolor
			fgcolor.red()   > 127 ? 0 : 255,
			fgcolor.green() > 127 ? 0 : 255,
			fgcolor.blue()  > 127 ? 0 : 255);

		painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
		painter->drawLine(line());

		painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
		painter->drawLine(line());
	}
}

QPolygonF ReportLineItem::geometry() const
{
	return QPolygonF() << line().p1() << line().p2();
}

void ReportLineItem::updateGeometry(const QPolygonF &polygon)
{
	setLine(QLineF(polygon.value(0), polygon.value(1)));
}

Tracker *ReportLineItem::createTracker()
{
	return new LineTracker(this); 
}

void ReportLineItem::save(QDataStream &stream)
{
	quint8 version = 1;
	stream << version << line() << pen();
}

void ReportLineItem::load(QDataStream &stream)
{
	quint8 version;
	QLineF line;
	QPen pen;

	stream >> version >> line >> pen;
	setLine(line);
	setPen(pen);
}

///////////////////////////////////////////////////////////////////////////////
// ReportRectItem

ReportRectItem::ReportRectItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
}

QPen ReportRectItem::pen() 
{
	return QGraphicsRectItem::pen(); 
}

void ReportRectItem::setPen(const QPen &pen)
{
	QGraphicsRectItem::setPen(pen);
}

QBrush ReportRectItem::brush() 
{
	return QGraphicsRectItem::brush(); 
}

void ReportRectItem::setBrush(const QBrush &brush)
{
	QGraphicsRectItem::setBrush(brush);
}

QPolygonF ReportRectItem::geometry() const
{
	return QPolygonF() << rect().topLeft() << rect().bottomRight();
}

void ReportRectItem::updateGeometry(const QPolygonF &polygon)
{
	setRect(QRectF(polygon.value(0), polygon.value(1)));
}

Tracker *ReportRectItem::createTracker()
{
	return new TransformTracker(this, rect()); 
}

void ReportRectItem::save(QDataStream &stream)
{
	quint8 version = 1;
	stream << version << rect() << pen() << brush();
}

void ReportRectItem::load(QDataStream &stream)
{
	quint8 version;
	QRectF rect;
	QPen pen;
	QBrush brush;

	stream >> version >> rect >> pen >> brush;
	setRect(rect);
	setPen(pen);
	setBrush(brush);
}

///////////////////////////////////////////////////////////////////////////////
// ReportTextItem

ReportTextItem::ReportTextItem(bool editable)
{
	m_editable = editable;
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
	setFlag(ItemIsFocusable, editable);
	connect(document(), SIGNAL(contentsChanged()), this, SLOT(contentsChanged()));
}

QPolygonF ReportTextItem::geometry() const
{
	return QPolygonF() << pos() << boundingRect().bottomRight();
}

void ReportTextItem::updateGeometry(const QPolygonF &polygon)
{
	qreal width = polygon.value(1).x() - polygon.value(0).x();
	if (width < 2)
		width = -1;

	setPos(polygon.value(0));
	setTextWidth(width);
}

Tracker *ReportTextItem::createTracker()
{
	return new TextTracker(this); 
}

void ReportTextItem::setEditable(bool editable)
{
	if (m_editable == editable)
		return;

	if (!(m_editable = editable))
		setTextInteractionFlags(Qt::NoTextInteraction);
}

void ReportTextItem::focusOutEvent(QFocusEvent *event)
{
	setTextInteractionFlags(Qt::NoTextInteraction);
	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	QGraphicsTextItem::focusOutEvent(event);
}

void ReportTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_editable && textInteractionFlags() == Qt::NoTextInteraction)
	{
		setTextInteractionFlags(Qt::TextEditorInteraction);
		setFocus();
	}
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

QFont ReportTextItem::font()
{
	return QGraphicsTextItem::font();
}

void ReportTextItem::setFont(const QFont &font)
{
	QGraphicsTextItem::setFont(font);
}

Qt::Alignment ReportTextItem::alignment()
{
	return m_alignment;
}

void ReportTextItem::setAlignment(Qt::Alignment align)
{
	m_alignment = align;
	QTextBlockFormat fmt;
	fmt.setAlignment(align);
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(fmt);
	cursor.clearSelection();
	setTextCursor(cursor);
}

void ReportTextItem::setPlainText(const QString &text)
{
	QGraphicsTextItem::setPlainText(text);
	setAlignment(alignment());
}

void ReportTextItem::setHtml(const QString &html)
{
	QGraphicsTextItem::setHtml(html);
	setAlignment(alignment());
}

void ReportTextItem::save(QDataStream &stream)
{
	quint8 version = 2;
	stream << version << (int)alignment() << pen() << brush() 
		   << font() << textWidth() << toPlainText();
}

void ReportTextItem::load(QDataStream &stream)
{
	quint8 version;
	QPen pen;
	Qt::Alignment align;
	QBrush brush;
	QFont font;
	qreal width;
	QString html;

	stream >> version >> (int&)align >> pen >> brush >> font >> width >> html;
	setPen(pen);
	setBrush(brush);
	setFont(font);
	setTextWidth(width);
	if (version == 1)
		setHtml(html);
	else
		setPlainText(html);
	setAlignment(align);
}

void ReportTextItem::contentsChanged()
{
	if (report())
		report()->itemContentsChange(this);
}

///////////////////////////////////////////////////////////////////////////////
// ReportFieldItem

ReportFieldItem::ReportFieldItem()
	: ReportTextItem(false)
{
}

ReportFieldItem::ReportFieldItem(const QString &table, const QString &field)
	: ReportTextItem(false), m_table(table), m_field(field)
{
	setDesignerText();
}

void ReportFieldItem::setTable(const QString &table)
{
	m_table = table;
	setDesignerText();
}

void ReportFieldItem::setField(const QString &field)
{
	m_field = field;
	setDesignerText();
}

void ReportFieldItem::setDesignerText()
{
	QString key = QString("%1/%2").arg(m_table).arg(m_field);
	QString text = profile.value(key, m_field).toString();
	setPlainText(QString("[%1]").arg(text));
}

void ReportFieldItem::updateData()
{
	Report *doc = report();
	if (!doc)
		return;

	QSqlRecord r = doc->record(m_table);
	QVariant val = r.value(m_field);
	setPlainText(val.toString());
}

void ReportFieldItem::save(QDataStream &stream)
{
	ReportTextItem::save(stream);

	quint8 version = 1;
	stream << version << m_table << m_field;
}

void ReportFieldItem::load(QDataStream &stream)
{
	ReportTextItem::load(stream);

	quint8 version;
	stream >> version >> m_table >> m_field;
}

///////////////////////////////////////////////////////////////////////////////
// ReportImageItem

ReportImageItem::ReportImageItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
}

QPolygonF ReportImageItem::geometry() const
{
	return QPolygonF() << pos() << QPointF(pixmap().width(), pixmap().height());
}

void ReportImageItem::updateGeometry(const QPolygonF &polygon)
{
	QPointF s = polygon[1] - polygon[0];
	resetTransform();
	setOffset(polygon[0]);
	scale(s.x() / pixmap().width(), s.y() / pixmap().height());
}

void ReportImageItem::save(QDataStream &stream)
{
	quint8 version = 1;
	stream << version << pixmap();
}

void ReportImageItem::load(QDataStream &stream)
{
	quint8 version;
	QPixmap pixmap;

	stream >> version >> pixmap;
	setPixmap(pixmap);
}

///////////////////////////////////////////////////////////////////////////////
// ReportTableItem

ReportTableItem::ReportTableItem()
	: m_spacing(8)
	, m_rowHeight(24)
	, m_rows(0)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsScenePositionChanges);
	setColumnCount(IndexCount);
	setColumnFlags(IndexResult, Qt::AlignRight);

//	int row = 0;
//	setItemText(row, IndexAbbr,		QObject::tr("代码"));
//	setItemText(row, IndexCode,		QObject::tr("项目名称"));
//	setItemText(row, IndexResult,	QObject::tr("结果"));
//	setItemText(row, IndexUnits,	QObject::tr("单位"));
//	setItemText(row, IndexFlag,		QObject::tr("标志"));
//	setItemText(row, IndexNormal,	QObject::tr("参考值"));

//	row++;
//	AnalysisResult result(12.3);
//	result.setFlags(0xffff);
//	setItemText(row, IndexAbbr,		QObject::tr("PT-S"));
//	setItemText(row, IndexCode,		QObject::tr("凝血酶原时间"));
//	setItemText(row, IndexResult,	result.toString(AnalysisResult::FormatValue));
//	setItemText(row, IndexUnits,	"s");
//	setItemText(row, IndexFlag,		result.suffix());
//	setItemText(row, IndexNormal,	"10.0-13.0");
}

QRectF ReportTableItem::boundingRect() const
{
	return QRectF(QPointF(0, 0), actualSize().expandedTo(m_size)); 
}

void ReportTableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setFont(m_font);
	painter->setPen(Qt::black);

	qreal x = 0;
	int flags = Qt::AlignVCenter | Qt::TextSingleLine | Qt::TextIncludeTrailingSpaces;
	for(int col = 0; col < columnCount(); col++)
	{
		qreal w = columnWidth(col);
		QRectF r(x+m_spacing, 0, w-2*m_spacing, rowHeight());
		int textFlags = m_headers.value(col).flags | flags;
		
		for(int row = 0; row < rowCount(); row++)
		{
			painter->drawText(r, textFlags, itemText(row, col));
			r.translate(0, rowHeight());
		}
		x += w;
	}

	if (option->state & QStyle::State_Selected)
	{
		const qreal penWidth = 0; // cosmetic pen

		const QColor fgcolor = option->palette.windowText().color();
		const QColor bgcolor( // ensure good contrast against fgcolor
			fgcolor.red()   > 127 ? 0 : 255,
			fgcolor.green() > 127 ? 0 : 255,
			fgcolor.blue()  > 127 ? 0 : 255);

		QRectF r(QPointF(), m_size);
		painter->setBrush(QBrush());

		painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
		painter->drawRect(r);

		painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
		painter->drawRect(r);
	}
}

Tracker *ReportTableItem::createTracker()
{
	return new TableTracker(this);
}

QPolygonF ReportTableItem::geometry() const
{
	QPolygonF p;
	p << QPointF(0, 0) << QPointF(m_size.width(), m_size.height());
	p << QPointF(0, rowHeight());
	qreal x = 0;
	for(int i=0; i<columnCount(); i++)
	{
		x += columnWidth(i);
		p << QPointF(x, 0);
	}
	return p;
}

void ReportTableItem::updateGeometry(const QPolygonF &polygon)
{
	prepareGeometryChange();

	if (polygon.size() == 2)
		setPos(polygon.front());

	QPolygonF p = polygon;
	p.translate(-polygon.front());
	m_size.setWidth(p[1].x());
	m_size.setHeight(p[1].y());

	if (p.size() <= 2)
		return;

	setRowHeight(p[2].y());
	qreal x = 0;
	for(int i=0; i<columnCount() && i+3 < p.size(); i++)
	{
		setColumnWidth(i, p.value(3+i).x() - x);
		x = p.value(3+i).x();
	}
}

QDataStream &operator<<(QDataStream &stream, const ReportTableItem::Header &header)
{
	return stream << header.width << header.flags;
}

QDataStream &operator>>(QDataStream &stream, ReportTableItem::Header &header)
{
	return stream >> header.width >> header.flags;
}

void ReportTableItem::save(QDataStream &stream)
{
	quint8 version = 1;
	stream << version;
	stream << m_rows << m_columns << m_spacing << m_rowHeight << m_size 
		<< m_cells << m_headers << m_font;
}

void ReportTableItem::load(QDataStream &stream)
{
	quint8 version;

	stream >> version;
	stream >> m_rows >> m_columns >> m_spacing >> m_rowHeight >> m_size 
		>> m_cells >> m_headers >> m_font;
}

void ReportTableItem::setRowHeight(qreal height)
{
	prepareGeometryChange();
	m_rowHeight = height;
}

qreal ReportTableItem::columnWidth(int i) const
{
	return m_headers.value(i).width;
}

void ReportTableItem::setColumnWidth(int i, qreal width)
{
	if (m_headers.size() <= i)
	{
		m_headers.resize(i + 1);
		if (m_columns < m_headers.size())
			m_columns = m_headers.size();
	}
	prepareGeometryChange();
	m_headers[i].width = width;
}

int ReportTableItem::columnFlags(int i) const
{
	return m_headers.value(i).flags;
}

void ReportTableItem::setColumnFlags(int i, int flags)
{
	if (m_headers.size() <= i)
	{
		m_headers.resize(i + 1);
		if (m_columns < m_headers.size())
			m_columns = m_headers.size();
	}
	m_headers[i].flags = flags;
	update();
}

QString ReportTableItem::itemText(int row, int col) const
{
	Cell1D r = m_cells.value(row);
	return r.value(col);
}

void ReportTableItem::setItemText(int row, int col, const QString &text)
{
	if (m_cells.size() <= row)
	{
		m_cells.resize(row + 1);
		if (m_rows < m_cells.size())
			m_rows = m_cells.size();
	}

	Cell1D &r = m_cells[row];
	if (r.size() <= col)
	{
		r.resize(col + 1);
		if (m_columns < r.size())
			m_columns = r.size();
	}

	r[col] = text;
}

QSizeF ReportTableItem::actualSize() const
{
	qreal w = 0, h = 0;

	for(int i=0; i<columnCount(); i++)
		w += columnWidth(i);

	h = rowCount() * rowHeight();

	return QSizeF(w, h);
}

void ReportTableItem::clear()
{
	m_cells.clear();
	setRowCount(0);
}

void ReportTableItem::adjustSize()
{
	m_size = actualSize();
}

void  ReportTableItem::setFont(const QFont &font)
{
	m_font = font;
	update();
}

inline QString fixedValue(double val, int precision)
{
	return QString("%1").arg(val, 0, 'f', precision);
}

void ReportTableItem::updateData()
{
	clear();
	if (!report())
		return;

	QSqlQuery q(profile.dataDatabase());
	if (!q.exec(QString("select item, subitem, abbr, code, result, unit, flag, "
		"lowerMarkLimit, upperMarkLimit from result "
		"where patientId=%1 order by displayOrder asc").arg(report()->patientId())))
		TRACE_LAST_ERROR(q);
	enum { item, subitem, abbr, code, result, unit, flag, lowerMarkLimit, upperMarkLimit };

	QSqlQuery ai(profile.database());
	if (!ai.prepare("select precision, visible "
					"from assayItem, assayTemplate "
					"where assayTemplate.id = assayItem.assayTemplateId and "
					" assayTemplate.name = :item and "
					" assayItem.name = :subitem"))
		TRACE_LAST_ERROR(ai);
	enum { aiPrecision, aiVisible };

	int row = 0;
//	for(; q.next(); )
//	{
//		ai.bindValue(":item", q.value(item));
//		ai.bindValue(":subitem", q.value(subitem));
//		if (!ai.exec())
//			TRACE_LAST_ERROR(ai);

//		if (!ai.next() || !ai.value(aiVisible).toBool())
//			continue;

//		//QString vitem = q.value(item).toString();
//		//QString vsubitem = q.value(subitem).toString();

//		int vprecision = ai.value(aiPrecision).toInt();
//		QString vabbr = q.value(abbr).toString();
//		QString vcode = q.value(code).toString();
//		AnalysisResult vresult = q.value(result).toDouble();
//		vresult.setFlags(q.value(flag).toInt());
//		QString vunit = q.value(unit).toString();
//		double lowerLimit = q.value(lowerMarkLimit).toDouble();
//		double upperLimit = q.value(upperMarkLimit).toDouble();
//		QString vnormal;
//		if (lowerLimit > 0 || upperLimit > 0)
//			vnormal = QString("%1-%2")
//					.arg(fixedValue(lowerLimit, vprecision))
//					.arg(fixedValue(upperLimit, vprecision));

//		setItemText(row, IndexAbbr,		vabbr);
//		setItemText(row, IndexCode,		vcode);
//        setItemText(row, IndexResult,	vresult.toString(AnalysisResult::FormatValue, 'f', vprecision));
//        //setItemText(row, IndexResult,	fixedValue(vresult, vprecision));
//		setItemText(row, IndexUnits,	vunit);
//		setItemText(row, IndexFlag,		vresult.suffix());
//		setItemText(row, IndexNormal,	vnormal);

//		row++;
//	}
}

///////////////////////////////////////////////////////////////////////////////
// ReportProfileItem

ReportProfileItem::ReportProfileItem()
	: ReportTextItem(false)
{
}

ReportProfileItem::ReportProfileItem(const QString &propName)
	: ReportTextItem(false), m_propName(propName)
{
	setDesignerText();
}

void ReportProfileItem::setPropName(const QString &propName)
{
	m_propName = propName;
	setDesignerText();
}

void ReportProfileItem::setDesignerText()
{
	setPlainText(QString("[%1]").arg(m_propName));
}

void ReportProfileItem::updateData()
{
	setPlainText(profile.value(m_propName).toString());
}

void ReportProfileItem::save(QDataStream &stream)
{
	ReportTextItem::save(stream);

	quint8 version = 1;
	stream << version << m_propName;
}

void ReportProfileItem::load(QDataStream &stream)
{
	ReportTextItem::load(stream);

	quint8 version;
	stream >> version >> m_propName;
}

///////////////////////////////////////////////////////////////////////////////
// ReportPropItem

ReportPropItem::ReportPropItem()
	: ReportTextItem(false)
{
}

ReportPropItem::ReportPropItem(const QString &propName)
	: ReportTextItem(false), m_propName(propName)
{
	updateData();
}

void ReportPropItem::setPropName(const QString &propName)
{
	m_propName = propName;
	updateData();
}

void ReportPropItem::updateData()
{
	if (m_propName == "date")
		setPlainText(QDate::currentDate().toString(Qt::DefaultLocaleLongDate));
	else if (m_propName == "time")
		setPlainText(QTime::currentTime().toString());
	else
		setPlainText(m_propName);
}

void ReportPropItem::save(QDataStream &stream)
{
	ReportTextItem::save(stream);

	quint8 version = 1;
	stream << version << m_propName;
}

void ReportPropItem::load(QDataStream &stream)
{
	ReportTextItem::load(stream);

	quint8 version;
	stream >> version >> m_propName;
}
