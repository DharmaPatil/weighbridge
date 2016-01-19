#pragma once

#include "report_global.h"
#include <QUndoCommand>
#include <QGraphicsItem>
#include <QMap>
#include <QPrinter>
#include <QPen>
#include <QBrush>

class ReportItem;
class Report;
class QGraphicsItem;

class REPORT_EXPORT NewCommand : public QUndoCommand
{
public:
	NewCommand(Report *report, ReportItem *item, QUndoCommand *parent = 0);
	~NewCommand();

	void undo();
	void redo();

	enum { Type = 1 };
	virtual int id() const { return Type; }

private:
	ReportItem *m_item;
	Report *m_report;
};

class REPORT_EXPORT DeleteCommand : public QUndoCommand
{
public:
	DeleteCommand(ReportItem *item, QUndoCommand *parent = 0);
	~DeleteCommand();

	void undo();
	void redo();

	enum { Type = 2 };
	virtual int id() const { return Type; }

private:
	ReportItem *m_item;
	Report *m_report;
};

class REPORT_EXPORT ToggleCommand : public QUndoCommand
{
public:
	explicit ToggleCommand(QUndoCommand *parent = 0);

	void undo();
	void redo();

protected:
	virtual void toggle() = 0;

	bool m_inNewState;
};

class REPORT_EXPORT TransformCommand : public ToggleCommand
{
public:
	TransformCommand(ReportItem *item, QUndoCommand *parent = 0);
	~TransformCommand();

	enum { Type = 3 };
	virtual int id() const { return Type; }
	virtual bool mergeWith(const QUndoCommand *other);

private:
	void toggle();

	ReportItem *m_item;
	QTransform m_transform;
	QPolygonF m_polygon;
	QPointF m_pos;
};

class REPORT_EXPORT PosCommand : public ToggleCommand
{
public:
	PosCommand(ReportItem *item, QUndoCommand *parent = 0);

	enum { Type = 4 };
	virtual int id() const { return Type; }
	virtual bool mergeWith(const QUndoCommand *other);

private:
	void toggle();

	ReportItem *m_item;
	QPointF m_pos;
};

class REPORT_EXPORT SelectionPosCommand : public ToggleCommand
{
public:
	SelectionPosCommand(const QList<QGraphicsItem*> &selection, QUndoCommand *parent = 0);

	enum { Type = 5 };
	virtual int id() const { return Type; }
	virtual bool mergeWith(const QUndoCommand *other);

private:
	void toggle();

	QList<QGraphicsItem*> m_item;
	QMap<QGraphicsItem*, QPointF> m_pos;
};

class REPORT_EXPORT SetupPageCommand : public ToggleCommand
{
public:
	SetupPageCommand(Report *report, 
		QPrinter::PaperSize size, QPrinter::Orientation orientation, 
		const QSizeF &sizef, QUndoCommand *parent = 0);

	enum { Type = 6 };
	virtual int id() const { return Type; }
	virtual bool mergeWith(const QUndoCommand *other);

private:
	void toggle();

	Report *m_report;
	QPrinter::PaperSize m_paperSize;
	QPrinter::Orientation m_orientation;
	QSizeF m_sizef;
};

class REPORT_EXPORT FontFamilyCommand : public ToggleCommand
{
public:
	FontFamilyCommand(ReportItem *item, const QString &fontFamily, QUndoCommand *parent = 0);

	enum { Type = 7 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	QString m_fontFamily;
};

class REPORT_EXPORT FontSizeCommand : public ToggleCommand
{
public:
	FontSizeCommand(ReportItem *item, int pixelSize, QUndoCommand *parent = 0);

	enum { Type = 8 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	int m_pixelSize;
};

class REPORT_EXPORT FontBoldCommand : public ToggleCommand
{
public:
	FontBoldCommand(ReportItem *item, bool bold, QUndoCommand *parent = 0);

	enum { Type = 8 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	bool m_bold;
};

class REPORT_EXPORT FontItalicCommand : public ToggleCommand
{
public:
	FontItalicCommand(ReportItem *item, bool italic, QUndoCommand *parent = 0);

	enum { Type = 9 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	bool m_italic;
};

class REPORT_EXPORT FontUnderlineCommand : public ToggleCommand
{
public:
	FontUnderlineCommand(ReportItem *item, bool underline, QUndoCommand *parent = 0);

	enum { Type = 10 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	bool m_underline;
};

class REPORT_EXPORT TextAlignmentCommand : public ToggleCommand
{
public:
	TextAlignmentCommand(ReportItem *item, Qt::Alignment alignment, QUndoCommand *parent = 0);

	enum { Type = 11 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	Qt::Alignment m_alignment;
};

class REPORT_EXPORT PenCommand : public ToggleCommand
{
public:
	PenCommand(ReportItem *item, const QPen &pen, QUndoCommand *parent = 0);

	enum { Type = 12 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	QPen m_pen;
};

class REPORT_EXPORT BrushCommand : public ToggleCommand
{
public:
	BrushCommand(ReportItem *item, const QBrush &brush, QUndoCommand *parent = 0);

	enum { Type = 12 };
	virtual int id() const { return Type; }

private:
	void toggle();

	ReportItem *m_item;
	QBrush m_brush;
};
