/**
* @file weightwidget.h
* @brief 轨道衡称重主界面类定义
* @ingroup ui
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef WEIGHTWIDGET_H
#define WEIGHTWIDGET_H

#include <QWidget>
#include "global.h"

class Workbench;
class WeightDoc;
class WeightDocModel;
class QLineEdit;
class StatusPlot;
class QTableView;


////////////////////////////////////////////////////////////////////////
/// \brief The WeightWidget class
///
class WeightWidget : public QWidget
{
    Q_OBJECT

public:
    WeightWidget(Workbench *wb, QWidget *parent = 0);
    ~WeightWidget();

    Workbench *workbench() const { return m_workbench; }

private:
    void setupUi();
    void init();

private:
    QLineEdit *m_refPoint[ChannelMax];
    QLineEdit *m_total;
    StatusPlot *m_plot;

    WeightDoc *m_doc;
    WeightDocModel *m_model;
    QTableView *m_view;

    Workbench *m_workbench;

};

#endif // WEIGHTWIDGET_H
