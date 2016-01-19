#ifndef REPORT_H
#define REPORT_H

#include "report_global.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPrinter>
#include <QMap>
#include <QSqlRecord>

class ReportItem;
class ReportGroupItem;
class QDataStream;
class QUndoStack;
class Tracker;
class QSqlDatabase;

class REPORT_EXPORT Report : public QGraphicsScene
{
	Q_OBJECT

public:
	static QString suffix;
	static QString mimeType;

	explicit Report(QObject *parent = 0);
	virtual ~Report();

	void setOrientation(QPrinter::Orientation o);
	QPrinter::Orientation orientation() const;

	void setPaperSize(QPrinter::PaperSize size);
	QPrinter::PaperSize paperSize() const;

	void setPaperSizeF(const QSizeF &paperSize);
	QSizeF paperSizeF() const;

	int dpiX() const { return m_dpiX; }
	int dpiY() const { return m_dpiY; }

	void setupPrinter(QPrinter *printer) const;

	QUndoStack *undoStack() const { return m_undoStack; }

	void setDesignMode(bool b) { m_designMode = b; }
	bool isDesignMode() const { return m_designMode; }

	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

	bool save(const QString &filename);
	bool load(const QString &filename);

	void itemChange(ReportItem *reportItem, QGraphicsItem::GraphicsItemChange change, 
		const QVariant &value, QVariant &result);
	void itemContentsChange(ReportItem *reportItem);

	QSizeF gridSize() const;
	void setGridSize(const QSizeF &grid);

	void snapGrid(QPointF &point, Qt::KeyboardModifiers modifiers);
	void snapAngle(QPointF &point, const QPointF &prev, Qt::KeyboardModifiers modifiers);

	QString filename() const { return m_filename; }
	void setFileName(const QString &filename) { m_filename = filename; }

	QString errorString() const { return m_errorString; }
	void setErrorString(const QString &s) { m_errorString = s; }

	void fetchData(int patientId, QSqlDatabase db);
	int patientId() const { return m_patientId; }
	QSqlRecord record(const QString &table) const;

public slots:
	void undo();
	void redo();

protected:
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
	void updateSceneRect();

private slots:
	void onSelectionChanged();

private:
	QPrinter::PaperSize m_paperSize;
	QPrinter::Orientation m_orientation;
	QSizeF m_paperSizeF;
	int m_dpiX;
	int m_dpiY;
	QUndoStack *m_undoStack;

	bool m_designMode;
	Tracker *m_tracker;
	bool m_selectionChanged;
	QSizeF m_gridSize;
	QString m_filename;
	QString m_errorString;
	bool m_inRedoUndo;

	int m_patientId;
	QMap<QString, QSqlRecord> m_records;
};

#endif // REPORT_H
