﻿/**
* @file messagewidget.h
* @brief 应用程序窗体消息提示类定义
* @ingroup core
* @author walterqin（walterqin@hotmail.com）
* @date 2015-09-14
*/
#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QList>
#include <QAbstractListModel>
#include <QTime>
#include <QMultiHash>

namespace Ui {
    class MessageWidget;
}

class QTimer;
class Workbench;

/**
* @brief message model定义
*/
class MessageModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit MessageModel(QObject *parent = 0);

	virtual int rowCount(const QModelIndex &parent) const;
	virtual int columnCount(const QModelIndex &parent) const;

	virtual QVariant data(const QModelIndex &index, int role) const;

	bool showMessage(Workbench *wb, int channel, int type, const QString &text, bool show = true);
	int messageCount(int type) const;
	bool hasWarnings() const;
	void confirm();

private:
	struct Message
	{
		Workbench *workbench;
		int type;
		int channel;
		bool confirmed;
		bool restored;
		QTime time;
		QTime restoreTime;
		QString text;
		QString fullText;
	};
	QList<Message> m_messages;
	QMultiHash<QString, int> m_msgIndex;	// message text to list index
	QVector<int> m_count;
	int m_confirmIndex;
};

/**
* @brief Message view定义，该类是QWidget的子类
*/
class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QWidget *parent = 0);
    ~MessageWidget();

	QAction *toggleAction() const { return m_toggleAction; }

	static MessageWidget *instance();

public slots:
	void showMessage(Workbench *wb, int channel, int type, const QString &text, bool show = true);

protected:
	virtual void showEvent(QShowEvent *);
	virtual void hideEvent(QHideEvent *);

private slots:
	void on_mute_toggled(bool checked);
	void on_confirm_clicked();
	void on_wordWrap_toggled(bool checked);

private:
    Ui::MessageWidget *ui;
	QAction *m_toggleAction;
	MessageModel *m_messages;
	QTimer *m_delayHideTimer;
	static MessageWidget *m_instance;
};

#endif // MESSAGEWIDGET_H
