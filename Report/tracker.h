#pragma once

#include "report_global.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

class Tracker;
class ReportItem;
class ReportTextItem;
class ReportLineItem;
class ReportTableItem;
class TransformCommand;
class PosCommand;

class REPORT_EXPORT TrackerHandle
{
public:
	TrackerHandle(int id, Tracker *tracker);
	virtual ~TrackerHandle();

	virtual QGraphicsItem *graphicsItem() = 0;
	virtual void setHandleSize(int size) = 0;

	int id() const { return m_id; }
	Tracker *tracker() const { return m_tracker; }

protected:
	const QPointF &initPos() const { return m_initPos; }

	void begin();
	void move(QPointF &pos, Qt::KeyboardModifiers modifiers);
	void end();

private:
	int m_id;
	Tracker *m_tracker;
	QPointF m_initPos;
};

template<class ShapeClass, class HandleClass = TrackerHandle>
class TrackerHandleT : public ShapeClass, public HandleClass
{
public:
	TrackerHandleT(int id, Tracker *tracker);

	QGraphicsItem *graphicsItem() { return this; }
	void setHandleSize(int size);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

class GraphicsDiamondItem : public QAbstractGraphicsShapeItem
{
public:
	explicit GraphicsDiamondItem(QGraphicsItem *parent = 0);

	QRectF rect() const { return m_rect; }
	void setRect(const QRectF &rect);
	void setRect(qreal x, qreal y, qreal w, qreal h) { setRect(QRectF(x, y, w, h)); }

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	QPolygonF polygon() const;

	QRectF m_rect;
};

typedef TrackerHandleT<QGraphicsRectItem> TrackerRectHandle;
typedef TrackerHandleT<QGraphicsEllipseItem> TrackerEllipseHandle;
typedef TrackerHandleT<GraphicsDiamondItem> TrackerDiamondHandle;

class REPORT_EXPORT Tracker : public QGraphicsItem
{
public:
	typedef QList<TrackerHandle*> HandleList;
	enum { DefaultHandleSize = 7 };

	Tracker(ReportItem *item);
	virtual ~Tracker();

	QPolygonF polygon() const;
	void setPolygon(const QPolygonF &polygon);

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	enum { Type = UserType + 1 };
	virtual int type() const { return Type; }

	void enableRecordPos(bool b) { m_recordPos = b; }

protected:
	virtual void begin(int id);
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);
	virtual void end(int id, const QPointF &pos, const QPointF &oldPos);

	virtual void snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers);
	virtual void snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers);

	virtual HandleList createHandle(const QPolygonF &polygon) = 0;
	virtual void updateHandlePos(const QPolygonF &polygon) = 0;

	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

	QPolygonF m_polygon;
	HandleList m_handles;
	ReportItem *m_item;
	TransformCommand *m_cmd;
	PosCommand *m_posCmd;
	bool m_recordPos;

	friend class TrackerHandle;
};

class REPORT_EXPORT LineTracker : public Tracker
{
public:
	LineTracker(ReportLineItem *line);

	QLineF line() const;
	void setLine(const QLineF &line);

protected:
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);
	virtual void end(int id, const QPointF &pos, const QPointF &oldPos);

	virtual HandleList createHandle(const QPolygonF &polygon);
	virtual void updateHandlePos(const QPolygonF &polygon);

private:
	ReportLineItem *m_line;
};

class REPORT_EXPORT RectTracker : public Tracker
{
public:
	RectTracker(ReportItem *item);
	RectTracker(ReportItem *item, const QRectF &rect);

	QRectF rect() const;
	void setRect(const QRectF &rect);

protected:
	virtual void begin(int id);
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);
	virtual void end(int id, const QPointF &pos, const QPointF &oldPos);

	virtual HandleList createHandle(const QPolygonF &polygon);
	virtual void updateHandlePos(const QPolygonF &polygon);

	const QPolygonF &initPolygon() const { return m_initPolygon; }

private:
	QPolygonF m_initPolygon;
};

class REPORT_EXPORT TransformTracker : public RectTracker
{
public:
	TransformTracker(ReportItem *item, bool effectTransform = false);
	TransformTracker(ReportItem *item, const QRectF &rect, bool effectTransform = false);

	void setEffectTransform(bool b);

protected:
	virtual void begin(int id);
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);
	virtual void end(int id, const QPointF &pos, const QPointF &oldPos);

	virtual HandleList createHandle(const QPolygonF &polygon);
	virtual void updateHandlePos(const QPolygonF &polygon);

private:
	QRectF m_originRect;
	QPointF m_initPos;
	QPointF m_initThisPos;
	QTransform m_initTransform;
	bool m_effectTransform;
	bool m_inUpdate;
};

class REPORT_EXPORT TextTracker : public Tracker
{
public:
	TextTracker(ReportTextItem *item);

protected:
	virtual void begin(int id);
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);
	virtual void end(int id, const QPointF &pos, const QPointF &oldPos);

	virtual HandleList createHandle(const QPolygonF &polygon);
	virtual void updateHandlePos(const QPolygonF &polygon = QPolygonF());

private:
	ReportTextItem *m_item;
	QPointF m_initPos;
	qreal m_initWidth;
};

class REPORT_EXPORT TableTracker : public RectTracker
{
public:
	TableTracker(ReportTableItem *item);

protected:
	virtual void begin(int id);
	virtual void move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers);

	virtual HandleList createHandle(const QPolygonF &polygon);
	virtual void updateHandlePos(const QPolygonF &polygon = QPolygonF());

private:
	ReportTableItem *m_item;
	QPointF m_initPos;
	QPointF m_initThisPos;
	bool m_inUpdate;
	int m_index;
};

///////////////////////////////////////////////////////////////////////////////
// TrackerHandleT

template<class ShapeClass, class HandleClass>
TrackerHandleT<ShapeClass, HandleClass>::TrackerHandleT(int id, Tracker *tracker)
	: ShapeClass(tracker)
	, HandleClass(id, tracker)
{
	ShapeClass::setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIgnoresTransformations);
	ShapeClass::setPen(QPen(Qt::black));
	ShapeClass::setBrush(Qt::green);
}

template<class ShapeClass, class HandleClass>
void TrackerHandleT<ShapeClass, HandleClass>::setHandleSize(int size)
{
	qreal xy = size / 2.f;
	ShapeClass::setRect(-xy, -xy, size, size);
}

template<class ShapeClass, class HandleClass>
void TrackerHandleT<ShapeClass, HandleClass>::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;

	HandleClass::begin();
}

template<class ShapeClass, class HandleClass>
void TrackerHandleT<ShapeClass, HandleClass>::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
	if (!(e->buttons() & Qt::LeftButton))
		return;

	// Find the active view.
	QGraphicsView *view = 0;
	if (e->widget())
		view = qobject_cast<QGraphicsView *>(e->widget()->parentWidget());

#if 1
	// Root items that ignore transformations need to
	// calculate their diff by mapping viewport coordinates
	// directly to parent coordinates.
	QTransform viewToParentTransform = (ShapeClass::transform().translate(ShapeClass::pos().x(), ShapeClass::pos().y())
		* ShapeClass::sceneTransform() * view->viewportTransform()).inverted();
	QPointF currentParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(e->screenPos())));
	QPointF buttonDownParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(e->buttonDownScreenPos(Qt::LeftButton))));
#else
	QPointF currentParentPos = ShapeClass::mapToParent(mapFromScene(e->scenePos()));
	QPointF buttonDownParentPos = ShapeClass::mapToParent(mapFromScene(e->buttonDownScenePos(Qt::LeftButton)));
#endif

	QPointF currentPos = HandleClass::initPos() + currentParentPos - buttonDownParentPos;
	HandleClass::move(currentPos, e->modifiers());
}

template<class ShapeClass, class HandleClass>
void TrackerHandleT<ShapeClass, HandleClass>::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;

	HandleClass::end();
}
