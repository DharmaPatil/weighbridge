#include "command.h"
#include "report.h"
#include "reportitem.h"

///////////////////////////////////////////////////////////////////////////////
// NewCommand

NewCommand::NewCommand(Report *report, ReportItem *item, QUndoCommand *parent)
	: QUndoCommand(parent)
	, m_item(item)
	, m_report(report)
{
}

NewCommand::~NewCommand()
{
	if (m_report && m_item)
		delete m_item;
}

void NewCommand::undo()
{
	Q_ASSERT(!m_report);
	m_report = m_item->report();
	m_report->removeItem(m_item->graphicsItem());
}

void NewCommand::redo()
{
	Q_ASSERT(m_report);
	m_report->addItem(m_item->graphicsItem());
	m_report = 0;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteCommand

DeleteCommand::DeleteCommand(ReportItem *item, QUndoCommand *parent)
	: QUndoCommand(parent)
	, m_item(item)
	, m_report(0)
{
}

DeleteCommand::~DeleteCommand()
{
	if (m_report && m_item)
		delete m_item;
}

void DeleteCommand::undo()
{
	Q_ASSERT(m_report);
	m_report->addItem(m_item->graphicsItem());
	m_report = 0;
}

void DeleteCommand::redo()
{
	Q_ASSERT(!m_report);
	m_report = m_item->report();
	m_report->removeItem(m_item->graphicsItem());
}

///////////////////////////////////////////////////////////////////////////////
// ToggleCommand

ToggleCommand::ToggleCommand(QUndoCommand *parent)
	: QUndoCommand(parent)
	, m_inNewState(true)
{
}

void ToggleCommand::undo()
{
	if (m_inNewState)
	{
		toggle();
		m_inNewState = !m_inNewState;
	}
}

void ToggleCommand::redo()
{
	if (!m_inNewState)
	{
		toggle();
		m_inNewState = !m_inNewState;
	}
}

///////////////////////////////////////////////////////////////////////////////
// TransformCommand

TransformCommand::TransformCommand(ReportItem *item, QUndoCommand *parent)
	: ToggleCommand(parent)
	, m_item(item)
{
	m_transform = m_item->graphicsItem()->transform();
	m_pos = m_item->graphicsItem()->pos();
	m_polygon = m_item->geometry();
}

TransformCommand::~TransformCommand()
{
}

void TransformCommand::toggle()
{
	bool selected = m_item->graphicsItem()->isSelected();
	QTransform transform = m_item->graphicsItem()->transform();
	QPointF pos = m_item->graphicsItem()->pos();
	QPolygonF polygon = m_item->geometry();

	m_item->graphicsItem()->setTransform(m_transform);
	m_item->graphicsItem()->setPos(m_pos);
	m_item->updateGeometry(m_polygon);
	m_item->graphicsItem()->setSelected(false);
	m_item->graphicsItem()->setSelected(selected);

	m_transform = transform;
	m_pos = pos;
	m_polygon = polygon;
}

bool TransformCommand::mergeWith(const QUndoCommand *other)
{
	if (other->id() != Type)
		return false;

	if (static_cast<const TransformCommand*>(other)->m_item != m_item)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// PosCommand

PosCommand::PosCommand(ReportItem *item, QUndoCommand *parent)
	: ToggleCommand(parent)
	, m_item(item)
{
	m_pos = m_item->graphicsItem()->pos();
}

void PosCommand::toggle()
{
	bool selected = m_item->graphicsItem()->isSelected();
	QPointF pos = m_item->graphicsItem()->pos();

	m_item->graphicsItem()->setPos(m_pos);
//	m_item->graphicsItem()->setSelected(false);
//	m_item->graphicsItem()->setSelected(selected);

	m_pos = pos;
}

bool PosCommand::mergeWith(const QUndoCommand *other)
{
	if (other->id() != Type)
		return false;

	if (static_cast<const PosCommand*>(other)->m_item != m_item)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// SelectionPosCommand

SelectionPosCommand::SelectionPosCommand(const QList<QGraphicsItem*> &selection, QUndoCommand *parent)
	: ToggleCommand(parent)
	, m_item(selection)
{
	foreach(QGraphicsItem *i, m_item)
		m_pos[i] = i->pos();
}

void SelectionPosCommand::toggle()
{
	foreach(QGraphicsItem *i, m_item)
	{
		QPointF pos;
		pos = i->pos();
		i->setPos(m_pos.value(i));
		m_pos[i] = pos;
	}
}

bool SelectionPosCommand::mergeWith(const QUndoCommand *other)
{
	if (other->id() != Type)
		return false;

	if (static_cast<const SelectionPosCommand*>(other)->m_item != m_item)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// SetupPageCommand

SetupPageCommand::SetupPageCommand(Report *report, 
								   QPrinter::PaperSize size, 
								   QPrinter::Orientation orientation, 
								   const QSizeF &sizef, 
								   QUndoCommand *parent)
	: ToggleCommand(parent)
	, m_report(report)
	, m_paperSize(size)
	, m_orientation(orientation)
	, m_sizef(sizef)
{
	if (m_orientation == QPrinter::Landscape && m_paperSize == QPrinter::Custom)
		m_sizef.transpose();

	m_inNewState = false;
}

void SetupPageCommand::toggle()
{
	QPrinter::PaperSize paperSize = m_report->paperSize();
	QPrinter::Orientation orientation = m_report->orientation();
	QSizeF sizef = m_report->paperSizeF();

	m_report->setPaperSize(m_paperSize);
	m_report->setOrientation(m_orientation);
	if (m_paperSize == QPrinter::Custom)
		m_report->setPaperSizeF(m_sizef);

	m_paperSize = paperSize;
	m_orientation = orientation;
	m_sizef = sizef;
}

bool SetupPageCommand::mergeWith(const QUndoCommand *other)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// FontFamilyCommand

FontFamilyCommand::FontFamilyCommand(ReportItem *item, const QString &fontFamily, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_fontFamily(fontFamily)
{
	m_inNewState = false;
}

void FontFamilyCommand::toggle()
{
	QFont font = m_item->font();
	const QString fontFamily = font.family();
	font.setFamily(m_fontFamily);
	m_fontFamily = fontFamily;
	m_item->setFont(font);
}

///////////////////////////////////////////////////////////////////////////////
// FontSizeCommand

FontSizeCommand::FontSizeCommand(ReportItem *item, int pixelSize, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_pixelSize(pixelSize)
{
	m_inNewState = false;
}

void FontSizeCommand::toggle()
{
	QFont font = m_item->font();
	const int pixelSize = font.pixelSize();
	font.setPixelSize(m_pixelSize);
	m_pixelSize = pixelSize;
	m_item->setFont(font);
}

///////////////////////////////////////////////////////////////////////////////
// FontBoldCommand

FontBoldCommand::FontBoldCommand(ReportItem *item, bool bold, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_bold(bold)
{
	m_inNewState = false;
}

void FontBoldCommand::toggle()
{
	QFont font = m_item->font();
	const bool bold = font.bold();
	font.setBold(m_bold);
	m_bold = bold;
	m_item->setFont(font);
}

///////////////////////////////////////////////////////////////////////////////
// FontItalicCommand

FontItalicCommand::FontItalicCommand(ReportItem *item, bool italic, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_italic(italic)
{
	m_inNewState = false;
}

void FontItalicCommand::toggle()
{
	QFont font = m_item->font();
	const bool italic = font.italic();
	font.setItalic(m_italic);
	m_italic = italic;
	m_item->setFont(font);
}

///////////////////////////////////////////////////////////////////////////////
// FontUnderlineCommand

FontUnderlineCommand::FontUnderlineCommand(ReportItem *item, bool underline, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_underline(underline)
{
	m_inNewState = false;
}

void FontUnderlineCommand::toggle()
{
	QFont font = m_item->font();
	const bool underline = font.underline();
	font.setUnderline(m_underline);
	m_underline = underline;
	m_item->setFont(font);
}

///////////////////////////////////////////////////////////////////////////////
// TextAlignmentCommand

TextAlignmentCommand::TextAlignmentCommand(ReportItem *item, Qt::Alignment alignment, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_alignment(alignment)
{
	m_inNewState = false;
}

void TextAlignmentCommand::toggle()
{
	Qt::Alignment alignment = m_item->alignment();
	m_item->setAlignment(m_alignment);
	m_alignment = alignment;
}

///////////////////////////////////////////////////////////////////////////////
// PenCommand

PenCommand::PenCommand(ReportItem *item, const QPen &pen, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_pen(pen)
{
	m_inNewState = false;
}

void PenCommand::toggle()
{
	QPen pen = m_item->pen();
	m_item->setPen(m_pen);
	m_pen = pen;
}

///////////////////////////////////////////////////////////////////////////////
// BrushCommand

BrushCommand::BrushCommand(ReportItem *item, const QBrush &brush, QUndoCommand *parent)
	: ToggleCommand(parent), m_item(item), m_brush(brush)
{
	m_inNewState = false;
}

void BrushCommand::toggle()
{
	QBrush brush = m_item->brush();
	m_item->setBrush(m_brush);
	m_brush = brush;
}
