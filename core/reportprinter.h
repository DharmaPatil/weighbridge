/**
* @file reportprinter.h
* @brief 报表打印类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-14
*/
#ifndef REPORTPRINTER_H
#define REPORTPRINTER_H

#include "datastorage.h"

class ReportPrinter : public DataStorage
{
    Q_OBJECT

public:
    ReportPrinter(QObject *parent = 0);
    ~ReportPrinter();

    virtual bool store(const QList<int> &id, QSqlDatabase db);
};

#endif // REPORTPRINTER_H
