/**
* @file statuswidget.h
* @brief 传感器状态波形显示类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-24
*/
#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>
#include <QVector>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>

class QPoint;
class Workbench;

namespace Ui {
class StatusWidgetClass;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(Workbench *wb, QWidget *parent = 0);
    ~StatusWidget();

    void setTitleString(QString &title);
    void setCurveData(qint16 &data);

signals:

public slots:
    void plotWaveCurve(qint16 &data);
    void setSensorRefPoint(QVector<qint16> &ref);

private:
    enum { MaxCount = 2048 };

    Ui::StatusWidgetClass *ui;
    QwtPlotCurve *m_waveplot;
    QVector<QPoint> m_curveData;
    QVector<qint16> m_refPoint;
    Workbench *m_workbench;
    QString m_title;
    int m_offset;
};

#endif // STATUSWIDGET_H
