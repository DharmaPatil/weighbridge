/**
* @file appwindow.h
* @brief 应用程序主窗体类实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#include <QTabBar>
#include <QStackedLayout>
#include <QBoxLayout>
#include <QAction>
#include <QCloseEvent>

#include "appwindow.h"
#include "wbapp.h"
#include "workbench.h"
#include "mainwindow.h"
#include "wbaction.h"
#include "messagewidget.h"

/**
* @brief 构造函数
* @param [I] 父指针
*/
AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent)
{
	resize(800, 600);

    QWidget *widget = new QWidget;

    m_tabBar = new QTabBar;
    m_tabBar->setExpanding(false);
    m_tabBar->setShape(QTabBar::RoundedWest);
    m_tabBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_tabBar->setIconSize(QSize(16, 16));

    m_layout = new QStackedLayout;

    QBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addSpacing(2);
    layout->addWidget(m_tabBar);
    layout->addItem(m_layout);
    widget->setLayout(layout);
    setCentralWidget(widget);

    connect(m_tabBar, SIGNAL(currentChanged(int)), m_layout, SLOT(setCurrentIndex(int)));
    //connect(m_layout, SIGNAL(currentChanged(int)), m_tabBar, SLOT(setCurrentIndex(int)));
    connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(onCurrentTabChanged(int)));

    foreach(Workbench *wb, qApp->workbenches())
    {
        MainWindow *w = wb->mainWindow();
        m_tabBar->addTab(w->windowTitle());
        m_layout->addWidget(w);
        w->setParent(widget, Qt::Widget);

        wb->actionManager()->exitAction->disconnect();
        connect(wb->actionManager()->exitAction, SIGNAL(triggered()), this, SLOT(close()));
        connect(w, SIGNAL(closed(QWidget*)), this, SLOT(removeWidget(QWidget*)));
    }

    MessageWidget::instance()->setParent(this, Qt::Tool);
}

/**
* @brief 析构函数
*/
AppWindow::~AppWindow()
{

}

/**
* @brief 窗体切换槽函数
* @param [I] 窗体索引
*/
void AppWindow::onCurrentTabChanged(int index)
{
    setWindowTitle(qApp->applicationName() + " - " + m_tabBar->tabText(index));
}

/**
* @brief 窗体关闭槽函数
* @param [I] 子窗体
*/
void AppWindow::removeWidget(QWidget *widget)
{
    int index = m_layout->indexOf(widget);
    if (index < 0)
        return;

    m_layout->removeWidget(widget);
    m_tabBar->removeTab(index);
}

/**
* @brief 窗体关闭事件处理器
* @param [I] 关闭事件
*/
void AppWindow::closeEvent(QCloseEvent *e)
{
    foreach(Workbench *wb, qApp->workbenches())
    {
        MainWindow *w = wb->mainWindow();
        if (w)
            w->close();
    }

    if (m_tabBar->count() > 0)
        e->ignore();
}
