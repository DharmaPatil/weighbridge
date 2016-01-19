/**
* @file samplecurveplot.h
* @brief 采样数据波形显示类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#ifndef SAMPLECURVEPLOT_H
#define SAMPLECURVEPLOT_H

#include "qwt_plot.h"
#include "core_global.h"
#include "global.h"

class QwtPlotCurve;
class QwtPlotMarker;

class CORE_EXPORT SampleCurvePlot : public QwtPlot
{
    Q_OBJECT

public:
    SampleCurvePlot(QWidget *parent = 0);
    ~SampleCurvePlot();

    QwtPlotCurve *channelCurve(int ch) const { return sampleData[ch].curve; }
    void setData(int ch, QVector<double> &data);
    void setFile(QString &filename);

private slots:
    void showCurve(QwtPlotItem *, bool on);
    void showPeak(double x, double y);

private:
    void init();

private:
    struct
    {
        QwtPlotCurve *curve;
        QVector<double> data;
    } sampleData[ChannelMax + 1];

    QwtPlotMarker *d_mrk1;
    QwtPlotMarker *d_mrk2;

    QString m_filename;
};

#endif // SAMPLECURVEPLOT_H
