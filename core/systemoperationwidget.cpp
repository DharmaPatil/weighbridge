/**
* @file systemsetuppagewidget.cpp
* @brief 系统使用配置类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2016-01-10
*/
#include <QSqlDatabase>
#include <QSqlQuery>

#include "systemoperationwidget.h"
#include "profile.h"
#include "global.h"

SystemOperationWidget::SystemOperationWidget(Workbench *wb, QWidget *parent)
    :  OptionWidget(wb, parent)
{
    ui.setupUi(this);
    initializePage();
    connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(save()));
}

SystemOperationWidget::~SystemOperationWidget()
{

}

void SystemOperationWidget::initializePage()
{

}

void SystemOperationWidget::save()
{

}
