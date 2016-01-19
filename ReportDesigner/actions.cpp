#include "actions.h"
#include "tools.h"
#include "itemfactory.h"
#include <QMenu>
#include <QCoreApplication>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QBrush>

Icons::Icons() : path(":/Resources/")
{
}

QIcon Icons::icon(const QString &name)
{
	QMap<QString, QIcon>::iterator it = icons.find(name);
	if (it != icons.end())
		return *it;

	QString pathname = path + name + ".png";
	QIcon i;

	if (QFileInfo(pathname).exists())
		i = QIcon(pathname);

	icons[name] = i;
	return i;
}

Actions::Actions(QObject *parent) : QObject(parent)
{
#define ACTION(name, text, shortcut) (name = createAction(#name, text, shortcut))
#define ACTION2(name, text) (name = createAction(#name, text))

	// 文件

	ACTION(fileNew,		tr("新建(&N)"),		QKeySequence::New);
	ACTION(fileOpen,	tr("打开(&O)..."),	QKeySequence::Open);
	ACTION(fileSave,	tr("保存(&S)"),		QKeySequence::Save);
	ACTION(fileSaveAs,	tr("另存为(&A)..."),	QKeySequence::SaveAs);
	ACTION(filePrint,	tr("打印..."),		QKeySequence::Print);
	ACTION(appExit,		tr("退出(&X)"),		tr("Alt+F4"));
	ACTION2(fileSetupPage,	tr("页面设置(&P)..."));

	for(int i=0; i<MaxRecentFiles; i++)
	{
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
	}

	// 编辑

	ACTION(editUndo,	tr("撤消(&U)"),		QKeySequence::Undo);
	ACTION(editRedo,	tr("重做(&R)"),		QKeySequence::Redo);
	ACTION(editCut,		tr("剪切(&X)"),		QKeySequence::Cut);
	ACTION(editCopy,	tr("复制(&C)"),		QKeySequence::Copy);
	ACTION(editPaste,	tr("粘贴(&P)"),		QKeySequence::Paste);
	ACTION(editDelete,	tr("清除(&D)"),		QKeySequence::Delete);
	ACTION(editSelectAll,tr("全选(&A)"),		QKeySequence::SelectAll);

	// 文本

	ACTION2(textBold,	tr("加粗"))->setCheckable(true);
	ACTION2(textItalic,	tr("斜体"))->setCheckable(true);
	ACTION2(textUnder,	tr("下划线"))->setCheckable(true);

	ACTION2(textLeft,	tr("左对齐"))->setCheckable(false);
	ACTION2(textCenter,	tr("居中"))->setCheckable(false);
	ACTION2(textRight,	tr("右对齐"))->setCheckable(false);
	ACTION2(textJustify,tr("分散对齐"))->setCheckable(false);

	textAlignGroup = createGroup(this, true);
	textAlignGroup->addAction(textLeft);
	textAlignGroup->addAction(textCenter);
	textAlignGroup->addAction(textRight);
	textAlignGroup->addAction(textJustify);

	createPenWidth();

	// 绘图工具

#define TOOLACTION(name, text)			\
	ACTION2(name, text);				\
	name->setCheckable(true);			\
	toolGroup->addAction(name)

#define TOOLACTION2(name, creator)		\
	name = createAction(creator);	\
	name->setObjectName(#name)

	toolGroup = createGroup(this, true);

	ItemFactory *factory = ItemFactory::instance();
	TOOLACTION2(toolArrow,	factory->creator(QObject::tr("指针")));
	TOOLACTION2(toolLine,	factory->creator(QObject::tr("直线")));
	TOOLACTION2(toolRect,	factory->creator(QObject::tr("矩形")));
	TOOLACTION2(toolText,	factory->creator(QObject::tr("文本")));
	TOOLACTION2(toolImage,	factory->creator(QObject::tr("图像")));

	connect(appExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

Actions::~Actions()
{
}

QAction *Actions::createAction(const QString &name, const QString &text)
{
	QAction *action = new QAction(icons[name], text, this);
	action->setObjectName(name);
	return action;
}

QAction *Actions::createAction(const QString &name, const QString &text, const QKeySequence &shortcut)
{
	QAction *action = createAction(name, text);
	action->setShortcut(shortcut);
	return action;
}

QAction *Actions::createAction(ReportItemCreator *creator, QObject *parent)
{
	QAction *action = new QAction(creator->icon(), creator->name(), parent ? parent : this);
	action->setCheckable(true);
	action->setData(reinterpret_cast<int>(creator));
	toolGroup->addAction(action);
	return action;
}

QActionGroup *Actions::createGroup(QObject *parent, bool exclusive)
{
	QActionGroup *g = new QActionGroup(parent);
	g->setExclusive(exclusive);
	return g;
}

static QIcon penWidthIcon(qreal w)
{
	const int width = 12, height = 12;
	QPixmap bmp(width, height);
	bmp.fill(Qt::transparent);
	QPainter painter(&bmp);
	QPen pen(Qt::black, bmp.physicalDpiY() * w / 72);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawLine(0, height/2, width, height/2);
	return bmp;
}

static QAction *penWidthAction(QObject *parent, qreal w)
{
	QAction *action = new QAction(penWidthIcon(w), QString("%1 pt").arg(w), parent);
	//action->setCheckable(true);
	action->setData(w);
	return action;
}

void Actions::createPenWidth()
{
	penWidthGroup = createGroup(this, true);

	penNull = new QAction(tr("无线条"), this);
	penNull->setCheckable(true);
	penWidthGroup->addAction(penNull);
	penWidthGroup->addAction(penWidthAction(this, 0.2));
	penWidthGroup->addAction(penWidthAction(this, 0.6));
	penWidthGroup->addAction(penWidthAction(this, 1.0));
	penWidthGroup->addAction(penWidthAction(this, 1.4));
	penWidthGroup->addAction(penWidthAction(this, 1.8));
	penWidthGroup->addAction(penWidthAction(this, 2.0));
	penWidthCustom = new QAction(tr("自定义(&C)..."), this);

	penWidthMenu = new QMenu();
	penWidthMenu->addActions(penWidthGroup->actions());
	penWidthMenu->addSeparator();
	penWidthMenu->addAction(penWidthCustom);

	penWidth = createAction("penWidth", tr("线宽(&W)"));
	penWidth->setMenu(penWidthMenu);
}
