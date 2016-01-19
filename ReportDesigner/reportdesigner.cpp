#include "reportdesigner.h"
#include "workbench.h"
#include "actions.h"
#include "reportitemcreator.h"
#include "report.h"
#include "reportview.h"
#include "reportitem.h"
#include "itemfactory.h"
#include "command.h"

#include <QtGui>

///////////////////////////////////////////////////////////////////////////////
// ToolBox

ToolBox::ToolBox(QWidget *parent)
	: QToolBox(parent)
{
	setMinimumWidth(200);
	addGroup(tr("通用信息"));
	addGroup(tr("用户信息"));

	foreach(ReportItemCreator *creator, ItemFactory::instance()->creators())
	{
		if (!creator->group().isEmpty())
			addReportItem(creator);
	}
}

int ToolBox::addGroup(const QString &name)
{
	int i = findGroup(name);
	if (i >= 0)
		return i;

	QWidget *w = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->addStretch();
	w->setLayout(layout);

	i = addItem(w, name);
	return i;
}

int ToolBox::findGroup(const QString &name) const
{
	for(int i=0; i<count(); i++)
		if (itemText(i) == name)
			return i;
	return -1;
}

void ToolBox::addReportItem(ReportItemCreator *item)
{
	int i = addGroup(item->group());
	QWidget *w = widget(i);
	if (!w)
		return;

	QToolButton *button = new QToolButton(w);
	Actions *actions = qWorkbench->actions();
	QAction *action = actions->createAction(item, button);
	button->addAction(action);
	button->setDefaultAction(action);
	button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	button->setAutoRaise(true);
	QVBoxLayout *layout = static_cast<QVBoxLayout *>(w->layout());
	layout->insertWidget(layout->count()-1, button);
}

///////////////////////////////////////////////////////////////////////////////
// ReportDesigner

ReportDesigner::ReportDesigner(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, m_tool(0)
	, m_report(0)
	, m_inSelectTool(false)
{
	showMaximized();
	Actions *actions = qWorkbench->actions();

	m_view = new ReportView(this);
	setCentralWidget(m_view);

	// 工具箱
	QDockWidget *dockToolBox = new QDockWidget(tr("工具箱"), this);
	m_toolbox = new ToolBox(dockToolBox);
	dockToolBox->setWidget(m_toolbox);
	addDockWidget(Qt::LeftDockWidgetArea, dockToolBox);

	// 标准 工具栏
	m_general = addToolBar(tr("常用"));
	m_general->addAction(actions->fileNew);
	m_general->addAction(actions->fileOpen);
	m_general->addAction(actions->fileSave);
	m_general->addSeparator();
	m_general->addAction(actions->editUndo);
	m_general->addAction(actions->editRedo);
	m_general->addAction(actions->editCut);
	m_general->addAction(actions->editCopy);
	m_general->addAction(actions->editPaste);
	m_general->addSeparator();
	m_general->addAction(actions->toolArrow);
	m_general->addAction(actions->toolLine);
	m_general->addAction(actions->toolRect);
	m_general->addAction(actions->toolText);
	m_general->addAction(actions->toolImage);
	m_general->addSeparator();
	m_general->addAction(actions->penWidth);

	// 格式 工具栏
	m_fontCombo = new QFontComboBox();
	connect(m_fontCombo, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(currentFontChanged(const QString &)));

	m_fontSizeCombo = new QComboBox;
	m_fontSizeCombo->setEditable(true);
	QFontDatabase db;
	foreach(int size, db.standardSizes())
		m_fontSizeCombo->addItem(QString::number(size));
	QIntValidator *validator = new QIntValidator(2, 100, this);
	m_fontSizeCombo->setValidator(validator);
	m_fontSizeCombo->setCurrentIndex(db.standardSizes().indexOf(12));
	connect(m_fontSizeCombo, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(fontSizeChanged(const QString &)));

	m_scaleCombo = new QComboBox;
	m_scaleCombo->setEditable(true);
	m_scaleCombo->addItems(QStringList() << "50%" << "75%" << "100%" << "125%" << "150%");
	m_scaleCombo->setCurrentIndex(2);
	connect(m_scaleCombo, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(scaleChanged(const QString &)));

	m_format = addToolBar(tr("格式"));
	m_format->addWidget(m_fontCombo);
	m_format->addWidget(m_fontSizeCombo);
	m_format->addSeparator();
	m_format->addAction(actions->textBold);
	m_format->addAction(actions->textItalic);
	m_format->addAction(actions->textUnder);
	m_format->addSeparator();
	m_format->addActions(actions->textAlignGroup->actions());
	m_format->addWidget(m_scaleCombo);


	// 菜单栏
	QMenuBar *mb = menuBar();

	QMenu *file = mb->addMenu(tr("文件(&F)"));
	file->addAction(actions->fileNew);
	file->addAction(actions->fileOpen);
	file->addAction(actions->fileSave);
	file->addAction(actions->fileSaveAs);
	file->addSeparator();
	file->addAction(actions->filePrint);
	file->addAction(actions->fileSetupPage);
	file->addSeparator();
	for(int i=0; i<Actions::MaxRecentFiles; i++)
	{
		file->addAction(actions->recentFileActs[i]);
		connect(actions->recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}
	file->addSeparator();
	file->addAction(actions->appExit);

	QMenu *edit = mb->addMenu(tr("编辑(&E)"));
	edit->addAction(actions->editUndo);
	edit->addAction(actions->editRedo);
	edit->addSeparator();
	edit->addAction(actions->editCut);
	edit->addAction(actions->editCopy);
	edit->addAction(actions->editPaste);
	edit->addSeparator();
	edit->addAction(actions->editDelete);
	edit->addAction(actions->editSelectAll);

	QMenu *view = createPopupMenu();
	view->setTitle(tr("视图(&V)"));
	mb->addMenu(view);

#define CN(act, slot) connect(actions->act, SIGNAL(triggered()), this, SLOT(slot))
	CN(fileNew,			fileNew());
	CN(fileOpen,		open());
	CN(fileSave,		save());
	CN(fileSaveAs,		saveAs());
	CN(filePrint,		print());
	CN(fileSetupPage,	setupPage());
#undef CN

#define CN(act, slot) connect(actions->act, SIGNAL(toggled(bool)), this, SLOT(slot))
	CN(textBold,		fontBoldChanged(bool));
	CN(textItalic,		fontItalicChanged(bool));
	CN(textUnder,		fontUnderlineChanged(bool));
#undef CN

	connect(actions->textAlignGroup, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
	connect(actions->toolGroup, SIGNAL(triggered(QAction*)), this, SLOT(selectTool(QAction*)));
	connect(qWorkbench, SIGNAL(penChanged(const QPen &)), this, SLOT(penChanged(const QPen &)));
	connect(qWorkbench, SIGNAL(brushChanged(const QBrush &)), this, SLOT(brushChanged(const QBrush &)));

	connect(actions->editCut,		SIGNAL(triggered()), this, SLOT(cut()));
	connect(actions->editCopy,		SIGNAL(triggered()), this, SLOT(copy()));
	connect(actions->editPaste,		SIGNAL(triggered()), this, SLOT(paste()));
	connect(actions->editDelete,	SIGNAL(triggered()), this, SLOT(clear()));
	connect(actions->editSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));

	connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));

	updateRecentFileActions();

	fileNew();
	actions->toolArrow->trigger();
}

ReportDesigner::~ReportDesigner()
{
}

void ReportDesigner::setReport(Report *report)
{
	m_report = report;
	m_report->setDesignMode(true);
	m_report->undoStack()->setClean();
	m_view->setScene(m_report);

	Actions *actions = qWorkbench->actions();
	actions->editUndo->setEnabled(m_report->undoStack()->canUndo());
	actions->editRedo->setEnabled(m_report->undoStack()->canRedo());
	actions->fileSave->setDisabled(m_report->undoStack()->isClean());

	connect(m_report->undoStack(), SIGNAL(canUndoChanged(bool)), actions->editUndo, SLOT(setEnabled(bool)));
	connect(m_report->undoStack(), SIGNAL(canRedoChanged(bool)), actions->editRedo, SLOT(setEnabled(bool)));
	connect(actions->editUndo, SIGNAL(triggered()), m_report, SLOT(undo()));
	connect(actions->editRedo, SIGNAL(triggered()), m_report, SLOT(redo()));
	connect(m_report->undoStack(), SIGNAL(cleanChanged(bool)), actions->fileSave, SLOT(setDisabled(bool)));
	connect(m_report->undoStack(), SIGNAL(cleanChanged(bool)), this, SLOT(setModified(bool)));
	connect(m_report, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

	applyFontChange(false);
}

void ReportDesigner::fileNew()
{
	if (!saveModified())
		return;

	delete m_report;
	m_report = 0;

	setReport(new Report(this));
	setCurrentFile(QString());
}

void ReportDesigner::open()
{
	if (!saveModified())
		return;

	QString filename = QFileDialog::getOpenFileName(window(), QString(), m_lastDir,
		tr("报表文件(*.%1);;所有文件(*.*)").arg(Report::suffix));
	if (filename.isEmpty())
		return;

	loadFile(filename);
}

bool ReportDesigner::save()
{
	if (m_report->filename().isEmpty())
		return saveAs();

	return saveFile(m_report->filename());
}

bool ReportDesigner::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(window(), QString(),
		m_report->filename(), tr("报表文件(*.%1);;所有文件(*.*)").arg(Report::suffix));
	if (filename.isEmpty())
		return false;

	return saveFile(filename);
}

void ReportDesigner::print()
{
	QPrinter printer(QPrinter::HighResolution);
	m_report->setupPrinter(&printer);
	printer.setDocName(qApp->applicationName());
	printer.setPrintRange(QPrinter::AllPages);

	QPrintDialog dlg(&printer);
	dlg.setOption(QAbstractPrintDialog::PrintPageRange, false);

	if (dlg.exec() != QDialog::Accepted)
		return;

	QPainter painter(&printer);
	m_report->render(&painter);
}

void ReportDesigner::setupPage()
{
	QPrinter printer;
	printer.setPaperSize(m_report->paperSize());
	printer.setOrientation(m_report->orientation());
	if (m_report->paperSize() == QPrinter::Custom)
		printer.setPaperSize(m_report->paperSizeF(), QPrinter::Millimeter);

	QPageSetupDialog dlg(&printer, this);
	if (dlg.exec() != QDialog::Accepted)
		return;

	m_report->undoStack()->push(new SetupPageCommand(m_report, printer.paperSize(), 
		printer.orientation(), printer.paperSize(QPrinter::Millimeter)));
}

void ReportDesigner::selectTool(QAction *tool)
{
	if (m_inSelectTool)
		return;

	m_inSelectTool = true;
	ReportItemCreator *creator = reinterpret_cast<ReportItemCreator*>(tool->data().toInt());

	if (m_tool)
	{
		m_tool->leave();
		disconnect(m_tool, SIGNAL(created(ReportItem*)), this, SLOT(itemCreated(ReportItem*)));
		disconnect(m_tool, SIGNAL(finished(ReportItem*)), this, SLOT(createFinished(ReportItem*)));
	}
	m_tool = creator;
	if (m_tool)
	{
		connect(m_tool, SIGNAL(created(ReportItem*)), this, SLOT(itemCreated(ReportItem*)));
		connect(m_tool, SIGNAL(finished(ReportItem*)), this, SLOT(createFinished(ReportItem*)));
		m_tool->enter(m_view);
	}
	m_inSelectTool = false;
}

void ReportDesigner::currentFontChanged(const QString &fontFamily)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("字体"));

	foreach(QGraphicsItem* item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new FontFamilyCommand(reportItem, fontFamily, cmd);
	}

	if (cmd->childCount())
	{
		m_report->undoStack()->push(cmd);
	}
	else
	{
		QFont font = qWorkbench->font();
		font.setFamily(fontFamily);
		qWorkbench->setFont(font);
	}
}

void ReportDesigner::fontSizeChanged(const QString &size)
{
	if (!m_report)
		return;

	bool ok;
	const float pt = m_report->dpiY() / 72.f;
	const int pixelSize = qRound(size.toFloat(&ok) * pt);
	if (!ok)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("字号"));

	foreach(QGraphicsItem* item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new FontSizeCommand(reportItem, pixelSize, cmd);
	}

	if (cmd->childCount())
	{
		m_report->undoStack()->push(cmd);
	}
	else
	{
		QFont font = qWorkbench->font();
		font.setPixelSize(pixelSize);
		qWorkbench->setFont(font);
	}
}

void ReportDesigner::fontBoldChanged(bool checked)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("加粗"));

	foreach(QGraphicsItem* item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new FontBoldCommand(reportItem, checked, cmd);
	}

	if (cmd->childCount())
	{
		m_report->undoStack()->push(cmd);
	}
	else
	{
		QFont font = qWorkbench->font();
		font.setBold(checked);
		qWorkbench->setFont(font);
	}
}

void ReportDesigner::fontItalicChanged(bool checked)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("倾斜"));

	foreach(QGraphicsItem* item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new FontItalicCommand(reportItem, checked, cmd);
	}

	if (cmd->childCount())
	{
		m_report->undoStack()->push(cmd);
	}
	else
	{
		QFont font = qWorkbench->font();
		font.setItalic(checked);
		qWorkbench->setFont(font);
	}
}

void ReportDesigner::fontUnderlineChanged(bool checked)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("下划线"));

	foreach(QGraphicsItem* item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new FontUnderlineCommand(reportItem, checked, cmd);
	}

	if (cmd->childCount())
	{
		m_report->undoStack()->push(cmd);
	}
	else
	{
		QFont font = qWorkbench->font();
		font.setUnderline(checked);
		qWorkbench->setFont(font);
	}
}

void ReportDesigner::applyFontChange(bool effectReport)
{
	if (!m_report)
		return;

	const float pt = m_report->dpiY() / 72.f;
	Actions *actions = qWorkbench->actions();
	QFont font = m_fontCombo->currentFont();
	font.setPixelSize(qRound(m_fontSizeCombo->currentText().toFloat()*pt));
	font.setBold(actions->textBold->isChecked());
	font.setItalic(actions->textItalic->isChecked());
	font.setUnderline(actions->textUnder->isChecked());

	if (!effectReport || m_report->selectedItems().isEmpty())
	{
		qWorkbench->setFont(font);
	}
	else
	{
		foreach(QGraphicsItem *item, m_report->selectedItems())
		{
			ReportItem *reportItem = reportItem_cast(item);
			if (reportItem)
				reportItem->setFont(font);
		}
	}
}

void ReportDesigner::scaleChanged(const QString &scale)
{
	QTextStream is(const_cast<QString*>(&scale), QIODevice::ReadOnly);
	qreal s = 0;
	is >> s;
	s /= 100.0;

	if (s < 0.01 || s > 100.0)
		return;

	m_view->scaleView(s);
}

bool ReportDesigner::saveModified()
{
	if (!m_report)
		return true;

	if (m_report->undoStack()->isClean())
		return true;

	QMessageBox::StandardButton button = QMessageBox::question(window(), tr("保存"), 
		tr("是否保存当前文档?"), 
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
	if (button == QMessageBox::Cancel)
		return false;
	else if (button == QMessageBox::No)
		return true;

	return save();
}

void ReportDesigner::setLastDir(const QString &dir)
{
	QDir d(dir);
	m_lastDir = d.path();
}

void ReportDesigner::closeEvent(QCloseEvent *e)
{
	if (!saveModified())
		e->ignore();
}

void ReportDesigner::itemCreated(ReportItem *item)
{
	if (!item)
		return;

	item->setPen(qWorkbench->pen());
	item->setBrush(qWorkbench->brush());
	item->setFont(qWorkbench->font());
}

void ReportDesigner::createFinished(ReportItem *item)
{
	if (!m_inSelectTool)
		QMetaObject::invokeMethod(qWorkbench->actions()->toolArrow,
								  "trigger",
								  Qt::QueuedConnection);
}

void ReportDesigner::textAlign(QAction *align)
{
	if (!m_report)
		return;

	Actions *actions = qWorkbench->actions();
	Qt::Alignment alignment;
	if (align == actions->textLeft)
		alignment = (Qt::AlignLeft | Qt::AlignAbsolute);
	else if (align == actions->textCenter)
		alignment = (Qt::AlignHCenter);
	else if (align == actions->textRight)
		alignment = (Qt::AlignRight | Qt::AlignAbsolute);
	else if (align == actions->textJustify)
		alignment = (Qt::AlignJustify);

	QUndoCommand *cmd = new QUndoCommand(tr("对齐"));
	foreach(QGraphicsItem *item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new TextAlignmentCommand(reportItem, alignment, cmd);
	}

	if (cmd->childCount())
		m_report->undoStack()->push(cmd);
	else
		delete cmd;
}

void ReportDesigner::penChanged(const QPen &pen)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("画笔"));
	foreach(QGraphicsItem *item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new PenCommand(reportItem, pen, cmd);
	}

	if (cmd->childCount())
		m_report->undoStack()->push(cmd);
	else
		delete cmd;
}

void ReportDesigner::brushChanged(const QBrush &brush)
{
	if (!m_report)
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("刷子"));
	foreach(QGraphicsItem *item, m_report->selectedItems())
	{
		ReportItem *reportItem = reportItem_cast(item);
		if (reportItem)
			new BrushCommand(reportItem, brush, cmd);
	}

	if (cmd->childCount())
		m_report->undoStack()->push(cmd);
	else
		delete cmd;
}

void ReportDesigner::cut()
{
	copy();
	clear();
}

void ReportDesigner::copy()
{
	if (!m_report)
		return;

	QList<QGraphicsItem*> items = m_report->selectedItems();
	if (items.isEmpty())
		return;

	QList<ReportItem*> reportItems;
	foreach(QGraphicsItem *item, items)
	{
		ReportItem *ri = reportItem_cast(item);
		if (!ri)
			continue;
		reportItems << ri;
	}

	if (reportItems.isEmpty())
		return;

	QMimeData *mime = new QMimeData;

	QByteArray buffer;
	QDataStream os(&buffer, QIODevice::ReadWrite);
	os << reportItems;
	mime->setData(Report::mimeType, buffer);

	QClipboard *cb = QApplication::clipboard();
	cb->setMimeData(mime);
}

void ReportDesigner::paste()
{
	if (!m_report)
		return;

	QClipboard *cb = QApplication::clipboard();
	const QMimeData *mime = cb->mimeData();

	QList<ReportItem*> reportItems;
	if (mime->hasFormat(Report::mimeType))
	{
		const QByteArray data = mime->data(Report::mimeType);
		QDataStream is(data);
		is >> reportItems;
	}
	else if (mime->hasHtml() || mime->hasText())
	{
		ReportTextItem *item = new ReportTextItem;
		if (mime->hasHtml())
			item->setHtml(mime->html());
		else
			item->setPlainText(mime->text());
		reportItems << item;
	}
	else if (mime->hasImage())
	{
		ReportImageItem *item = new ReportImageItem;
		item->setPixmap(QPixmap::fromImage(qvariant_cast<QImage>(mime->imageData())));
		reportItems << item;
	}

	if (reportItems.isEmpty())
		return;

	// center items
	QList<QGraphicsItem*> items;
	foreach(ReportItem *ri, reportItems)
		items << ri->graphicsItem();
	QRectF boundingRect = items.front()->boundingRect();
	for(int i=1; i<items.size(); i++)
		boundingRect |= items[i]->boundingRect();

	QPointF center = m_view->mapToScene(m_view->rect().center());
	QPointF offset = center - boundingRect.center();
	if (offset.x() || offset.y())
	{
		foreach(QGraphicsItem *item, items)
			item->moveBy(offset.x(), offset.y());
	}

	QUndoCommand *cmd = new QUndoCommand(tr("粘贴"));
	foreach(ReportItem *ri, reportItems)
		new NewCommand(m_report, ri, cmd);
	m_report->undoStack()->push(cmd);

	m_report->clearSelection();
	foreach(ReportItem *ri, reportItems)
		ri->graphicsItem()->setSelected(true);
}

void ReportDesigner::clear()
{
	if (!m_report)
		return;

	QList<QGraphicsItem*> items = m_report->selectedItems();
	if (items.isEmpty())
		return;

	QUndoCommand *cmd = new QUndoCommand(tr("清除"));
	foreach(QGraphicsItem *item, items)
	{
		ReportItem *ri = reportItem_cast(item);
		if (!ri)
			continue;
		
		new DeleteCommand(ri, cmd);
	}

	if (cmd->childCount())
		m_report->undoStack()->push(cmd);
	else
		delete cmd;
}

void ReportDesigner::selectAll()
{
	if (!m_report)
		return;

	foreach(QGraphicsItem *item, m_report->items())
		item->setSelected(true);
}

void ReportDesigner::selectionChanged()
{
	Actions *actions = qWorkbench->actions();

	QList<QGraphicsItem*> items = m_report->selectedItems();
	actions->editCopy->setEnabled(!items.isEmpty());
	actions->editCut->setEnabled(!items.isEmpty());
	actions->editDelete->setEnabled(!items.isEmpty());

	// TODO: update font combobox contents
	QSet<QString> fontFamily;
	QSet<int> fontSize;
	QSet<bool> bold, italic, underline;
	foreach(QGraphicsItem* item, items)
	{
		ReportItem *ri = reportItem_cast(item);
		if (!ri)
			continue;

		QFont font = ri->font();
		if (font.isCopyOf(QFont()))
			continue;

		fontFamily += font.family();
		fontSize += font.pixelSize();
		bold += font.bold();
		italic += font.italic();
		underline += font.underline();
	}

	if (fontFamily.isEmpty())
	{
		QFont font;
		fontFamily += font.family();
		if (font.pixelSize() != -1)
			fontSize += font.pixelSize();
		else
			fontSize += qRound(m_report->dpiY() / 72.f * font.pointSize());
		bold += font.bold();
		italic += font.italic();
		underline += font.underline();
	}

	bool b = m_fontCombo->blockSignals(true);
	if (fontFamily.size() == 1)
	{
		QFont font;
		font.setFamily(*fontFamily.begin());
		m_fontCombo->setCurrentFont(font);
	}
	else
		m_fontCombo->setCurrentIndex(-1);
	m_fontCombo->blockSignals(b);

	b = m_fontSizeCombo->blockSignals(true);
	if (fontSize.size() == 1)
	{
		QString size = QString::number(qRound(*fontSize.begin()*72.f/m_report->dpiY()));
		int i = m_fontSizeCombo->findText(size);
		if (i < 0)
			m_fontSizeCombo->setEditText(size);
		else
			m_fontSizeCombo->setCurrentIndex(i);
	}
	else
		m_fontSizeCombo->setCurrentIndex(-1);
	m_fontSizeCombo->blockSignals(b);

	if (bold.size() == 1)
	{
		b = actions->textBold->blockSignals(true);
		actions->textBold->setChecked(*bold.begin());
		actions->textBold->blockSignals(b);
	}

	if (italic.size() == 1)
	{
		b = actions->textItalic->blockSignals(true);
		actions->textItalic->setChecked(*italic.begin());
		actions->textItalic->blockSignals(b);
	}

	if (underline.size() == 1)
	{
		b = actions->textUnder->blockSignals(true);
		actions->textUnder->setChecked(*underline.begin());
		actions->textUnder->blockSignals(b);
	}
}

void ReportDesigner::clipboardDataChanged()
{
	Actions *actions = qWorkbench->actions();

	QClipboard *cb = QApplication::clipboard();
	const QMimeData *mime = cb->mimeData();
	actions->editPaste->setEnabled(mime->hasText() 
		|| mime->hasHtml()
		|| mime->hasImage()
		|| mime->hasFormat(Report::mimeType));
}

void ReportDesigner::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void ReportDesigner::loadFile(const QString &filename)
{
	Report *report = new Report();
	if (!report->load(filename))
	{
		delete report;
		return;
	}

	report->setFileName(filename);
	report->undoStack()->setClean();

	delete m_report;
	m_report = 0;

	setReport(report);

	setLastDir(filename);
	setCurrentFile(filename);
}

bool ReportDesigner::saveFile(const QString &filename)
{
	m_report->setFileName(filename);
	setLastDir(filename);
	setCurrentFile(filename);

	if (m_report->save(filename))
	{
		m_report->undoStack()->setClean();
		return true;
	}

	return false;
}

void ReportDesigner::setCurrentFile(const QString &fileName)
{
	m_curFile = fileName;
	setWindowFilePath(m_curFile.isEmpty() ? tr("无标题") : m_curFile);

	if (!m_curFile.isEmpty())
	{
		QSettings settings;
		QStringList files = settings.value("recentFileList").toStringList();
		files.removeAll(fileName);
		files.prepend(fileName);
		while (files.size() > Actions::MaxRecentFiles)
			files.removeLast();

		settings.setValue("recentFileList", files);
		updateRecentFileActions();
	}
}

void ReportDesigner::updateRecentFileActions()
{
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)Actions::MaxRecentFiles);
	Actions *actions = qWorkbench->actions();

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		actions->recentFileActs[i]->setText(text);
		actions->recentFileActs[i]->setData(files[i]);
		actions->recentFileActs[i]->setStatusTip(files[i]);
		actions->recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < Actions::MaxRecentFiles; ++j)
		actions->recentFileActs[j]->setVisible(false);
}

QString ReportDesigner::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void ReportDesigner::setModified(bool m)
{
	setWindowModified(!m);
}
