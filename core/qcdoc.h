/**
* @file qcdoc.h
* @brief 质控文档类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef QCDOC_H
#define QCDOC_H

#include <QObject>

class QcDoc : public QObject
{
    Q_OBJECT
public:
    explicit QcDoc(QObject *parent = 0);
    ~QcDoc();

signals:

public slots:
};

#endif // QCDOC_H
