#ifndef REPORTDESIGNER_H
#define REPORTDESIGNER_H

#include <QtGui/QMainWindow>
#include <QListWidget>
#include <QToolBox>
#include <QMap>

class ReportItemCreator;
class Report;
class QToolBar;
class ReportView;
class QFontComboBox;
class QComboBox;
class ReportItem;

class ToolBox : public QToolBox
{
	Q_OBJECT

public:
	enum DataRole { ReportItemRole = Qt::UserRole + 1, };
	explicit ToolBox(QWidget *parent = 0);

	int addGroup(const QString &name);
	int findGroup(const QString &name) const;
	void addReportItem(ReportItemCreator *item);

private:
};

class ReportDesigner : public QMainWindow
{
	Q_OBJECT

public:
	ReportDesigner(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ReportDesigner();

	void setReport(Report *report);

protected:
	virtual void closeEvent(QCloseEvent *e);

private slots:
	void fileNew();
	void open();
	bool save();
	bool saveAs();
	void print();
	void setupPage();
	void selectTool(QAction *tool);
	void textAlign(QAction *align);
	void currentFontChanged(const QString &fontFamily);
	void fontSizeChanged(const QString &size);
	void fontBoldChanged(bool checked);
	void fontItalicChanged(bool checked);
	void fontUnderlineChanged(bool checked);
	void scaleChanged(const QString &scale);
	void itemCreated(ReportItem *item);
	void createFinished(ReportItem *item);
	void penChanged(const QPen &pen);
	void brushChanged(const QBrush &brush);
	void applyFontChange(bool effectReport = true);
	void cut();
	void copy();
	void paste();
	void clear();
	void selectAll();
	void selectionChanged();
	void clipboardDataChanged();
	void openRecentFile();
	void setModified(bool m);

private:
	bool saveModified();
	void setLastDir(const QString &dir);
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);

private:
	QString m_curFile;

	ToolBox *m_toolbox;
	QToolBar *m_general;
	QToolBar *m_format;
	ReportItemCreator *m_tool;
	ReportView *m_view;
	Report *m_report;

	QComboBox *m_sceneScaleCombo;
	//QComboBox *m_itemColorCombo;
	//QComboBox *m_textColorCombo;
	QComboBox *m_fontSizeCombo;
	QFontComboBox *m_fontCombo;
	QComboBox *m_scaleCombo;

	QString m_lastDir;
	bool m_inSelectTool;
};

#endif // REPORTDESIGNER_H
