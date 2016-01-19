/**
* @file gainparamwidget.h
* @brief 增益参数类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#ifndef GAINPARAMWIDGET_H
#define GAINPARAMWIDGET_H

#include <QWidget>
#include "optionwidget.h"
#include "ui_gainparamwidget.h"

class GainParamWidget : public OptionWidget
{
public:
    GainParamWidget(Workbench *wb, QWidget *parent = 0);
    ~GainParamWidget();

    void initializePage();

private slots:
    void save();

private:
    Ui::gainParamWidget ui;
};

#endif // GAINPARAMWIDGET_H
