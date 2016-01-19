#ifndef REPORTITEMCREATOR_H
#define REPORTITEMCREATOR_H

#include "report_global.h"
#include <QObject>
#include <QPolygonF>

class QGraphicsItem;
class QGraphicsView;
class QDataStream;
class ReportItem;
class Report;
class QIcon;
class PickMachine;
class QUndoCommand;

class REPORT_EXPORT ReportItemCreator : public QObject
{
	Q_OBJECT

public:
	enum SelectionMode {
		PointSelection,
		RectSelection,
		PolygonSelection,
	};

	ReportItemCreator(SelectionMode mode, QObject *parent = 0);
	virtual ~ReportItemCreator();

	virtual QString key() = 0;
	virtual QString name() = 0;
	virtual QString group() = 0;
	virtual QIcon icon() = 0;
	virtual ReportItem *create() = 0;

	Report *report() const { return m_report; }
	QGraphicsView *view() const { return m_view; }
	ReportItem *reportItem() const { return m_item; }

public slots:
	virtual void enter(QGraphicsView *view);
	virtual void leave();
	void prepare(ReportItem *item);
	void accept();
	void reject();
	void finish();

signals:
	void created(ReportItem *item);
	void finished(ReportItem *item);

protected:
	virtual bool eventFilter(QObject *watched, QEvent *e);

	bool transition(const QEvent *e);
	void reset();

	virtual bool accept(const QPolygonF &selection) const;
	virtual void snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers);
	virtual void snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers);

	virtual void begin();
	virtual void append(const QPointF &point);
	virtual void move(const QPointF &point);
	virtual bool end(bool ok = true);

	QPolygonF selection() const { return m_selection; }
	bool isActive() const { return m_active; }

private:
	Report *m_report;
	QGraphicsView *m_view;
	ReportItem *m_item;
	QPolygonF m_selection;
	SelectionMode m_selectionMode;
	PickMachine *m_stateMachine;
	QUndoCommand *m_command;
	bool m_active;
	bool m_trackingMouse;
};

#endif // REPORTITEMCREATOR_H
