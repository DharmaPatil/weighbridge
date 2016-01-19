/**
* @file abstractanalysisdocmodel.cpp
* @brief 称重测量文档基类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#include <QtDebug>
#include <QRegExp>
#include <QApplication>

#include "abstractanalysisdocmodel.h"
#include "analysisdoc.h"
#include "workbench.h"
#include "analysisresult.h"
#include "profile.h"
#include "global.h"

AbstractAnalysisDocModel::AbstractAnalysisDocModel(AbstractAnalysisDoc *doc, QObject *parent)
    : QAbstractTableModel(parent)
    , m_doc(doc)
{
    m_editable = false;
}

AbstractAnalysisDocModel::~AbstractAnalysisDocModel()
{

}

void AbstractAnalysisDocModel::setDoc(AbstractAnalysisDoc *doc)
{
    m_doc = qobject_cast<AbstractAnalysisDoc*>(doc);
    reset();
}

Workbench *AbstractAnalysisDocModel::workbench() const
{
    return m_doc->workbench();
}

int AbstractAnalysisDocModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_result.isEmpty() ? 0 : m_result.count();
}

int AbstractAnalysisDocModel::columnCount(const QModelIndex &/*parent*/) const
{
    return m_headerTitle.count();
}

QVariant AbstractAnalysisDocModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        if (isEditable() && index.row() == rowCount() - 1)
        {
            switch (index.column())
            {

            }
        }
        break;
    case Qt::TextAlignmentRole:
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    return QVariant();
}

bool AbstractAnalysisDocModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QString newValue = value.toString();
        QString oldValue = index.data().toString();

        if (newValue == oldValue)
            return false;

        QRegExp pattern("[+-]?\\d+(\\.(\\d+)?)?([eE][+-]?\\d+)?");
        if (pattern.indexIn(newValue) < 0)
        {
            if (isEditable() && index.row() == rowCount() - 1)
            {
                // !!! 数据处理
            }
            return false;
        }

        double d = pattern.cap().toDouble();

        if (isEditable() && index.row() == rowCount() - 1)
        {
            // !!! 数据处理
            emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags AbstractAnalysisDocModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (m_editable)
        flags |= Qt::ItemIsEditable;
    return flags;
}

QVariant AbstractAnalysisDocModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::VerPattern)
        return section + 1;

    return m_headerTitle.value(section);
}

bool AbstractAnalysisDocModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Vertical || role != Qt::EditRole)
        return QAbstractTableModel::setHeaderData(section, orientation, value, role);

    if ((unsigned)section >= (unsigned)m_headerTitle.size())
        return false;

    m_headerTitle[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

void AbstractAnalysisDocModel::insertResultData(QStringList &data, int row, const QModelIndex &parent)
{
    Q_UNUSED(data)
    Q_UNUSED(row)
    Q_UNUSED(parent)
}

void AbstractAnalysisDocModel::aboutToInsertData(QStringList &data)
{
    Q_UNUSED(data)
}

void AbstractAnalysisDocModel::insertData(QStringList &data)
{
    Q_UNUSED(data)
}

void AbstractAnalysisDocModel::onAddSample(QList<int> &raw, QList<int> &flt)
{

}

void AbstractAnalysisDocModel::setEditable(bool editable)
{
    if (m_editable == editable)
        return;

    m_editable = editable;
    reset();
}

void AbstractAnalysisDocModel::setExpandRow(bool e)
{
    if (m_expandRow == e)
        return;

    emit layoutAboutToBeChanged();
    m_expandRow = e;
    emit layoutChanged();

    reset();
}

void AbstractAnalysisDocModel::setExpandColumn(bool e)
{
    if (m_expandColumn == e)
        return;

    emit layoutAboutToBeChanged();
    m_expandColumn = e;
    emit layoutChanged();

    reset();
}
