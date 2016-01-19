#include "connectionpage.h"
#include "ui_connectionpage.h"
#include "serial.h"
#include "profile.h"
#include "wbapp.h"
#include "workbench.h"
#include "zgu100sgy.h"
#include "global.h"

ConnectionPage::ConnectionPage(Zgu100sgy *instr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionPage),
    m_instrument(instr)
{
    ui->setupUi(this);
    ui->commandGroup->setVisible(qApp->isSystemLogin());
    connect(qApp, SIGNAL(userLogin(bool)), ui->commandGroup, SLOT(setVisible(bool)));

    QStringList ports = Serial::EnumDevices();
    ui->port->blockSignals(true);
    ui->port->addItem(Instrument::tr("自动"));
    ui->port->addItems(ports);
    ui->port->setCurrentIndex(
                ui->port->findText(
                    profile.value("instrument/port").toString(), Qt::MatchExactly));
    ui->port->blockSignals(false);
}

ConnectionPage::~ConnectionPage()
{
    delete ui;
}

Workbench *ConnectionPage::workbench() const
{
    return m_instrument->workbench();
}

void ConnectionPage::on_reconnect_clicked()
{
    WaitCursor wait;
    Instrument *ins = m_instrument;
    ins->disconnectFromHost();
    ins->connectToHost();
    ins->waitForConnected();
    if (ins->isConnected())
        workbench()->information(tr("连接成功"));
    else
        workbench()->information(tr("连接失败"));
}

void ConnectionPage::on_port_currentIndexChanged(const QString &port)
{
    profile.setValue("instrument/port", port);
}

void ConnectionPage::on_reset_clicked()
{
    WaitCursor wait;
    Instrument *ins = m_instrument;
    if (!ins)
        return;
    ins->reset();
}

int str2hex(const char *ptr, char *buf, int maxlen, bool *ok)
{
    int len = 0;
    while(len < maxlen)
    {
        // skip space
        while(*ptr == ' ' || *ptr == '\t')
            ptr++;
        if (*ptr == '\0')
            break;

        // 高位
        *buf = 0;
        if (*ptr >= '0' && *ptr <= '9')
            *buf = *ptr - '0';
        else if (*ptr >= 'a' && *ptr <= 'f')
            *buf = *ptr - 'a' + 10;
        else if (*ptr >= 'A' && *ptr <= 'F')
            *buf = *ptr - 'A' + 10;
        else
        {
            if (ok)
                *ok = false;
            return len;
        }
        ++ptr;

        // 低位
        if (*ptr >= '0' && *ptr <= '9')
        {
            *buf <<= 4;
            *buf |= *ptr - '0';
            ++ptr;
        }
        else if (*ptr >= 'a' && *ptr <= 'f')
        {
            *buf <<= 4;
            *buf |= *ptr - 'a' + 10;
            ++ptr;
        }
        else if (*ptr >= 'A' && *ptr <= 'F')
        {
            *buf <<= 4;
            *buf |= *ptr - 'A' + 10;
            ++ptr;
        }
        else if (*ptr != ' ' && *ptr != '\t' && *ptr != '\0')
        {
            if (ok)
                *ok = false;
            return len;
        }

        ++len;
        ++buf;
    }

    if (*ok)
        *ok = true;
    return len;
}

QString hex2str(const char *buf, int len)
{
    QByteArray s(len * 3, Qt::Uninitialized);
    char *p = s.data();

    for(; len--; p += 3)
        qsnprintf(p, 4, "%02X ", (int)(uchar)*buf++);

    return s;
}

void ConnectionPage::on_send_clicked()
{
    char buf[128];
    bool ok;
    int len = str2hex(ui->cmd->text().toAscii().constData(), buf, 128, &ok);
    ui->cmd->selectAll();

    if (!ok)
    {
        warning(tr("输入中含有非法字符, 请输入表示十六进制的字符."));
        return;
    }

    ui->log->addItem(tr("请求(0x%1): %2")
                     .arg(len, 8, 16, QChar('0'))
                     .arg(hex2str(buf, len)));

    len = m_instrument->talk(buf, 128, buf, len);

    ui->log->addItem(tr("应答(0x%1): %2")
                     .arg((unsigned)len, 0, 16, QChar('0'))
                     .arg(len > 0 ? hex2str(buf, len) : QString()));
}

void ConnectionPage::on_log_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    ui->cmd->setText(item->text().mid(item->text().indexOf(": ")));
    ui->cmd->setFocus();
    ui->cmd->selectAll();
}
