#include "reportitemcreator.h"
#include "report.h"
#include "reportitem.h"
#include "command.h"
#include <QList>
#include <QGraphicsSceneEvent>
#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <QLineF>
#include <QGraphicsView>

///////////////////////////////////////////////////////////////////////////////
// PickMachine

class PickMachine
{
protected:
	PickMachine() : m_state(0) {}

public:
	enum Command {
		Begin,
		Append,
		Move,
		End,
	};
	typedef QList<Command> CommandList;

	virtual ~PickMachine() {}

	virtual CommandList transition(const QEvent *e) = 0;
	void reset() { setState(0); }

	int state() const { return m_state; }
	void setState(int state) { m_state = state; }

	static PickMachine *createMachine(ReportItemCreator::SelectionMode mode);

private:
	int m_state;
};

///////////////////////////////////////////////////////////////////////////////
// ClickPointMachine

class ClickPointMachine : public PickMachine
{
public:
	virtual CommandList transition(const QEvent *e)
	{
		CommandList c;

		switch(e->type())
		{
		case QEvent::GraphicsSceneMousePress:
			if (static_cast<const QGraphicsSceneMouseEvent *>(e)->button() == Qt::LeftButton)
				c << Begin << Append << End;
			break;

		default:
			break;
		}
		return c;
	}
};

///////////////////////////////////////////////////////////////////////////////
// DragRectMachine

class DragRectMachine : public PickMachine
{
public:
	virtual CommandList transition(const QEvent *e)
	{
		CommandList c;

		switch(e->type())
		{
		case QEvent::GraphicsSceneMousePress:
			switch(state())
			{
			case 0:
				if (((QGraphicsSceneMouseEvent*)e)->button() == Qt::LeftButton)
				{
					c << Begin << Append << Append << Move;
					setState(1);
				}
				break;
			default:
				break;
			};

		case QEvent::GraphicsSceneMouseMove:
			if (state() != 0)
				c << Move;
			break;

		case QEvent::GraphicsSceneMouseRelease:
			if (state() == 1)
			{
				c << End;
				setState(0);
			}
			break;

		default:
			break;
		}
		return c;
	}
};

///////////////////////////////////////////////////////////////////////////////

PickMachine *PickMachine::createMachine(ReportItemCreator::SelectionMode mode)
{
	switch(mode)
	{
	case ReportItemCreator::PointSelection:
		return new ClickPointMachine();
	case ReportItemCreator::RectSelection:
		return new DragRectMachine();
	default:
		Q_ASSERT(false);
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// ReportItemCreator

ReportItemCreator::ReportItemCreator(ReportItemCreator::SelectionMode mode, QObject *parent)
	: QObject(parent)
	, m_report(0)
	, m_view(0)
	, m_item(0)
	, m_selectionMode(mode)
	, m_stateMachine(PickMachine::createMachine(mode))
	, m_active(false)
{
}

ReportItemCreator::~ReportItemCreator()
{
	if (m_stateMachine)
		delete m_stateMachine;
}

void ReportItemCreator::prepare(ReportItem *item)
{
	if (reportItem())
		reject();

	item->setKey(key());
	m_item = item;
	report()->addItem(m_item->graphicsItem());

	emit created(item);
}

void ReportItemCreator::accept()
{
	if (m_item)
	{
		Q_ASSERT(m_report);
		m_report->undoStack()->push(new NewCommand(report(), m_item));
		m_report->clearSelection();
		m_item->graphicsItem()->setSelected(true);

		m_item = 0;
	}
	finish();
}

void ReportItemCreator::reject()
{
	if (m_item)
	{
		report()->removeItem(m_item->graphicsItem());
		delete m_item;
		m_item = 0;
	}
	finish();
}

void ReportItemCreator::finish()
{
	emit finished(m_item);
}

bool ReportItemCreator::eventFilter(QObject *watched, QEvent *e)
{
	if (watched != m_report || !m_report || !m_stateMachine)
		return false;

	if (!transition(e))
		return false;

	return true;
}

void ReportItemCreator::enter(QGraphicsView *view)
{
	Q_ASSERT(view);
	m_view = view;
	m_report = static_cast<Report*>(view->scene());
	m_report->installEventFilter(this);
}

void ReportItemCreator::leave()
{
	if (m_report)
		m_report->removeEventFilter(this);

	reject();
	m_report = 0;
	m_view = 0;
}

bool ReportItemCreator::accept(const QPolygonF &selection) const
{
	return true;
}

void ReportItemCreator::begin()
{
	m_selection.clear();
	m_active = true;
}

void ReportItemCreator::append(const QPointF &point) 
{
	if (isActive())
		m_selection.append(point); 
}

void ReportItemCreator::move(const QPointF &point) 
{
	if (isActive())
		m_selection.back() = point; 
}

bool ReportItemCreator::end(bool ok)
{
	if (isActive())
	{
		if (ok)
			ok = accept(selection());
		if (!ok)
			m_selection.clear();
		m_active = false;
	}

	return ok;
}

bool ReportItemCreator::transition(const QEvent *e)
{
	if (!m_stateMachine)
		return false;

	PickMachine::CommandList cmd = m_stateMachine->transition(e);
	if (cmd.isEmpty())
		return false;

	QPointF point;
	Qt::KeyboardModifiers modifiers = Qt::NoModifier;
	switch(e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	case QEvent::GraphicsSceneMouseMove:
	case QEvent::GraphicsSceneMouseRelease:
	case QEvent::GraphicsSceneMouseDoubleClick:
		{
			const QGraphicsSceneMouseEvent *me = static_cast<const QGraphicsSceneMouseEvent *>(e);
			point = me->scenePos();
			modifiers = me->modifiers();
			break;
		}
	default:
		point = view()->mapToScene(view()->mapFromGlobal(QCursor::pos()));
		break;
	}

	foreach(PickMachine::Command c, cmd)
	{
		switch(c)
		{
		case PickMachine::Begin:
			begin();
			break;
		case PickMachine::Append:
			snapGrid(point, modifiers);
			append(point);
			break;
		case PickMachine::Move:
			snapGrid(point, modifiers);
			if (selection().size() > 1)
				snapAngle(point, selection()[selection().size() - 2], modifiers);
			if (!selection().isEmpty() && selection().back() != point)
				move(point);
			break;
		case PickMachine::End:
			end();
			break;
		}
	}

	return true;
}

void ReportItemCreator::reset()
{
	if (m_stateMachine)
		m_stateMachine->reset();

	if (isActive())
		end(false);
}

void ReportItemCreator::snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers)
{
	m_report->snapGrid(point, modifiers);
}

void ReportItemCreator::snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers)
{
	m_report->snapAngle(point, prev, modifiers);
}
