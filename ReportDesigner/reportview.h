#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <QGraphicsView>

class ReportView : public QGraphicsView
{
	Q_OBJECT

public:
	ReportView(QWidget *parent = 0);
	ReportView(QGraphicsScene *scene, QWidget *parent = 0);
	~ReportView();

	void scaleView(qreal factor);

protected:
	virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
	void init();
};

Q_DECLARE_METATYPE(QGraphicsView *)

#endif // REPORTVIEW_H
