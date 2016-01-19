/**
* @file appwindow.h
* @brief 应用程序主窗体类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QMainWindow>

class QStackedLayout;
class QTabBar;

class AppWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();

private slots:
    void onCurrentTabChanged(int index);
    void removeWidget(QWidget *widget);

protected:
    virtual void closeEvent(QCloseEvent *e);

private:
    QStackedLayout *m_layout;
    QTabBar *m_tabBar;
};

#endif // APPWINDOW_H
