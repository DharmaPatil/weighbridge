/**
* @file MeasureTemplate.h
* @brief 测量方法管理类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef MEASURETEMPLATEMANAGER_H
#define MEASURETEMPLATEMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>

#include "global.h"
#include "measuretemplate.h"

class MeasureTemplate;

class MeasureTemplateManager : public QObject
{
    Q_OBJECT
public:
    explicit MeasureTemplateManager(QObject *parent = 0);
    ~MeasureTemplateManager();

    void add(MeasureTemplate *t);
    QList<MeasureTemplate *> measure() const;
    MeasureTemplate *measure(int i) const;
    MeasureTemplate *measure(QString &name);
    void remove(int i);
    int count() const;

    int idOf(const QString &measureName) const;
    int indexOf(const QString &measureName) const;

    void save();
    void load();

signals:

public slots:

private:
    QList<MeasureTemplate*> m_itemList;
//    QMap<QString, const MeasureTemplate::Calibration *> m_calibration;
};

#endif // MEASURETEMPLATEMANAGER_H
