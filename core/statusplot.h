/**
* @file statusplot.h
* @brief 状态波形显示类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-24
*/
#ifndef STATUSPLOT_H
#define STATUSPLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;

////////////////////////////////////////////////////////////////////
/// \brief The StatusPlot class
///
class StatusPlot : public QwtPlot
{
    Q_OBJECT

public:
    StatusPlot(QWidget *parent = 0);
    QwtPlotCurve *statusCurve() const  { return data.curve;}

public slots:
    void setData(QList<int> &data);

private slots:
    void showCurve(QwtPlotItem *);

private:
    enum { MAXLEN = 2048 };
    struct
    {
        QwtPlotCurve *curve;
        double data[MAXLEN];
    } data;

    double timeData[MAXLEN];
};

#endif // STATUSPLOT_H
