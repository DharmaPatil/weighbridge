/**
* @file instrtypedialog.cpp
* @brief 轨道衡仪器类型选择对话窗口实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-11-14
*/
#include <QPushButton>

#include "instrtypedialog.h"
#include "global.h"
#include "ui_instrtypedialog.h"
#include "serial.h"
#include "workbench.h"
#include "instrument.h"
#include "wbapp.h"

InstrTypeDialog::InstrTypeDialog(const QList<QVariantMap> &instrInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InstrTypeDialog)
{
    ui->setupUi(this);
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (button)
        button->setEnabled(false);

    QStringList ports;
    ports.append(Instrument::tr("自动"));
    ports.append(Serial::EnumDevices());
    ui->port_1->addItems(ports);
    ui->port_2->addItems(ports);

    ports = qApp->tmpValue("port").toStringList();
    if (ports.size() > 0)
        ui->port_1->setCurrentIndex(qMax(0, ui->port_1->findText(ports.value(0))));
    if (ports.size() > 1)
        ui->port_2->setCurrentIndex(qMax(0, ui->port_2->findText(ports.value(1))));

    m_instrInfo = instrInfo;
    bool sim = qApp->tmpValue("debug/sim").toBool();
    QStringList instrs;
    foreach(QVariantMap m, instrInfo)
    {
        if (sim || !m.value("hide").toBool())
            instrs << m.value("name").toString();
    }
    ui->type->addItems(instrs);

    QString instrName = qApp->value("settings/instr").toString();
    ui->type->setCurrentRow(instrs.indexOf(instrName));

    connect(ui->type, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accept()));
}

InstrTypeDialog::~InstrTypeDialog()
{
    delete ui;
}

QString InstrTypeDialog::instrName() const
{
    return ui->type->currentIndex().data().toString();
}

QString InstrTypeDialog::port(int index) const
{
    QComboBox *box[] = { ui->port_1, ui->port_2 };
    if ((unsigned)index < 2)
        return box[index]->currentText();
    return QString();
}

QStringList InstrTypeDialog::ports() const
{
    return QStringList() << ui->port_1->currentText()
                         << ui->port_2->currentText();
}

void InstrTypeDialog::on_type_currentTextChanged(const QString &currentText)
{
    QPushButton *button = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (button)
        button->setEnabled(!currentText.isEmpty());

    QVariantMap instrInfo = find_instr_info(m_instrInfo, currentText);
    if (instrInfo.isEmpty())
        return;

    QVariantList subdir = instrInfo.value("subdir").toList();
    ui->portLabel_2->setVisible(subdir.size() > 1);
    ui->port_2->setVisible(subdir.size() > 1);

    if (subdir.isEmpty())
        ui->portLabel_1->setText(instrInfo.value("name").toString());
    else
    {
        ui->portLabel_1->setText(subdir.value(0).toMap().value("title").toString());
        ui->portLabel_2->setText(subdir.value(1).toMap().value("title").toString());
    }
}
