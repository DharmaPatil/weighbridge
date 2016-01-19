/**
* @file instrtypedialog.h
* @brief 轨道衡仪器类型选择对话框定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-14
*/
#ifndef INSTRTYPEDIALOG_H
#define INSTRTYPEDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class InstrTypeDialog;
}

class InstrTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InstrTypeDialog(const QList<QVariantMap>  &instrInfo, QWidget *parent = 0);
    ~InstrTypeDialog();

    QString instrName() const;
    QString port(int index) const;
    QStringList ports() const;

private slots:
    void on_type_currentTextChanged(const QString &currentText);

private:
    Ui::InstrTypeDialog *ui;
    QList<QVariantMap> m_instrInfo;
};

#endif // INSTRTYPEDIALOG_H
