/**
* @file blind.h
* @brief 模拟数据类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#ifndef BLINK_H
#define BLINK_H

#include "core_global.h"
#include <QObject>
#include <QPalette>
#include <QPointer>

class QWidget;

class CORE_EXPORT Blink : public QObject
{
    Q_OBJECT
public:
    explicit Blink(QObject *parent = 0);

	void addWidget(QWidget *widget);

public slots:
	void start();
	void stop();

protected:
	virtual void timerEvent(QTimerEvent *e);

private:
	int m_hideTime;
	int m_showTime;
	int m_timerId;
	bool m_show;
	struct Item {
		QPointer<QWidget> widget;
		QPalette originPalette;
		QPalette showPalette;
		QPalette hidePalette;
	};

	QList<Item> m_items;
};

#endif // BLINK_H
