/**
* @file modifycalibrationdialog.h
* @brief 定标曲线对话框类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-12-14
*/
#ifndef MODIFYCALIBRATIONDIALOG_H
#define MODIFYCALIBRATIONDIALOG_H

#include <QDialog>
#include "ui_stdcurvewidget.h"

class ModifyCalibrationDialog : public QDialog
{
public:
    ModifyCalibrationDialog(QWidget *parent = 0);
    ~ModifyCalibrationDialog();

private:
    Ui::StdCurveWidgetClass ui;
};

#endif // MODIFYCALIBRATIONDIALOG_H
