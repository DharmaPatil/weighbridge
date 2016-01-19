#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QWidget>

namespace Ui {
class ConnectionPage;
}

class Zgu100sgy;
class Workbench;
class QListWidgetItem;

class ConnectionPage : public QWidget
{
	Q_OBJECT

public:
    explicit ConnectionPage(Zgu100sgy *instr, QWidget *parent = 0);
	~ConnectionPage();

	Workbench *workbench() const;

private slots:
	void on_reset_clicked();
	void on_reconnect_clicked();
	void on_port_currentIndexChanged(const QString &port);
	void on_send_clicked();
	void on_log_itemDoubleClicked(QListWidgetItem *item);

private:
	Ui::ConnectionPage *ui;
    Zgu100sgy *m_instrument;
};

#endif // CONNECTIONPAGE_H
