#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QObject>
#include <QPen>
#include <QBrush>
#include <QFont>

class Actions;
class QAction;

#define qWorkbench (Workbench::instance())

class Workbench : public QObject
{
	Q_OBJECT

public:
	explicit Workbench(QObject *parent = 0);
	~Workbench();

	static Workbench *instance() { return self; }

	Actions *actions() const { return m_actions; }

	QPen pen() const { return m_pen; }
	void setPen(const QPen &pen);
	void setPenWidth(qreal width);

	QBrush brush() const { return m_brush; }
	void setBrush(const QBrush &brush);

	QFont font() const { return m_font; }
	void setFont(const QFont &font);

public slots:
	void onPenWidth(QAction *act);
	void onPenWidthCustom();

signals:
	void penChanged(const QPen &);
	void brushChanged(const QBrush &);
	void fontChanged(const QFont &);

private:
	static Workbench *self;
	Actions *m_actions;
	QPen m_pen;
	QBrush m_brush;
	QFont m_font;
};

#endif // WORKBENCH_H
