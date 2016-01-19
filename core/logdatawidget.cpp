#include <QtGui>
#include "logdatawidget.h"
#include "profile.h"

LogDataWidget::LogDataWidget(QWidget *parent)
    : QWidget(parent)
{
    QIcon logWarmIcon(style()->standardPixmap(QStyle::SP_MessageBoxWarning));
    QIcon logCritialIcon(style()->standardPixmap(QStyle::SP_MessageBoxCritical));
    QIcon logInfoIcon(style()->standardPixmap(QStyle::SP_MessageBoxInformation));

    QStringList logTypeLbl;
    logTypeLbl << tr("系统日志");

    m_logTypeWidget = new QTreeWidget;
    m_logTypeWidget->setHeaderLabels(logTypeLbl);
    addLogType(logCritialIcon, tr("紧急"));
    addLogType(logWarmIcon, tr("警告"));
    addLogType(logInfoIcon, tr("提示"));

    QStringList messageLabels;
    messageLabels << tr("概要") << tr("生成者") << tr("日期");

    m_logBriefWidget = new QTreeWidget;
    m_logBriefWidget->setHeaderLabels(messageLabels);
    m_logBriefWidget->resizeColumnToContents(0);
    m_logBriefWidget->resizeColumnToContents(1);

    m_logDetail = new QTextEdit;
    m_logDetail->setReadOnly(true);

    m_logDataSplitter = new QSplitter(Qt::Vertical);
    m_logDataSplitter->addWidget(m_logBriefWidget);
    m_logDataSplitter->addWidget(m_logDetail);
    m_logDataSplitter->setStretchFactor(1, 1);

    m_mainSplitter = new QSplitter(Qt::Horizontal);
    m_mainSplitter->addWidget(m_logTypeWidget);
    m_mainSplitter->addWidget(m_logDataSplitter);
    m_mainSplitter->setStretchFactor(1, 1);
}

LogDataWidget::~LogDataWidget()
{

}

void LogDataWidget::addLogType(const QIcon &icon, const QString &name)
{
    QTreeWidgetItem *root;
    if (m_logTypeWidget->topLevelItemCount() == 0)
    {
        root = new QTreeWidgetItem(m_logTypeWidget);
        root->setText(0, tr("日志类型"));
        m_logTypeWidget->setItemExpanded(root, true);
    }
    else
    {
        root = m_logTypeWidget->topLevelItem(0);
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem(root);
    newItem->setText(0, name);
    newItem->setIcon(0, icon);

    if (!m_logTypeWidget->currentItem())
        m_logTypeWidget->setCurrentItem(newItem);
}

void LogDataWidget::addMessage(const QString &subject, const QString &from,
                const QString &date)
{

}
