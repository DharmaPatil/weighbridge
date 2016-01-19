/**
* @file MeasureTemplate.h
* @brief 测量方法模板类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef MEASURETEMPLATE_H
#define MEASURETEMPLATE_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QVector>

#include "global.h"
//#include "regression.h"
#include "analysisresult.h"

class MeasureTemplate : public QObject
{
    Q_OBJECT
public:
    explicit MeasureTemplate(QObject *parent = 0);
    ~MeasureTemplate();

signals:

public slots:
};

#endif // MEASURETEMPLATE_H
