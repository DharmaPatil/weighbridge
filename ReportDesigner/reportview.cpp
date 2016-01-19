#include "reportview.h"
#include "report.h"
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QPixmap>

ReportView::ReportView(QWidget *parent)
	: QGraphicsView(parent)
{
	init();
}

ReportView::ReportView(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent)
{
	init();
}

void ReportView::init()
{
	setBackgroundBrush(Qt::lightGray);
	//setRenderHints(QPainter::Antialiasing | 
	//	QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	scaleView(1);
}

ReportView::~ReportView()
{
}

void ReportView::drawBackground(QPainter *painter, const QRectF &rect)
{
	QGraphicsView::drawBackground(painter, rect);

	if (!scene())
		return;

	// Shadow
	QRectF sceneRect = this->scene()->sceneRect();
	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		painter->fillRect(rightShadow, Qt::darkGray);
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		painter->fillRect(bottomShadow, Qt::darkGray);

	// Fill
	Report *report = (Report*)scene();
	QPoint size = mapFromScene(report->gridSize().width(), report->gridSize().height())
		- mapFromScene(QPointF());
	QPixmap pixmap(size.x(), size.y());
	{
		pixmap.fill();
		QPainter p(&pixmap);
		QPen pen(Qt::DotLine);
		pen.setColor(Qt::gray);
		p.setPen(pen);
		p.drawLine(0, 0, size.x(), 0);
		p.drawLine(0, 0, 0, size.y());
	}
	painter->setBrush(pixmap);
	painter->drawRect(sceneRect);
}

void ReportView::scaleView(qreal factor)
{
	qreal f = matrix().scale(factor, factor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (f < 0.07 || f > 100)
		return;

	Report *report = (Report*)scene();
	int dpiX = report ? report->dpiX() : 96;
	int dpiY = report ? report->dpiY() : 96;

	QTransform oldTransform = transform();
	resetTransform();
	translate(oldTransform.dx(), oldTransform.dy());
	scale(factor * physicalDpiX() / dpiX, factor * physicalDpiY() / dpiY);;
}
