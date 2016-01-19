/**
* @file abstractanalysisdocmodel.h
* @brief 称重测量文档基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef ABSTRACTANALYSISDOCMODEL_H
#define ABSTRACTANALYSISDOCMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QAtomicInt>

#include "core_global.h"
#include "global.h"

class AbstractAnalysisDoc;
class Workbench;
class AnalysisResultItem;

//////////////////////////////////////////////////////////
/// \brief The ChannelData struct
///
struct ChannelData
{
    int m_len;
    QMap<int, short> m_rawData;
    QMap<int, short> m_filterData;
};


///////////////////////////////////////////////////////////////////////
/// \brief The AbstractAnalysisDocModel class
///
class CORE_EXPORT AbstractAnalysisDocModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    AbstractAnalysisDocModel(AbstractAnalysisDoc *doc, QObject *parent = 0);
    ~AbstractAnalysisDocModel();

    Workbench *workbench() const;

    void setDoc(AbstractAnalysisDoc *doc);
    AbstractAnalysisDoc *doc() const { return m_doc; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                           int role = Qt::EditRole);

    void insertResultData(QStringList &data, int row, const QModelIndex &parent = QModelIndex());

    void setEditable(bool editable);
    bool isEditable() const { return m_editable; }


protected slots:
    void aboutToInsertData(QStringList &data);
    void insertData(QStringList &data);
    void onAddSample(QList<int> &raw, QList<int> &flt);

protected:
    void setExpandRow(bool e);
    void setExpandColumn(bool e);

protected:
    bool m_editable;

    AbstractAnalysisDoc *m_doc;
    bool m_expandColumn;
    bool m_expandRow;

    QStringList m_headerTitle;
    QList<AnalysisResultItem> m_result;
    QList<ChannelData> m_sample;
};

#endif // ABSTRACTANALYSISDOCMODEL_H
