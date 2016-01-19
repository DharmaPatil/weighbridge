#include "report.h"
#include "tracker.h"
#include "reportitem.h"
#include "command.h"
#include "profile.h"
#include "global.h"

#include <QFile>
#include <QDataStream>
#include <QGraphicsSceneEvent>
#include <QUndoStack>
#include <QDebug>
#include <QtSql>

QString Report::suffix("rpt");
QString Report::mimeType("application/x-qt-windows-mime;value=\"ReportItems\"");

Report::Report(QObject *parent)
	: QGraphicsScene(parent)
	, m_paperSize(QPrinter::A4)
	, m_orientation(QPrinter::Portrait)
	, m_dpiX(96)
	, m_dpiY(96)
	, m_undoStack(new QUndoStack(this))
	, m_designMode(false)
	, m_tracker(0)
	, m_selectionChanged(false)
	, m_gridSize(10, 10)
	, m_inRedoUndo(false)
	, m_patientId(0)
{
	updateSceneRect();
	connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

Report::~Report()
{
}

void Report::setOrientation(QPrinter::Orientation o)
{
	m_orientation = o;
	updateSceneRect();
}

QPrinter::Orientation Report::orientation() const
{
	return m_orientation;
}

void Report::setPaperSize(QPrinter::PaperSize size)
{
	m_paperSize = size;
	updateSceneRect();
}

QPrinter::PaperSize Report::paperSize() const
{
	return m_paperSize;
}

void Report::setPaperSizeF(const QSizeF &paperSize)
{
	m_paperSize = QPrinter::Custom;
	m_paperSizeF = paperSize;
	updateSceneRect();
}

QSizeF Report::paperSizeF() const
{
	return m_paperSizeF;
}

void Report::updateSceneRect()
{
	if (m_paperSize != QPrinter::Custom)
	{
		QPrinter printer;
		printer.setPaperSize(m_paperSize);
		printer.setOrientation(QPrinter::Portrait);

		m_paperSizeF = printer.paperSize(QPrinter::Millimeter);
	}

	QSizeF size = m_paperSizeF;
	if (m_orientation == QPrinter::Landscape)
		size.transpose();

	size.rwidth() *= dpiX() / 25.4;
	size.rheight() *= dpiY() / 25.4;

	setSceneRect(QRectF(QPointF(0, 0), size));
}

void Report::setupPrinter(QPrinter *printer) const
{
	printer->setPaperSize(m_paperSize);
	printer->setOrientation(m_orientation);
	if (m_paperSize == QPrinter::Custom)
	{
		QSizeF size = paperSizeF();
		if (m_orientation == QPrinter::Landscape)
			size.transpose();
		printer->setPaperSize(size, QPrinter::Millimeter);
	}
}

static const int signature = *(const int *)"nrpt";
static const quint8 report_version = 1;

bool Report::save(QDataStream &stream)
{
	try
	{
		stream << signature << report_version;

		stream << m_dpiX << m_dpiY << (int)m_paperSize << (int)m_orientation << paperSizeF();

		QList<ReportItem*> reportItems;
		foreach(QGraphicsItem *item, items())
		{
			ReportItem *ri = reportItem_cast(item);
			if (ri)
				reportItems << ri;
		}
		stream << reportItems;
	}
	catch(QString &s)
	{
		setErrorString(s);
		return false;
	}
	return true;
}

bool Report::load(QDataStream &stream)
{
	try
	{
		int sign;
		quint8 version;

		stream >> sign >> version;
		if (sign != signature)
			throw tr("错误的文件格式");
		if (version > report_version)
			throw tr("无法读取高版本的文档");

		int paperSize, orientation;
		QSizeF sizeF;
		stream >> m_dpiX >> m_dpiY >> paperSize >> orientation >> sizeF;
		m_paperSize = (QPrinter::PaperSize)paperSize;
		m_orientation = (QPrinter::Orientation)orientation;
		m_paperSizeF = sizeF;
		updateSceneRect();

		QList<ReportItem*> reportItems;
		stream >> reportItems;

		foreach(ReportItem *ri, reportItems)
			addItem(ri->graphicsItem());
	}
	catch(QString &s)
	{
		setErrorString(s);
		return false;
	}
	return true;
}

bool Report::save(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		information(file.errorString());
		return false;
	}

	QDataStream stream(&file);
	return save(stream);
}

bool Report::load(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		setErrorString(file.errorString());
		return false;
	}

	QDataStream stream(&file);
	return load(stream);
}

void Report::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	QGraphicsScene::mouseReleaseEvent(e);
}

void Report::onSelectionChanged()
{
	m_selectionChanged = true;

	if (!isDesignMode() || !m_selectionChanged)
		return;

	if (m_tracker)
	{
		delete m_tracker;
		m_tracker = 0;
	}

	if (selectedItems().isEmpty())
		return;

	ReportItem *selectedItem = 0;
	if (selectedItems().size() == 1)
		selectedItem = reportItem_cast(selectedItems().first());

	if (selectedItem)
		m_tracker = selectedItem->createTracker();

	if (selectedItem && !m_tracker)
		m_tracker = new TransformTracker(selectedItem, true);

	if (m_tracker && !m_tracker->scene())
	{
		m_tracker->setZValue(1);
		m_tracker->setPolygon(selectedItem->geometry());
		addItem(m_tracker);
		m_tracker->setPos(selectedItem->graphicsItem()->pos());
		m_tracker->setTransform(selectedItem->graphicsItem()->transform());
	}
}

void Report::itemChange(ReportItem *reportItem, QGraphicsItem::GraphicsItemChange change, 
						const QVariant &value, QVariant &result)
{
	if (!isDesignMode())
		return;

	if (change != QGraphicsItem::ItemPositionChange)
		return;

	QGraphicsItem *item = reportItem->graphicsItem();
	if (!item->isSelected())
		return;

	if (!m_inRedoUndo)
	{
		QPointF pos = result.toPointF();
		snapGrid(pos, 0);
		result = pos;
		undoStack()->push(new SelectionPosCommand(selectedItems()));
	}

	if (m_tracker)
		m_tracker->setPos(m_tracker->pos() + result.toPointF() - item->pos());
}

void Report::itemContentsChange(ReportItem *reportItem)
{
	if (!isDesignMode())
		return;

	QGraphicsItem *item = reportItem->graphicsItem();
	if (!item->isSelected())
		return;

	if (m_tracker)
		m_tracker->setPolygon(reportItem->geometry());
}

QSizeF Report::gridSize() const
{
	return m_gridSize;
}

void Report::setGridSize(const QSizeF &grid)
{
	m_gridSize = grid;
}

void Report::snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers)
{
	if (modifiers & Qt::AltModifier)
		return;

	point.setX(qRound(point.x() / m_gridSize.width()) * m_gridSize.width());
	point.setY(qRound(point.y() / m_gridSize.height()) * m_gridSize.height());
}

void Report::snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers)
{
	if (!(modifiers & Qt::ShiftModifier))
		return;

	QLineF line(prev, point);
	qreal angle = line.angle();
	angle = qRound(angle / 45) * 45;
	line.setAngle(angle);
	point = line.p2();
}

void Report::undo()
{
	m_inRedoUndo = true;
	if (undoStack()->canUndo())
		undoStack()->undo();
	m_inRedoUndo = false;
}

void Report::redo()
{
	m_inRedoUndo = true;
	if (undoStack()->canRedo())
		undoStack()->redo();
	m_inRedoUndo = false;
}

void Report::fetchData(int patientId, QSqlDatabase db)
{
	m_records.clear();
	m_patientId = patientId;

	QSqlQuery q(db);
	if (!q.exec(QString("select * from patient where id=%1").arg(patientId)))
		TRACE_LAST_ERROR(q);
	else if (q.next())
	{
		QSqlRecord r = q.record();
		//if (!q.exec(QString("select name from sex where id=%1").arg(r.value("sex").toInt())))
		//	TRACE_LAST_ERROR(q);
		//q.next();
		//r.setValue("sex", q.value(0));
		m_records["patient"] = r;
	}

	foreach(QGraphicsItem *item, items())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			reportItem->updateData();
	}
}

QSqlRecord Report::record(const QString &table) const
{
	return m_records.value(table);
}
