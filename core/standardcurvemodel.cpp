#include <QtDebug>
#include <QRegExp>

#include "standardcurvemodel.h"
#include "standardcurve.h"
#include "analysistemplate.h"

StandardCurveModel::StandardCurveModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_curve(0)
    , m_editable(false)
{
    m_headerTitle << tr("标准品值") << tr("测量值") << tr("计算值");
}

StandardCurveModel::~StandardCurveModel()
{

}

void StandardCurveModel::setCurve(StandardCurve *curve)
{
    m_curve = curve;
    m_x.clear();
    m_y.clear();
    reset();
}

int StandardCurveModel::rowCount(const QModelIndex &parent) const
{
    return m_curve ? m_curve->pointCount() + (isEditable() ? 1 : 0) : 0;
}

int StandardCurveModel::columnCount(const QModelIndex &parent) const
{
    return CI_Count;
}

QVariant StandardCurveModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
    case Qt::EditRole:
    case Qt::DisplayRole:
        if (isEditable() && index.row() == rowCount()-1)
        {
            switch(index.column())
            {
            case CI_Standard:
                return m_y.toString();
            case CI_Measure:
                return m_x.toString();
            case CI_Calc:
                return QVariant();
            }
        }
        else
        {
            switch(index.column())
            {
            case CI_Standard:
                return QString("%1 %2")
                    .arg(m_curve->points()[index.row()].y())
                    .arg(m_curve->yUnit());
            case CI_Measure:
                // TODO: 显示测量值单位, 根据AssayTemplate
                return QString("%1 %2 %3")
                    .arg(m_curve->points()[index.row()].flagString())
                    .arg(m_curve->points()[index.row()].x())
                    .arg(m_curve->xUnit());
            case CI_Calc:
                return m_curve->isValid() ? QString("%1 %2")
                    .arg(m_curve->y(m_curve->points()[index.row()].x()))
                    .arg(m_curve->yUnit())
                    : QVariant();
            }
        }
        break;

    case Qt::TextAlignmentRole:
        switch(index.column())
        {
        case CI_Standard:
        case CI_Measure:
        case CI_Calc:
            return int(Qt::AlignRight | Qt::AlignVCenter);
        }
        break;
    }

    return QVariant();
}

bool StandardCurveModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
            if (isEditable() && index.row() == rowCount()-1)
            {
                switch(index.column())
                {
                case CI_Standard:	m_y.clear(); break;
                case CI_Measure:	m_x.clear(); break;
                default: break;
                }
            }
            return false;
        }

        double d = pattern.cap().toDouble();

        if (isEditable() && index.row() == rowCount()-1)
        {
            switch(index.column())
            {
            case CI_Standard:	m_y = d; break;
            case CI_Measure:	m_x = d; break;
            default: break;
            }

            bool xok, yok;
            double x, y;
            x = m_x.toDouble(&xok);
            y = m_y.toDouble(&yok);

            if (!xok || !yok)
            {
                emit dataChanged(index, index);
                return true;
            }

            m_curve->addPoint(x, y);
            m_x.clear();
            m_y.clear();

            int row = rowCount();
            beginInsertRows(QModelIndex(), row, row);
            endInsertRows();
        }
        else
        {
            StdPoint point = m_curve->point(index.row());

            switch(index.column())
            {
            case CI_Standard:
                if (point.y() == d)
                    return false;

                point.setY(d);
                m_curve->setPoint(index.row(), point);

                break;

            case CI_Measure:
                if (point.x() == d)
                    return false;

                point.setX(d);
                m_curve->setPoint(index.row(), point);

                break;

            case CI_Calc:
                return false;
            }
        }

        StdPoint point = m_curve->point(index.row());
        point.rflags() |= StdPoint::ManualInput;
        m_curve->setPoint(index.row(), point);
        m_curve->setModified(true);

        if (m_curve->pointCount() > 1)
        {
            m_curve->setValid(false);
            m_curve->update();
            emit dataChanged(this->index(0, 0), this->index(rowCount()-1, rowCount()-1));
        }
        else
        {
            emit dataChanged(index, index);
        }

        return true;
    }
    return false;
}

QVariant StandardCurveModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return section+1;

    return m_headerTitle.value(section);
}

bool StandardCurveModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Vertical || role != Qt::EditRole)
        return QAbstractTableModel::setHeaderData(section, orientation, value, role);

    if ((unsigned)section >= (unsigned)m_headerTitle.size())
        return false;

    m_headerTitle[section] = value.toString();
    emit headerDataChanged(orientation, section, section);
    return true;
}

Qt::ItemFlags StandardCurveModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (m_editable && index.column() != CI_Calc)
        flags |= Qt::ItemIsEditable;
    return flags;
}

void StandardCurveModel::setEditable(bool editable)
{
    if (m_editable == editable)
        return;

    m_editable = editable;
    m_x.clear();
    m_y.clear();
    reset();
}

bool StandardCurveModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!isEditable())
        return false;

    beginRemoveRows(parent, row, row+count-1);
    for(int i=0; i<count; i++)
        m_curve->removePoint(row);
    m_curve->update();
    endRemoveRows();

    return true;
}

