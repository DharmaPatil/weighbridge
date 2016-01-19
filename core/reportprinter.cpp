/**
* @file reportprinter.cpp
* @brief 打印类的实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-14
*/

#include <QSqlDatabase>
#include <QPrintDialog>
#include <QPainter>

#include "global.h"
#include "profile.h"
#include "workbench.h"
#include "reportprinter.h"

ReportPrinter::ReportPrinter(QObject *parent)
    : DataStorage(parent)
{

}

ReportPrinter::~ReportPrinter()
{

}

bool ReportPrinter::store(const QList<int> &id, QSqlDatabase db)
{
    Q_UNUSED(db)
    if (id.isEmpty())
        return true;

    return true;
}
