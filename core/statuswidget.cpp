/**
* @file statuswidget.cpp
* @brief 传感器状态波形显示类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#include <QtDebug>
#include <QtCore>
#include <QLabel>
#include <QPainter>
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_plot_zoomer.h>

#include "workbench.h"
#include "instrument.h"
#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(Workbench *wb, QWidget *parent)
    : QWidget(parent)
    , m_workbench(wb)
    , ui(new Ui::StatusWidgetClass)
    , m_offset(0)
{
    ui->setupUi(this);
    ui->qwtPlot->setAutoReplot(false);
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "ms");
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, MaxCount);

    connect(m_workbench->instrument(), SIGNAL(updateRefPoint(QVector<qint16>&)),
            this, SLOT(setSensorRefPoint(QVector<qint16>&)));
    connect(m_workbench->instrument(), SIGNAL(updateSamplePoint(qint16&)),
            this, SLOT(plotWaveCurve(qint16&)));
}

StatusWidget::~StatusWidget()
{

}

void StatusWidget::setTitleString(QString &title)
{
	m_title = title;
}

void StatusWidget::setCurveData(qint16 &data)
{
	data = data;
}

void StatusWidget::plotWaveCurve(qint16 &data)
{
	data = data;
}

void StatusWidget::setSensorRefPoint(QVector<qint16> &ref)
{
	Q_UNUSED(ref)
}
