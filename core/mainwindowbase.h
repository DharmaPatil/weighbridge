/**
* @file mainwindowbase.h
* @brief 主窗口的基类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef MAINWINDOWBASE_H
#define MAINWINDOWBASE_H

#include <QtGui/QMainWindow>
#include <QToolBar>

class QAction;
class QToolButton;

class ToolBar : public QToolBar
{
    Q_OBJECT

public:
    ToolBar(QWidget *parent = 0) : QToolBar(parent) {}

    QToolButton *toolButtonForAction(QAction *action) const;

    using QToolBar::setToolButtonStyle;
    void setToolButtonStyle(QAction *action, Qt::ToolButtonStyle style);

    void setToolButtonAutoRepeat(QAction *action, bool on = true);
};

class MainWindowBase : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowBase(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindowBase();

protected:
    ToolBar *createFixedToolbar();

signals:

public slots:
};

#endif // MAINWINDOWBASE_H
