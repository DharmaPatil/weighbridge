#pragma once

#include <QObject>
#include <QMap>
#include <QAction>

class QAction;
class QKeySequence;
class QIcon;
class QActionGroup;
class ReportItemCreator;
class QMenu;

class Icons
{
public:
	Icons();
	QIcon icon(const QString &name);
	QIcon operator[](const QString &name) { return icon(name); }

private:
	QMap<QString, QIcon> icons;
	QString path;
};

class Actions : public QObject
{
	Q_OBJECT

public:
	explicit Actions(QObject *parent = 0);
	~Actions();

	Icons icons;

	QAction *fileNew;
	QAction *fileOpen;
	QAction *fileSave;
	QAction *fileSaveAs;
	QAction *filePrint;
	QAction *fileSetupPage;
	QAction *appExit;

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];

	QAction *editUndo;
	QAction *editRedo;
	QAction *editCut;
	QAction *editCopy;
	QAction *editPaste;
	QAction *editDelete;
	QAction *editSelectAll;

	QAction *toolArrow;
	QAction *toolLine;
	QAction *toolRect;
	QAction *toolText;
	QAction *toolImage;

	QAction *textBold;
	QAction *textItalic;
	QAction *textUnder;

	QAction *textLeft;
	QAction *textCenter;
	QAction *textRight;
	QAction *textJustify;

	QActionGroup *textAlignGroup;

	QActionGroup *toolGroup;

	QAction *penWidth;
	QAction *penNull;
	QAction *penWidthCustom;
	QActionGroup *penWidthGroup;
	QMenu *penWidthMenu;

	QAction *createAction(ReportItemCreator *creator, QObject *parent = 0);

private:
	QAction *createAction(const QString &name, const QString &text);
	QAction *createAction(const QString &name, const QString &text, const QKeySequence &shortcut);
	QActionGroup *createGroup(QObject *parent, bool exclusive);
	void createPenWidth();
};
