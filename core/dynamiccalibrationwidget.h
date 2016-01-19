/**
* @file dynamiccalibrationwidget.h
* @brief 动态标定窗口类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-14
*/
#ifndef DYNAMICCALIBRATIONWIDGET_H
#define DYNAMICCALIBRATIONWIDGET_H

#include <QWidget>

class Workbench;
class CalibrationDoc;
class CalibrationDocModel;
class QTableView;

class DynamicCalibrationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DynamicCalibrationWidget(Workbench *wb, QWidget *parent = 0);
    ~DynamicCalibrationWidget();

    Workbench *workbench() const { return m_workbench; }
signals:

public slots:

private:
    Workbench *m_workbench;

    CalibrationDoc *m_dynDoc;
    CalibrationDocModel *m_dynDocModel;
    QTableView *m_tableView;
};

#endif // DYNAMICCALIBRATIONWIDGET_H
