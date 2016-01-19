#include "workbench.h"
#include "actions.h"
#include "application.h"
#include <QInputDialog>

Workbench *Workbench::self = 0;

Workbench::Workbench(QObject *parent)
	: QObject(parent)
	, m_pen(Qt::black, 0.5)
{
	self = this;
	m_actions = new Actions(this);

	connect(m_actions->penWidthGroup, SIGNAL(triggered(QAction*)), 
		this, SLOT(onPenWidth(QAction*)));
	connect(m_actions->penWidthCustom, SIGNAL(triggered()),
		this, SLOT(onPenWidthCustom()));

	m_actions->penNull->setVisible(false);
	m_actions->penWidthGroup->actions()[4]->trigger();
}

Workbench::~Workbench()
{
	self = 0;
}

void Workbench::setPenWidth(qreal width)
{
	m_pen.setWidthF(width * 96 / 72);
	emit penChanged(m_pen);
}

void Workbench::setPen(const QPen &pen)
{
	m_pen = pen;
	emit penChanged(m_pen);
}

void Workbench::setBrush(const QBrush &brush)
{
	m_brush = brush;
	emit brushChanged(m_brush);
}

void Workbench::setFont(const QFont &font)
{
	m_font = font;
	emit fontChanged(font);
}

void Workbench::onPenWidth(QAction *act)
{
	if (act == m_actions->penNull)
		setPen(QPen());
	else
		setPenWidth(act->data().toDouble());
}

void Workbench::onPenWidthCustom()
{
	bool ok;
	qreal width = QInputDialog::getDouble(QApplication::activeWindow(), 
		tr("输入"), tr("请输入线宽(pt)"), m_pen.widthF() * 72 / 96, 0, 10, 1, &ok);
	if (ok)
		setPenWidth(width);
}
