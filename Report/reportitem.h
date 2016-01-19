#ifndef REPORTITEM_H
#define REPORTITEM_H

#include "report_global.h"
#include "report.h"
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QVector>

class QPen;
class QBrush;
class QFont;
class Report;
class Tracker;
class QGraphicsItem;
class QDataStream;
class QTableWidget;

///////////////////////////////////////////////////////////////////////////////
// ReportItem

class REPORT_EXPORT ReportItem
{
public:
	ReportItem();
	virtual ~ReportItem();
	virtual QGraphicsItem *graphicsItem() = 0;
	Report *report();

	enum DataKey
	{
		ReportItemKey = 1,
		ReportItemCreatorKey,
	};

	QString key() const;
	void setKey(const QString &key);

	virtual QPen pen();
	virtual void setPen(const QPen &pen) {}

	virtual QBrush brush();
	virtual void setBrush(const QBrush &brush) {}

	virtual QFont font();
	virtual void setFont(const QFont &font) {}

	virtual Qt::Alignment alignment() { return 0; }
	virtual void setAlignment(Qt::Alignment alignment) {}

	virtual QPolygonF geometry() const;
	virtual void updateGeometry(const QPolygonF &polygon);

	virtual void updateData() {}

	virtual Tracker *createTracker() { return 0; }

	virtual void save(QDataStream &stream) = 0;
	virtual void load(QDataStream &stream) = 0;
};

Q_DECLARE_METATYPE(ReportItem*)

REPORT_EXPORT QDataStream & operator<<(QDataStream &stream, const ReportItem *item);
REPORT_EXPORT QDataStream & operator>>(QDataStream &stream, ReportItem *&item);

inline ReportItem *reportItem_cast(QGraphicsItem *item)
{
	return reinterpret_cast<ReportItem*>(item->data(ReportItem::ReportItemKey).toInt());
}

///////////////////////////////////////////////////////////////////////////////
// ReportItemImpl

template<class GraphicsItemBase, class ReportItemBase = ReportItem>
class ReportItemImpl : public GraphicsItemBase, public ReportItemBase
{
public:
	ReportItemImpl()
	{
		GraphicsItemBase::setData(ReportItem::ReportItemKey,
			reinterpret_cast<int>(static_cast<ReportItem*>(this)) );
	}
	QGraphicsItem *graphicsItem()
	{
		return static_cast<QGraphicsItem*>(this);
	}

protected:
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
	{
		QVariant val = GraphicsItemBase::itemChange(change, value);
		if (ReportItemBase::report())
			ReportItemBase::report()->itemChange(this, change, value, val);
		return val;
	}
};

///////////////////////////////////////////////////////////////////////////////
// ReportLineItem

class REPORT_EXPORT ReportLineItem : public ReportItemImpl<QGraphicsLineItem>
{
public:
	ReportLineItem();

	QPen pen();
	void setPen(const QPen &pen);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	QPolygonF geometry() const;
	void updateGeometry(const QPolygonF &polygon);
	Tracker *createTracker();

	void save(QDataStream &stream);
	void load(QDataStream &stream);
};

///////////////////////////////////////////////////////////////////////////////
// ReportRectItem

class REPORT_EXPORT ReportRectItem : public ReportItemImpl<QGraphicsRectItem>
{
public:
	ReportRectItem();

	QPen pen();
	void setPen(const QPen &pen);

	QBrush brush();
	void setBrush(const QBrush &brush);

	QPolygonF geometry() const;
	void updateGeometry(const QPolygonF &polygon);
	Tracker *createTracker();

	void save(QDataStream &stream);
	void load(QDataStream &stream);
};

///////////////////////////////////////////////////////////////////////////////
// ReportTextItem

class REPORT_EXPORT ReportTextItem : public ReportItemImpl<QGraphicsTextItem>
{
	Q_OBJECT
public:
	ReportTextItem(bool editable = true);

	QPolygonF geometry() const;
	void updateGeometry(const QPolygonF &polygon);
	Tracker *createTracker();

	virtual QFont font();
	virtual void setFont(const QFont &font);

	Qt::Alignment alignment();
	void setAlignment(Qt::Alignment align);

	void setPlainText(const QString &text);
	void setHtml(const QString &html);

	void save(QDataStream &stream);
	void load(QDataStream &stream);

	void setEditable(bool editable);

protected:
	void focusOutEvent(QFocusEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private slots:
	void contentsChanged();

private:
	bool m_editable;
	Qt::Alignment m_alignment;
};

///////////////////////////////////////////////////////////////////////////////
// ReportFieldItem

class REPORT_EXPORT ReportFieldItem : public ReportTextItem
{
public:
	ReportFieldItem();
	ReportFieldItem(const QString &table, const QString &field);

	QString table() const { return m_table; }
	void setTable(const QString &table);

	QString field() const { return m_field; }
	void setField(const QString &field);

	void updateData();

	void save(QDataStream &stream);
	void load(QDataStream &stream);

private:
	void setDesignerText();

	QString m_table;
	QString m_field;
};

///////////////////////////////////////////////////////////////////////////////
// ReportImageItem

class REPORT_EXPORT ReportImageItem : public ReportItemImpl<QGraphicsPixmapItem>
{
public:
	ReportImageItem();

	QPolygonF geometry() const;
	void updateGeometry(const QPolygonF &polygon);

	void save(QDataStream &stream);
	void load(QDataStream &stream);
};

///////////////////////////////////////////////////////////////////////////////
// ReportTableItem

class REPORT_EXPORT ReportTableItem : public ReportItemImpl<QGraphicsItem>
{
public:
	ReportTableItem();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	QPolygonF geometry() const;
	void updateGeometry(const QPolygonF &polygon);

	void save(QDataStream &stream);
	void load(QDataStream &stream);

	int rowCount() const { return m_rows; }
	void setRowCount(int count) { m_rows = count; }

	int columnCount() const { return m_columns; }
	void setColumnCount(int count) { m_columns = count; }

	qreal columnWidth(int i) const;
	void setColumnWidth(int i, qreal width);

	int columnFlags(int i) const;
	void setColumnFlags(int i, int flags);

	qreal rowHeight() const { return m_rowHeight; }
	void setRowHeight(qreal height);

	QString itemText(int row, int col) const;
	void setItemText(int row, int col, const QString &text);

	void clear();
	void adjustSize();

	QFont font() { return m_font; }
	void setFont(const QFont &font);
	
	void updateData();
	virtual Tracker *createTracker();

private:
	QSizeF actualSize() const;

	typedef QVector<QString> Cell1D;
	typedef QVector<Cell1D> Cell2D;

	enum ColumnIndex
	{
		IndexAbbr,
		IndexCode,
		IndexResult,
		IndexUnits,
		IndexFlag,
		IndexNormal,

		IndexCount,
	};

	struct Header
	{
		qreal width;
		int flags;

		Header() : width(100), flags(0) {}
	};

	friend QDataStream &operator<<(QDataStream &stream, const Header &header);
	friend QDataStream &operator>>(QDataStream &stream, Header &header);

	int m_rows;
	int m_columns;
	qreal m_spacing;
	qreal m_rowHeight;
	QSizeF m_size;
	Cell2D m_cells;
	QVector<Header> m_headers;
	QFont m_font;
};

///////////////////////////////////////////////////////////////////////////////
// ReportProfileItem

class REPORT_EXPORT ReportProfileItem : public ReportTextItem
{
public:
	ReportProfileItem();
	ReportProfileItem(const QString &propName);

	QString propName() const { return m_propName; }
	void setPropName(const QString &propName);

	void updateData();

	void save(QDataStream &stream);
	void load(QDataStream &stream);

private:
	void setDesignerText();

	QString m_propName;
};

///////////////////////////////////////////////////////////////////////////////
// ReportPropItem

class REPORT_EXPORT ReportPropItem : public ReportTextItem
{
public:
	ReportPropItem();
	ReportPropItem(const QString &propName);

	QString propName() const { return m_propName; }
	void setPropName(const QString &propName);

	void updateData();

	void save(QDataStream &stream);
	void load(QDataStream &stream);

private:
	QString m_propName;
};

#endif // REPORTITEM_H
