/**
* @file blind.cpp
* @brief 模拟数据类定实现
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-10-01
*/
#include "blink.h"
#include <QWidget>
#include <QTimerEvent>

Blink::Blink(QObject *parent) : QObject(parent)
{
	m_showTime = 1200;
	m_hideTime = 800;
	m_timerId = startTimer(m_showTime);
	m_show = true;
}

void Blink::addWidget(QWidget *widget)
{
	Item item;
	item.widget = widget;
	item.originPalette = widget->palette();

	item.showPalette = widget->palette();
	item.showPalette.setColor(QPalette::WindowText, Qt::red);

	item.hidePalette = widget->palette();
	item.hidePalette.setColor(QPalette::WindowText, Qt::transparent);

	m_items.append(item);
}

void Blink::timerEvent(QTimerEvent *e)
{
	if (m_timerId == e->timerId())
	{
		killTimer(m_timerId);
		m_show = !m_show;
		m_timerId = startTimer(m_show ? m_showTime : m_hideTime);

		foreach (const Item &item, m_items) {
			if (item.widget)
				item.widget->setPalette(
							m_show ? item.showPalette
								   : item.hidePalette);
		}
	}
}

void Blink::start()
{
	if (m_timerId)
		return;

	m_timerId = startTimer(m_showTime);
}

void Blink::stop()
{
	if (!m_timerId)
		return;

	killTimer(m_timerId);
	m_show = true;
	foreach (const Item &item, m_items) {
		if (item.widget)
			item.widget->setPalette(item.originPalette);
	}
}
