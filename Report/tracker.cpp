#include "tracker.h"
#include "reportitem.h"
#include "command.h"
#include <QGraphicsView>
#include <QPen>
#include <QBrush>
#include <QDebug>

///////////////////////////////////////////////////////////////////////////////
// TrackerHandle

TrackerHandle::TrackerHandle(int id, Tracker *tracker)
	: m_id(id), m_tracker(tracker)
{
}

TrackerHandle::~TrackerHandle()
{
}

void TrackerHandle::begin()
{
	m_initPos = graphicsItem()->pos();
	m_tracker->begin(m_id);
}

void TrackerHandle::move(QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	m_tracker->move(m_id, pos, modifiers);
}

void TrackerHandle::end()
{
	m_tracker->end(m_id, graphicsItem()->pos(), m_initPos);
}

///////////////////////////////////////////////////////////////////////////////
// GraphicsDiamondItem

GraphicsDiamondItem::GraphicsDiamondItem(QGraphicsItem *parent)
	: QAbstractGraphicsShapeItem(parent)
{
}

void GraphicsDiamondItem::setRect(const QRectF &rect)
{
	prepareGeometryChange();
	m_rect = rect;
}

QRectF GraphicsDiamondItem::boundingRect() const
{
	return m_rect;
}

QPolygonF GraphicsDiamondItem::polygon() const
{
	QPointF c = m_rect.center();
	QPolygonF p;
	p	<< QPointF(m_rect.left(), c.y())
		<< QPointF(c.x(), m_rect.top())
		<< QPointF(m_rect.right(), c.y())
		<< QPointF(c.x(), m_rect.bottom());
	p	<< p.front();
	return p;
}

QPainterPath GraphicsDiamondItem::shape() const
{
	QPainterPath path;
	path.addPolygon(polygon());
	return path;
}

void GraphicsDiamondItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(pen());
	painter->setBrush(brush());
	painter->drawPolygon(polygon());
}

///////////////////////////////////////////////////////////////////////////////
// Tracker

Tracker::Tracker(ReportItem *item)
	: m_item(item)
	, m_cmd(0)
	, m_posCmd(0)
	, m_recordPos(true)
{
	setFlags(ItemIsMovable);
}

Tracker::~Tracker()
{
	delete m_cmd;
	delete m_posCmd;
}

QPolygonF Tracker::polygon() const
{
	return m_polygon; 
}

void Tracker::setPolygon(const QPolygonF &polygon)
{
	m_polygon = polygon;
	if (m_handles.isEmpty())
		m_handles = createHandle(polygon);
	updateHandlePos(polygon);
}

QRectF Tracker::boundingRect() const
{
	return QRectF();
}

void Tracker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void Tracker::begin(int id)
{
	m_cmd = new TransformCommand(m_item);
}

void Tracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	snapGrid(pos, modifiers);
}

void Tracker::end(int id, const QPointF &pos, const QPointF &oldPos)
{
	m_item->report()->undoStack()->push(m_cmd);
	m_cmd = 0;
}

void Tracker::snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers)
{
	m_item->report()->snapGrid(point, modifiers);
}

void Tracker::snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers)
{
	m_item->report()->snapAngle(point, prev, modifiers);
}

QVariant Tracker::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (m_recordPos && !m_cmd && change == QGraphicsItem::ItemPositionChange)
	{
		//m_posCmd = new PosCommand(m_item);
		//m_item->report()->undoStack()->push(m_posCmd);
		//m_posCmd = 0;
	}

	return QGraphicsItem::itemChange(change, value);
}

///////////////////////////////////////////////////////////////////////////////
// LineTracker

LineTracker::LineTracker(ReportLineItem *line)
	: Tracker(line)
	, m_line(line)
{
	setLine(line->line());
}

QLineF LineTracker::line() const
{
	return QLineF(polygon().value(0), polygon().value(1));
}

void LineTracker::setLine(const QLineF &line)
{
	QPolygonF polygon;
	polygon << line.p1() << line.p2();
	setPolygon(polygon);
}

void LineTracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	Tracker::move(id, pos, modifiers);

	if (id < 2)
	{
		// snapGrid(pos, modifiers);
		snapAngle(pos, m_polygon[1-id], modifiers);

		m_polygon[id] = pos;
		updateHandlePos(m_polygon);
	}
}

void LineTracker::end(int id, const QPointF &pos, const QPointF &oldPos)
{
	Tracker::end(id, pos, oldPos);
}

LineTracker::HandleList LineTracker::createHandle(const QPolygonF &polygon)
{
	HandleList handles;
	for(int i=0; i<2; i++)
	{
		TrackerRectHandle *handle = new TrackerRectHandle(i, this);
		handle->setHandleSize(DefaultHandleSize);
		handles += handle;
	}
	return handles;
}

void LineTracker::updateHandlePos(const QPolygonF &polygon)
{
	for(int i=0; i<2; i++)
		m_handles[i]->graphicsItem()->setPos(polygon.value(i));

	if (m_line)
		m_line->setLine(line());
}

///////////////////////////////////////////////////////////////////////////////
// RectTracker

RectTracker::RectTracker(ReportItem *item)
	: Tracker(item)
{
}

RectTracker::RectTracker(ReportItem *item, const QRectF &rect)
	: Tracker(item)
{
	setRect(rect);
}

QRectF RectTracker::rect() const
{
	return QRectF(polygon().value(0), polygon().value(1));
}

void RectTracker::setRect(const QRectF &rect)
{
	QPolygonF polygon;
	polygon << rect.topLeft() << rect.bottomRight();
	m_initPolygon = polygon;
	setPolygon(polygon);
}

void RectTracker::begin(int id)
{
	Tracker::begin(id);
	m_initPolygon = m_polygon;
}

void RectTracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	Tracker::move(id, pos, modifiers);

	snapGrid(pos, modifiers);

	if (id % 2 == 0 && (modifiers & Qt::ControlModifier))
	{
		QPointF oldSize = m_initPolygon[1] - m_initPolygon[0];
		QPointF size = oldSize;

		if (id == 0 || id == 6)
			size.setX(m_initPolygon[1].x() - pos.x());
		else
			size.setX(pos.x() - m_initPolygon[0].x());

		if (id == 0 || id == 2)
			size.setY(m_initPolygon[1].y() - pos.y());
		else
			size.setY(pos.y() - m_initPolygon[0].y());

		qreal xratio = size.x() / oldSize.x();
		qreal yratio = size.y() / oldSize.y();

		if (xratio > yratio)
		{
			yratio = xratio;
			if (id == 0 || id == 2)
				pos.setY(m_initPolygon[1].y() - oldSize.y() * yratio);
			else
				pos.setY(m_initPolygon[0].y() + oldSize.y() * yratio);
		}
		else if (yratio > xratio)
		{
			xratio = yratio;
			if (id == 0 || id == 6)
				pos.setX(m_initPolygon[1].x() - oldSize.x() * xratio);
			else
				pos.setX(m_initPolygon[0].x() + oldSize.x() * xratio);
		}
	}

	switch(id)
	{
	case 0:
		m_polygon[0] = pos;
		break;
	case 1:
		m_polygon[0].setY(pos.y());
		break;
	case 2:
		m_polygon[0].setY(pos.y());
		m_polygon[1].setX(pos.x());
		break;
	case 3:
		m_polygon[1].setX(pos.x());
		break;
	case 4:
		m_polygon[1] = pos;
		break;
	case 5:
		m_polygon[1].setY(pos.y());
		break;
	case 6:
		m_polygon[0].setX(pos.x());
		m_polygon[1].setY(pos.y());
		break;
	case 7:
		m_polygon[0].setX(pos.x());
		break;
	default:
		return;
	}

	updateHandlePos(m_polygon);
}

void RectTracker::end(int id, const QPointF &pos, const QPointF &oldPos)
{
	Tracker::end(id, pos, oldPos);
}

RectTracker::HandleList RectTracker::createHandle(const QPolygonF &polygon)
{
	HandleList handles;
	for(int i=0; i<8; i++)
	{
		TrackerRectHandle *handle = new TrackerRectHandle(i, this);
		handle->setHandleSize(DefaultHandleSize);
		handles += handle;
	}
	return handles;
}

/*

矩形的8个点顺序

p0     p1      p2
   +----+----+
   |         |
p7 +         + p3
   |         |
   +----+----+
p6     p5      p4

*/
void RectTracker::updateHandlePos(const QPolygonF &polygon)
{
	QPointF p1 = polygon.value(0), p2 = polygon.value(1);
	const qreal x[] = { p1.x(), (p1.x() + p2.x()) / 2, p2.x() };
	const qreal y[] = { p1.y(), (p1.y() + p2.y()) / 2, p2.y() };
	const int ix[] = { 0, 1, 2, 2, 2, 1, 0, 0 };
	const int iy[] = { 0, 0, 0, 1, 2, 2, 2, 1 };

	for(int i=0; i<8; i++)
		m_handles[i]->graphicsItem()->setPos(x[ix[i]], y[iy[i]]);
}

///////////////////////////////////////////////////////////////////////////////
// TransformTracker

TransformTracker::TransformTracker(ReportItem *item, bool effectTransform)
	: RectTracker(item)
	, m_effectTransform(effectTransform)
	, m_inUpdate(false)
{
	m_originRect = item->graphicsItem()->boundingRect();
	m_initThisPos = pos();
	m_initPos = item->graphicsItem()->pos();
	m_initTransform = item->graphicsItem()->transform();
	setRect(item->graphicsItem()->boundingRect());
}

TransformTracker::TransformTracker(ReportItem *item, const QRectF &rect, bool effectTransform)
	: RectTracker(item)
	, m_effectTransform(effectTransform)
	, m_inUpdate(false)
{
	m_originRect = rect;
	m_initThisPos = pos();
	m_initPos = item->graphicsItem()->pos();
	m_initTransform = item->graphicsItem()->transform();
	setRect(rect);
}

void TransformTracker::begin(int id)
{
	RectTracker::begin(id);
	m_originRect = rect();
	m_initThisPos = pos();
	m_initPos = m_item->graphicsItem()->pos();
	m_initTransform = m_item->graphicsItem()->transform();
}

void TransformTracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	RectTracker::move(id, pos, modifiers);
}

void TransformTracker::end(int id, const QPointF &pos, const QPointF &oldPos)
{
	RectTracker::end(id, pos, oldPos);
}

Tracker::HandleList TransformTracker::createHandle(const QPolygonF &polygon)
{
	HandleList handles = RectTracker::createHandle(polygon);
	return handles;
}

void TransformTracker::updateHandlePos(const QPolygonF &polygon)
{
	if (m_inUpdate)
		return;

	m_inUpdate = true;
	if (m_effectTransform)
	{
		if (m_item)
		{
			QPointF size = polygon[1] - polygon[0];
			QPointF parentTopLeft = mapToParent(polygon[0]);
			QPointF originParentTopLeft = mapToParent(initPolygon()[0]);
			QPointF offset = parentTopLeft - originParentTopLeft;
			QPointF p = m_initPos + offset;
			QTransform t = m_initTransform;
			t.scale(size.x()/m_originRect.width(), size.y()/m_originRect.height());
			QPointF oldPos = pos();
			m_item->graphicsItem()->setPos(p);
			m_item->graphicsItem()->setTransform(t);
			setPos(oldPos);
		}
		RectTracker::updateHandlePos(polygon);
	}
	else
	{
		RectTracker::updateHandlePos(polygon);
		if (m_item)
			m_item->updateGeometry(polygon);
	}
	m_inUpdate = false;
}

///////////////////////////////////////////////////////////////////////////////
// TextTracker

/*

调整文本的的3个点顺序

             
   +----+----+
   |         |
p0 +         + p1
   |         |
   +----+----+
               p2

*/

TextTracker::TextTracker(ReportTextItem *item)
	: Tracker(item)
	, m_item(item)
{
	m_handles = createHandle(QPolygonF());
	updateHandlePos(QPolygonF());
}

void TextTracker::begin(int id)
{
	Tracker::begin(id);

	if (id == 0)
	{
		m_initPos = pos();
		m_initWidth = m_item->boundingRect().width();
	}
}

void TextTracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	if (id == 2)
		return;

	Tracker::move(id, pos, modifiers);

	switch(id)
	{
	case 0:
		{
			m_item->setPos(m_initPos.x() + pos.x(), m_initPos.y());
			m_item->setTextWidth(m_initWidth - pos.x());
			m_handles[2]->graphicsItem()->show();
		}
		break;
	case 1:
		m_item->setTextWidth(pos.x());
		m_handles[2]->graphicsItem()->show();
		break;
	default:
		break;
	}

	updateHandlePos();
}

void TextTracker::end(int id, const QPointF &pos, const QPointF &oldPos)
{
	if (id == 2)
	{
		m_item->setTextWidth(-1);
		m_handles[2]->graphicsItem()->hide();
		updateHandlePos();
	}
	Tracker::end(id, pos, oldPos);
}

Tracker::HandleList TextTracker::createHandle(const QPolygonF &polygon)
{
	HandleList handles;
	for (int i=0; i<2; i++)
		handles += new TrackerRectHandle(i, this);
	handles += new TrackerEllipseHandle(2, this);
	handles.back()->graphicsItem()->setToolTip(Report::tr("自动调整大小"));
	foreach(TrackerHandle *handle, handles)
		handle->setHandleSize(DefaultHandleSize);
	static_cast<TrackerEllipseHandle*>(handles.back())->setBrush(Qt::yellow);
	return handles;
}

void TextTracker::updateHandlePos(const QPolygonF &polygon)
{
	if (m_item)
	{
		QRectF r = m_item->boundingRect();
		qreal y = r.center().y();
		m_handles[0]->graphicsItem()->setPos(
			r.left(), y);
		m_handles[1]->graphicsItem()->setPos(
			r.right(), y);
		m_handles[2]->graphicsItem()->setPos(r.bottomRight());
		m_handles[2]->graphicsItem()->setVisible(m_item->textWidth() != -1);
	}
}

///////////////////////////////////////////////////////////////////////////////
// TableTracker

TableTracker::TableTracker(ReportTableItem *item)
	: RectTracker(item)
	, m_item(item)
	, m_inUpdate(false)
	, m_index(8)
{
	m_initThisPos = pos();
	m_initPos = item->graphicsItem()->pos();
}

void TableTracker::begin(int id)
{
	RectTracker::begin(id);
	m_initThisPos = pos();
	m_initPos = m_item->graphicsItem()->pos();
}

void TableTracker::move(int id, QPointF &pos, Qt::KeyboardModifiers modifiers)
{
	RectTracker::move(id, pos, modifiers);

	if (id < m_index)
	{
		QPointF offset(	m_polygon[0].x() - m_polygon[2].x(),
						m_polygon[0].y() - m_polygon[3].y());
		for(int i=2; i<m_polygon.size(); i++)
			m_polygon[i] += offset;
	}
	else
	{
		int index = id - m_index + 2;
		if (id == m_index)
		{
			if (pos.y() < m_polygon[0].y())
				pos.setY(m_polygon[0].y());

			m_polygon[index].setY(pos.y());
			pos.setX(m_polygon[index].x());
		}
		else
		{
			if (pos.x() < m_polygon[index-1].x())
				pos.setX(m_polygon[index-1].x());

			qreal dx = pos.x() - m_polygon[index].x();
			m_polygon[index].setX(pos.x());
			pos.setY(m_polygon[index].y());

			for(int i=index+1; i<m_polygon.size(); i++)
				m_polygon[i].rx() += dx;
		}
	}

	updateHandlePos(m_polygon);
}

Tracker::HandleList TableTracker::createHandle(const QPolygonF &polygon)
{
	Q_ASSERT(polygon.size() == 3 + m_item->columnCount());
	HandleList handles = RectTracker::createHandle(polygon);
	m_index = handles.size();

	for(int i=2; i<polygon.size(); i++)
	{
		TrackerDiamondHandle *diamond = new TrackerDiamondHandle(handles.size(), this);
		diamond->setBrush(Qt::yellow);
		diamond->setHandleSize(DefaultHandleSize);
		handles << diamond;
	}
	return handles;
}

void TableTracker::updateHandlePos(const QPolygonF &polygon)
{
	if (m_inUpdate)
		return;

	m_inUpdate = true;

	if (m_item && !initPolygon().isEmpty())
	{
		QPointF size = polygon[1] - polygon[0];
		QPointF parentTopLeft = mapToParent(polygon[0]);
		QPointF originParentTopLeft = mapToParent(initPolygon()[0]);
		QPointF offset = parentTopLeft - originParentTopLeft;
		QPointF p = m_initPos + offset;
		QPointF oldPos = pos();
		m_item->graphicsItem()->setPos(p);
		setPos(oldPos);
	}
	RectTracker::updateHandlePos(polygon);

	for(int i=2; i<polygon.size() && m_index+i-2 < m_handles.size(); i++)
		m_handles[m_index+i-2]->graphicsItem()->setPos(polygon[i]);

	if (m_item)
		m_item->updateGeometry(polygon);

	m_inUpdate = false;
}
