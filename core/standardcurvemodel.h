/**
* @file standardcurvemodel.h
* @brief 标准曲线model类
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-14
*/
#ifndef STANDARDCURVEMODEL_H
#define STANDARDCURVEMODEL_H

#include "core_global.h"

#include <QAbstractTableModel>
#include <QStringList>

class StandardCurve;
class AnalysisTemplate;

class CORE_EXPORT StandardCurveModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIndex { CI_Standard, CI_Measure, CI_Calc, CI_Count };

    StandardCurveModel(QObject *parent = 0);
    ~StandardCurveModel();

    void setCurve(StandardCurve *curve);
    StandardCurve *curve() const { return m_curve; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);


    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void setEditable(bool editable);
    bool isEditable() const { return m_editable; }

private:
    StandardCurve *m_curve;
    bool m_editable;
    QVariant m_x, m_y;
    QStringList m_headerTitle;

};

#endif // STANDARDCURVEMODEL_H
