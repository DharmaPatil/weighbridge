/**
* @file datastorage.h
* @brief 打印类的基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-14
*/
#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QObject>
#include <QList>
#include <QSqlDatabase>

class DataStorage : public QObject
{
    Q_OBJECT
public:
    explicit DataStorage(QObject *parent = 0);
    ~DataStorage();

    virtual bool store(const QList<int> &id, QSqlDatabase db) = 0;
signals:

public slots:
};

#endif // DATASTORAGE_H
