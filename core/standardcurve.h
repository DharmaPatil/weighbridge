/**
* @file standardcurve.h
* @brief 标准曲线类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-24
*/
#ifndef STANDARDCURVE_H
#define STANDARDCURVE_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QStringList>

#include "core_global.h"
#include "global.h"
#include "regression.h"

////////////////////////////////////////////////////////////////////////
/// \brief The StdPoint class
///
class CORE_EXPORT StdPoint
{
public:
    enum Flag
    {
        ManualInput = 0x0001,		// x为手工输入值
        NullValue	= 0x8000,		// 空值
    };

    StdPoint() : zp(0), m_flags(0) {}
    StdPoint(const StdPoint &point) : xp(point.xp), yp(point.yp), zp(point.zp), m_flags(point.m_flags){}
    StdPoint(qreal xpos, qreal ypos) : xp(xpos), yp(ypos), zp(0), m_flags(0) {}
    StdPoint(qreal xpos, qreal ypos, qreal zpos) : xp(xpos), yp(ypos), zp(zpos), m_flags(0) {}

    qreal x() const { return xp; }
    void setX(qreal x) { xp = x; }
    qreal &rx() { return xp; }

    qreal y() const { return yp; }
    void setY(qreal y) { yp = y; }
    qreal &ry() { return yp; }

    qreal z() const { return zp; }
    void setZ(qreal z) { zp = z; }
    qreal &rz() { return zp; }

    int flags() const { return m_flags; }
    void setFlags(int flags) { m_flags = flags; }
    void clearFlag(int flags) { m_flags &= ~flags; }
    int &rflags() { return m_flags; }
    bool testFlag(int flag) const { return m_flags & flag; }

    QString flagString() const
    {
        QString s;
        if (m_flags & ManualInput)
            s += "M";
        return s;
    }

    bool isNull() const { return testFlag(NullValue); }
    void setNull() { xp = 0; m_flags = NullValue; }

private:
    qreal xp;			// 测量值
    qreal yp;			// 标准品值
    qreal zp;			// 回归值
    int m_flags;		// 点标志, 见enum Flag
};
Q_DECLARE_TYPEINFO(StdPoint, Q_MOVABLE_TYPE);
typedef QVector<StdPoint> StdPoints;


/////////////////////////////////////////////////////////////////////
/// \brief 标准曲线
///
class CORE_EXPORT StandardCurve : public QObject
{
    Q_OBJECT
public:
    explicit StandardCurve(QObject *parent = 0);
    StandardCurve(const QString &name, QObject *parent = 0);
    ~StandardCurve();

    void setName(const QString &name) { m_name = name; }
    QString name() const { return m_name; }

    void setXUnit(const QString &unit) { m_xunit = unit; }
    QString xUnit() const { return m_xunit; }

    void setYUnit(const QString &unit) { m_yunit = unit; }
    QString yUnit() const { return m_yunit; }

    QDate date() const { return m_date; }
    void setDate(const QDate &d) { m_date = d; }

    bool isModified() const { return m_modified; }
    void setModified(bool b) { m_modified = b; }

    bool isValid() const;
    void setValid(bool valid = true);

    weighbridge::RegressionMethod method() const { return m_method; }
    void setMethod(weighbridge::RegressionMethod method);

    weighbridge::Function xFun() const { return m_xFun; }
    weighbridge::Function yFun() const { return m_yFun; }
    void setXFun(weighbridge::Function xFun);
    void setYFun(weighbridge::Function yFun);

    int coefficientCount() const;
    QVector<double> coefficient() const { return m_coef; }
    double coefficient(int i) const { return m_coef.value(i); }
    void setCoefficient(const QVector<double> &c);

    StdPoints points() const { return m_points; }
    void setPoints(const StdPoints &points) { m_points = points; setValid(false); }

    void addPoint(double x, double y) { addPoint(StdPoint(x, y)); }
    void addPoint(const StdPoint &point);
    void setPoint(int i, const StdPoint &point) { m_points[i] = point; }
    void removePoint(int i);
    void clear();
    StdPoint& point(int i) { return m_points[i]; }
    int pointCount() const { return m_points.size(); }
    double correlation() const { return m_correlation; }

    void setCorrelation(double r) { m_correlation = r; }

    void update();
    Regression *regression() const { return m_regression; }
    double x(double y) const;
    double y(double x) const;

    double minDisplayX() const;
    double minDisplayY() const;
    double boundX(double x) const;
    double boundY(double y) const;

    virtual void addResult();
    virtual void finish();
    virtual void validate();

    // dynamic variables
    QVariant var(const QString &name) const;
    void setVar(const QString &name, const QVariant &val);
    QStringList vars() const;

    // debug infomation
    virtual QVariantMap info() const { return QVariantMap(); }
    virtual void setInfo(const QVariantMap &info) { Q_UNUSED(info)}

private:
    typedef QMap<QString, QVariant> Variables;

    bool m_modified;                            // 曲线已更改
    bool m_valid;                               // 曲线有效
    weighbridge::RegressionMethod m_method;		// 定标方法
    weighbridge::Function m_xFun;				// X函数
    weighbridge::Function m_yFun;				// Y函数
    QString m_name;                             // 曲线名称
    QString m_xunit;                            // X单位
    QString m_yunit;                            // Y单位
    StdPoints m_points;                         // 曲线点
    double m_correlation;                       // 相关系数
    QDate m_date;                               // 定标曲线保存日期
    Variables m_vars;                           // 定标变量
    Regression *m_regression;                   // 回归对象
    QVector<double> m_coef;                     // 系数
};

#endif // STANDARDCURVE_H
