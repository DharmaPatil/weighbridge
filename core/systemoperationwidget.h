/**
* @file systemoperationwidget.h
* @brief 系统使用类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#ifndef SYSTEMOPERATIONWIDGET_H
#define SYSTEMOPERATIONWIDGET_H

#include "optionwidget.h"
#include "ui_operationwidget.h"

class SystemOperationWidget : public OptionWidget
{
    Q_OBJECT

public:
    SystemOperationWidget(Workbench *wb, QWidget *parent = 0);
    ~SystemOperationWidget();

    virtual void initializePage();

private slots:
    void save();

private:
    Ui::OperationWidget ui;
};

#endif // SYSTEMOPERATIONWIDGET_H
