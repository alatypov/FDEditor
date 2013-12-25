#include <QtGui>
#include <QLabel>

#include "mainwindow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "linepoint.h"

const int InsertTextButton = 10;

MainWindow *mainWindow;

#include "ipad.h"


//! [0]
MainWindow::MainWindow()
{
	createActions();
	createToolBox();
	createMenus();

	scene = new DiagramScene(itemMenu);
	
	editMenu->addSeparator();

	//align top
	alignByTop = new QAction(tr("Align by Top"), this);
	alignByTop->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Up));
	connect(alignByTop, SIGNAL(triggered()),
		scene, SLOT(alignItemsByTop()));
	editMenu->addAction(alignByTop);

	//align left
	alignByLeft = new QAction(tr("Align by Left"), this);
	alignByLeft->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Left));
	connect(alignByLeft, SIGNAL(triggered()),
		scene, SLOT(alignItemsByLeft()));
	editMenu->addAction(alignByLeft);

	//adjust size
	adjustSizes = new QAction(tr("Align by Left"), this);
	adjustSizes->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Equal));
	connect(adjustSizes, SIGNAL(triggered()),
		scene, SLOT(adjustItemSizes()));
	editMenu->addAction(adjustSizes);

	editMenu->addSeparator();

	QAction *undo, *redo;
	undo = new QAction(tr("Undo"), this);
	undo->setShortcut(QKeySequence::Undo);
	connect(undo,SIGNAL(triggered()),
		&undoStack,SLOT(undo()));
	editMenu->addAction(undo);
	
	redo = new QAction(tr("redo"), this);
	redo->setShortcut(QKeySequence::Redo);
	connect(redo,SIGNAL(triggered()),
		&undoStack,SLOT(redo()));
	editMenu->addAction(redo);

	copyAction = new QAction(tr("Copy Selected Items"), this);
	copyAction->setShortcut(Qt::CTRL+Qt::Key_C);
	connect(copyAction, SIGNAL(triggered()),
		scene, SLOT(copyItem()));

	pasteAction = new QAction(tr("Paste Items"), this);
	pasteAction->setShortcut(Qt::CTRL+Qt::Key_V);
	connect(pasteAction, SIGNAL(triggered()),
		scene, SLOT(pasteItem()));

	itemMenu->addSeparator();
	itemMenu->addAction(copyAction);
	itemMenu->addAction(pasteAction);

	sameDistHorizontal = new QAction(tr("Set same horizontal distance"), this);
	sameDistHorizontal->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_3);
	connect(sameDistHorizontal, SIGNAL(triggered()),
		scene, SLOT(setSameDistHorizontal()));

	sameDistVertical = new QAction(tr("Set same vertical distance"), this);
	sameDistVertical->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_3);
	connect(sameDistVertical, SIGNAL(triggered()),
		scene, SLOT(setSameDistVertical()));

	itemMenu->addSeparator();
	itemMenu->addAction(sameDistHorizontal);
	itemMenu->addAction(sameDistVertical);
	
	scene->setSceneRect(QRectF(0, 0, 5000, 5000));
	connect(scene, SIGNAL(itemInserted(DiagramItem *)),
		this, SLOT(itemInserted(DiagramItem *)));
	connect(scene, SIGNAL(textInserted(QGraphicsTextItem *)),
		this, SLOT(textInserted(QGraphicsTextItem *)));
	connect(scene, SIGNAL(itemSelected(QGraphicsItem *)),
		this, SLOT(itemSelected(QGraphicsItem *)));

    makeArrowAction = new QAction(tr("Make Arrow"),this);
    makeArrowAction->setShortcut(tr("Ctrl+M"));
    itemMenu->addAction(makeArrowAction);
    connect(makeArrowAction,SIGNAL(triggered()),scene,SLOT(makeCurrentLinePointAnArrow()));

	createToolbars();

	QHBoxLayout *mainLayout = new QHBoxLayout;

	view = new QGraphicsView(scene);
	view->setSceneRect(0,0,IPAD_WIDTH*2,IPAD_HEIGHT*2);
	view->setInteractive(true);
	view->setDragMode(QGraphicsView::RubberBandDrag);
	view->centerOn(IPAD_WIDTH, IPAD_HEIGHT);
	this->sceneScaleChanged("50%");

	QDockWidget *dockToolBox = new QDockWidget(tr("ToolBox"), this);
	dockToolBox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockToolBox->setWidget(toolBox);
	addDockWidget(Qt::LeftDockWidgetArea, dockToolBox);

	mainLayout->addWidget(view);

	QWidget *widget = new QWidget;
	widget->setLayout(mainLayout);

	infoDialog = new InfoDialog(this);
	//infoDialog->show();

	setCentralWidget(widget);
	setWindowTitle(tr("Diagram Draw JSON"));
	setUnifiedTitleAndToolBarOnMac(true);

	QSettings settings("Aziz U. Latypov", "Qt FD-Editor");

	//helper DB
	if(settings.value("default/jsonDB").toString().isEmpty()){
		settings.setValue("default/jsonDB", "./jsonDB/");
	}
	jsonDB = settings.value("default/jsonDB").toString();
	scene->helper.readDB(jsonDB);

	//default sizes
    //if(settings.value("default/sizesJson").toString().isEmpty()){
        settings.setValue("default/sizesJson", ":/defaults.json");
    //}
	defaultSizesJson = settings.value("default/sizesJson").toString();
	scene->loadDefaults(defaultSizesJson);
	settings.sync();
	
	jsonFileName = QString("chart.json");
	mainWindow = this;

	//addDockWidget(Qt::RightDockWidgetArea,infoDialog,Qt::Vertical);
	QDockWidget *dock = new QDockWidget(tr("Item Info"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setWidget(infoDialog);
	addDockWidget(Qt::RightDockWidgetArea, dock);
}

MainWindow::~MainWindow(){
	mainWindow=NULL;
}
//! [0]

//! [1]
void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
{
	QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
	foreach (QAbstractButton *myButton, buttons) {
		if (myButton != button)
			button->setChecked(false);
	}
	QString text = button->text();
	if (text == tr("Blue Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
	else if (text == tr("White Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
	else if (text == tr("Gray Grid"))
		scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
	else
		scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

	scene->update();
	view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id){
	QList<QAbstractButton *> buttons = buttonGroup->buttons();
	foreach (QAbstractButton *button, buttons) {
		if (buttonGroup->button(id) != button)
			button->setChecked(false);
	}
	if (id == InsertTextButton) {
		scene->setMode(DiagramScene::InsertText);
	} else {
		scene->setItemType(DiagramItem::DiagramType(id));
		scene->setMode(DiagramScene::InsertItem);
	}
}
//! [2]

//! [3]
void MainWindow::deleteItem()
{
	foreach (QGraphicsItem *item, scene->selectedItems()) {
		scene->removeItem(item);
	}
}
//! [3]

//! [4]
void MainWindow::pointerGroupClicked(int)
{
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void MainWindow::bringToFront()
{
	if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem *selectedItem = scene->selectedItems().first();
	QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	foreach (QGraphicsItem *item, overlapItems) {
		if (item->zValue() >= zValue &&
			item->type() == DiagramItem::Type)
			zValue = item->zValue() + 0.1;
	}
	selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void MainWindow::sendToBack()
{
	if (scene->selectedItems().isEmpty())
		return;

	QGraphicsItem *selectedItem = scene->selectedItems().first();
	QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

	qreal zValue = 0;
	foreach (QGraphicsItem *item, overlapItems) {
		if (item->zValue() <= zValue &&
			item->type() == DiagramItem::Type)
			zValue = item->zValue() - 0.1;
	}
	selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramItem *item)
{
	pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
	buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void MainWindow::textInserted(QGraphicsTextItem *)
{
	buttonGroup->button(InsertTextButton)->setChecked(false);
	scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(const QFont &)
{
	handleFontChange();
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(const QString &)
{
	handleFontChange();
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
	double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
	QMatrix oldMatrix = view->matrix();
	view->resetMatrix();
	view->translate(oldMatrix.dx(), oldMatrix.dy());
	view->scale(newScale, newScale);
}
//! [11]

//! [12]
void MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
	fontColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/textpointer.png",
        qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
	fillColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/floodfill.png",
        qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
	lineColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/linecolor.png",
        qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange()
{
	QFont font = fontCombo->currentFont();
	font.setPointSize(fontSizeCombo->currentText().toInt());
	font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
	font.setItalic(italicAction->isChecked());
	font.setUnderline(underlineAction->isChecked());

	scene->setFont(font);
}
//! [18]

//! [19]
void MainWindow::itemSelected(QGraphicsItem *item)
{
	DiagramTextItem *textItem =
		qgraphicsitem_cast<DiagramTextItem *>(item);

	QFont font = textItem->font();
    //QColor color = textItem->defaultTextColor();
	fontCombo->setCurrentFont(font);
	fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
	boldAction->setChecked(font.weight() == QFont::Bold);
	italicAction->setChecked(font.italic());
	underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
	/*QMessageBox::about(this, tr("About Diagram Scene"),
	tr("The <b>Diagram Scene</b> example shows "
	"use of the graphics framework."));*/
	scene->exportToJSON(jsonFileName);
}
//! [20]

//! [21]
void MainWindow::createToolBox()
{
	buttonGroup = new QButtonGroup;
	buttonGroup->setExclusive(false);
	connect(buttonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(buttonGroupClicked(int)));
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(createCellWidget(tr("Level 0"),
		DiagramItem::Level0), 0, 0);
	layout->addWidget(createCellWidget(tr("Level 1"),
		DiagramItem::Level1), 0, 1);
	layout->addWidget(createCellWidget(tr("Level 2"),
		DiagramItem::Level2), 0, 2);
	layout->addWidget(createCellWidget(tr("Decision"),
		DiagramItem::Decision),1, 0);
	layout->addWidget(createCellWidget(tr("Storage"),
		DiagramItem::Storage), 1, 1);
	layout->addWidget(createCellWidget(tr("Handbook"),
		DiagramItem::Handbook), 1, 2);
	layout->addWidget(createCellWidget(tr("Gray Box"),
		DiagramItem::GrayBox), 2, 0);

	layout->addWidget(createCellWidget(tr("Group Box"),
		DiagramItem::GroupBox), 2, 1);
	layout->addWidget(createCellWidget(tr("Green Handbook"),
		DiagramItem::HandbookGreen), 2, 2);
	layout->addWidget(createCellWidget(tr("Yello Handbook"),
		DiagramItem::HandbookYellow), 3, 2);

	layout->addWidget(createCellWidget(tr("Up Arrow"),
		DiagramItem::UArrow), 3, 1);
	layout->addWidget(createCellWidget(tr("Down Arrow"),
		DiagramItem::DArrow), 4, 1);
	layout->addWidget(createCellWidget(tr("Right Arrow"),
		DiagramItem::RArrow), 4, 2);
	layout->addWidget(createCellWidget(tr("Left Arrow"),
		DiagramItem::LArrow), 4, 0);

	layout->addWidget(createCellWidget(tr("Group Box"),
		DiagramItem::GroupBox), 5, 0);
	//! [21]
	
	QToolButton *textButton = new QToolButton;
	textButton->setCheckable(true);
	buttonGroup->addButton(textButton, InsertTextButton);
	textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")
	.scaled(30, 30)));
	textButton->setIconSize(QSize(50, 50));
	QGridLayout *textLayout = new QGridLayout;
	textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
	textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
	QWidget *textWidget = new QWidget;
	textWidget->setLayout(textLayout);
	layout->addWidget(textWidget, 3, 0);

	//layout->setRowStretch(3, 10);
	//layout->setColumnStretch(2, 10);

	QWidget *itemWidget = new QWidget;
	itemWidget->setLayout(layout);
/*
	backgroundButtonGroup = new QButtonGroup;
	connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton *)),
		this, SLOT(backgroundButtonGroupClicked(QAbstractButton *)));

	QGridLayout *backgroundLayout = new QGridLayout;
	backgroundLayout->addWidget(createBackgroundCellWidget(tr("Line"),
		":/images/linepointer.png"), 0, 0);

	backgroundLayout->setRowStretch(2, 10);
	backgroundLayout->setColumnStretch(2, 10);

	QWidget *backgroundWidget = new QWidget;
	backgroundWidget->setLayout(backgroundLayout);
*/

	//! [22]
	toolBox = new QToolBox;
	//toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	//toolBox->setMinimumWidth(itemWidget->sizeHint().width());
	toolBox->addItem(itemWidget, tr("Flowchart Shapes"));
    //toolBox->addItem(backgroundWidget, tr("Flowchart Lines"));
}
//! [22]

//! [23]
void MainWindow::createActions()
{
	toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
		tr("Bring to &Front"), this);
	toFrontAction->setShortcut(tr("Ctrl+F"));
	toFrontAction->setStatusTip(tr("Bring item to front"));
	connect(toFrontAction, SIGNAL(triggered()),
		this, SLOT(bringToFront()));
	//! [23]

	sendBackAction = new QAction(QIcon(":/images/sendtoback.png"),
		tr("Send to &Back"), this);
	sendBackAction->setShortcut(tr("Ctrl+B"));
	sendBackAction->setStatusTip(tr("Send item to back"));
	connect(sendBackAction, SIGNAL(triggered()),
		this, SLOT(sendToBack()));

	deleteAction = new QAction(QIcon(":/images/delete.png"),
		tr("&Delete"), this);
	deleteAction->setShortcut(tr("Delete"));
	deleteAction->setStatusTip(tr("Delete item from diagram"));
	connect(deleteAction, SIGNAL(triggered()),
		this, SLOT(deleteItem()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setStatusTip(tr("Quit Scenediagram example"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	boldAction = new QAction(tr("Bold"), this);
	boldAction->setCheckable(true);
	QPixmap pixmap(":/images/bold.png");
	boldAction->setIcon(QIcon(pixmap));
	boldAction->setShortcut(tr("Ctrl+B"));
	connect(boldAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	italicAction = new QAction(QIcon(":/images/italic.png"),
		tr("Italic"), this);
	italicAction->setCheckable(true);
	italicAction->setShortcut(tr("Ctrl+I"));
	connect(italicAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	underlineAction = new QAction(QIcon(":/images/underline.png"),
		tr("Underline"), this);
	underlineAction->setCheckable(true);
	underlineAction->setShortcut(tr("Ctrl+U"));
	connect(underlineAction, SIGNAL(triggered()),
		this, SLOT(handleFontChange()));

	buildAction = new QAction(tr("Build"), this);
    buildAction->setShortcut(tr("Ctrl+Shift+B"));
	connect(buildAction, SIGNAL(triggered()),
		this, SLOT(about()));
	
	loadJSONDataToDialogAction = new QAction(tr("Load Object Info"), this);
	loadJSONDataToDialogAction->setShortcut(tr("Ctrl+R"));
	connect(loadJSONDataToDialogAction, SIGNAL(triggered()),
		this, SLOT(loadDataForSelectedItem()));
	
	saveJSONDataToItem = new QAction(tr("Save Object Info"), this);
	saveJSONDataToItem->setShortcut(tr("Ctrl+S"));
	connect(saveJSONDataToItem, SIGNAL(triggered()),
		this, SLOT(saveDataForSelectedItem()));

	importJSONFromFile = new QAction(tr("Import JSON From File"), this);
	importJSONFromFile->setShortcut(tr("Ctrl+O"));
	connect(importJSONFromFile, SIGNAL(triggered()),
		this, SLOT(importJSON()));
	
	clearSceneAction = new QAction(tr("Clear Scene"), this);
	connect(clearSceneAction, SIGNAL(triggered()),
		this, SLOT(clearScene()));

	showToolpadAction = new QAction(tr("Show Toolpad"), this);
	showToolpadAction->setShortcut(tr("Ctrl+P"));
	connect(showToolpadAction, SIGNAL(triggered()),
		this, SLOT(showToolpad()));

	loadHelperDbAction = new QAction(tr("Load DB"), this);
	loadHelperDbAction->setShortcut(tr("Ctrl+L"));
	connect(loadHelperDbAction, SIGNAL(triggered()),
		this, SLOT(loadHelperDb()));

	selectAllAction = new QAction(tr("Select All Items"),this);
	selectAllAction->setShortcut(Qt::CTRL+Qt::Key_A);
	connect(selectAllAction, SIGNAL(triggered()),
		this, SLOT(selectAllItems()));

	reloadHelperDbAction = new QAction(tr("Reload DB"), this);
	reloadHelperDbAction->setShortcut(Qt::CTRL+Qt::Key_0);
	connect(reloadHelperDbAction, SIGNAL(triggered()),
		this, SLOT(reloadHelperDb()));
}

void MainWindow::loadDataForSelectedItem(){
	QList<QGraphicsItem*> its = scene->selectedItems();
	if(its.size()==0)
		return;
	QGraphicsItem *it = its[0];
	DiagramItem *item = dynamic_cast<DiagramItem*>(it);
	if(item==NULL)
		return;
	//do smth
	infoDialog->setInfo(item);
}

void MainWindow::saveDataForSelectedItem(){
	QList<QGraphicsItem*> its = scene->selectedItems();
	if(its.size()==0)
		return;
	QGraphicsItem *it = its[0];
	DiagramItem *item = dynamic_cast<DiagramItem*>(it);
	if(item==NULL)
		return;
	//do smth
	item->header = infoDialog->headerEdit.text();
	item->body = infoDialog->bodyEdit.text();
	//item->footer = infoDialog->footerEdit.text();
}

//! [24]
void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addSeparator();

	fileMenu->addAction(buildAction);
	fileMenu->addSeparator();
	fileMenu->addAction(loadJSONDataToDialogAction);
	fileMenu->addAction(saveJSONDataToItem);
	fileMenu->addAction(importJSONFromFile);
	fileMenu->addAction(clearSceneAction);
	fileMenu->addSeparator();
	fileMenu->addAction(loadHelperDbAction);
	fileMenu->addAction(reloadHelperDbAction);

	editMenu = menuBar()->addMenu(tr("Edit"));
	editMenu->addAction(selectAllAction);

	itemMenu = menuBar()->addMenu(tr("&Item"));
	itemMenu->addAction(deleteAction);
	itemMenu->addSeparator();
	itemMenu->addAction(toFrontAction);
	itemMenu->addAction(sendBackAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
	//! [25]
	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(deleteAction);
	editToolBar->addAction(toFrontAction);
	editToolBar->addAction(sendBackAction);

	fontCombo = new QFontComboBox();
	fontSizeCombo = new QComboBox();
	connect(fontCombo, SIGNAL(currentFontChanged(const QFont &)),
		this, SLOT(currentFontChanged(const QFont &)));

	fontSizeCombo = new QComboBox;
	fontSizeCombo->setEditable(true);
	for (int i = 8; i < 30; i = i + 2)
		fontSizeCombo->addItem(QString().setNum(i));
	QIntValidator *validator = new QIntValidator(2, 64, this);
	fontSizeCombo->setValidator(validator);
	connect(fontSizeCombo, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(fontSizeChanged(const QString &)));

	fontColorToolButton = new QToolButton;
	fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
		Qt::black));
	textAction = fontColorToolButton->menu()->defaultAction();
	fontColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/textpointer.png", Qt::black));
	fontColorToolButton->setAutoFillBackground(true);
	connect(fontColorToolButton, SIGNAL(clicked()),
		this, SLOT(textButtonTriggered()));

	//! [26]
	fillColorToolButton = new QToolButton;
	fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()),
		Qt::white));
	fillAction = fillColorToolButton->menu()->defaultAction();
	fillColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/floodfill.png", Qt::white));
	connect(fillColorToolButton, SIGNAL(clicked()),
		this, SLOT(fillButtonTriggered()));
	//! [26]

	lineColorToolButton = new QToolButton;
	lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
	lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()),
		Qt::black));
	lineAction = lineColorToolButton->menu()->defaultAction();
	lineColorToolButton->setIcon(createColorToolButtonIcon(
		":/images/linecolor.png", Qt::black));
	connect(lineColorToolButton, SIGNAL(clicked()),
		this, SLOT(lineButtonTriggered()));

	textToolBar = addToolBar(tr("Font"));
	textToolBar->addWidget(fontCombo);
	textToolBar->addWidget(fontSizeCombo);
	textToolBar->addAction(boldAction);
	textToolBar->addAction(italicAction);
	textToolBar->addAction(underlineAction);

	colorToolBar = addToolBar(tr("Color"));
	colorToolBar->addWidget(fontColorToolButton);
	colorToolBar->addWidget(fillColorToolButton);
	colorToolBar->addWidget(lineColorToolButton);

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(QIcon(":/images/pointer.png"));

	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

	QToolButton *linePointerButton2 = new QToolButton;
	linePointerButton2->setCheckable(true);
	linePointerButton2->setIcon(QIcon(":/images/linepointer2.png"));

	pointerTypeGroup = new QButtonGroup;
	pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));

	pointerTypeGroup->addButton(linePointerButton,
		int(DiagramScene::InsertLine));

	pointerTypeGroup->addButton(linePointerButton2,
		int(DiagramScene::InsertDashLine));

	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(pointerGroupClicked(int)));

	sceneScaleCombo = new QComboBox;
	QStringList scales;
	scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%") << tr("200%") << tr("250%") << tr("300%") << tr("350%") << tr("400%");
	sceneScaleCombo->addItems(scales);
	sceneScaleCombo->setCurrentIndex(2);
	connect(sceneScaleCombo, SIGNAL(currentIndexChanged(const QString &)),
		this, SLOT(sceneScaleChanged(const QString &)));

	pointerToolbar = addToolBar(tr("Pointer type"));
	pointerToolbar->addWidget(pointerButton);
	pointerToolbar->addWidget(linePointerButton);
	pointerToolbar->addWidget(linePointerButton2);
	pointerToolbar->addWidget(sceneScaleCombo);
	//! [27]
}
//! [27]

//! [28]
QWidget *MainWindow::createBackgroundCellWidget(const QString &text,
																								const QString &image)
{
	QToolButton *button = new QToolButton;
	button->setText(text);
	button->setIcon(QIcon(image));
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	backgroundButtonGroup->addButton(button);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}
//! [28]

//! [29]
QWidget *MainWindow::createCellWidget(const QString &text,
																			DiagramItem::DiagramType type)
{

	DiagramItem item(type, itemMenu);
	QIcon icon(item.pixmap());

	QToolButton *button = new QToolButton;
	button->setIcon(icon);
	button->setIconSize(QSize(50, 50));
	button->setCheckable(true);
	buttonGroup->addButton(button, int(type));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(button, 0, 0, Qt::AlignHCenter);
	layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	return widget;
}

//! [30]
QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor)
{
	QList<QColor> colors;
	colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
	QStringList names;
	names << tr("black") << tr("white") << tr("red") << tr("blue")
		<< tr("yellow");

    QMenu *colorMenu = new QMenu;
	for (int i = 0; i < colors.count(); ++i) {
		QAction *action = new QAction(names.at(i), this);
		action->setData(colors.at(i));
		action->setIcon(createColorIcon(colors.at(i)));
		connect(action, SIGNAL(triggered()),
			this, slot);
		colorMenu->addAction(action);
		if (colors.at(i) == defaultColor) {
			colorMenu->setDefaultAction(action);
		}
	}
	return colorMenu;
}
//! [30]

//! [31]
QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile,
																						QColor color)
{
	QPixmap pixmap(50, 80);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPixmap image(imageFile);
	QRect target(0, 0, 50, 60);
	QRect source(0, 0, 42, 42);
	painter.fillRect(QRect(0, 60, 50, 80), color);
	painter.drawPixmap(target, image, source);

	return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon MainWindow::createColorIcon(QColor color)
{
	QPixmap pixmap(20, 20);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.fillRect(QRect(0, 0, 20, 20), color);

	return QIcon(pixmap);
}
//! [32]


void MainWindow::chooseFile(){
	QString filePath = QFileDialog::getOpenFileName();
	if(!filePath.isEmpty())
		filePathEdit->setText(filePath);
}

void MainWindow::saveJSON(){
	QString filePath = filePathEdit->text();
	if(!filePath.isEmpty())
		scene->exportToJSON(filePath);
}

void MainWindow::importJSON(){
    jsonFileName = QFileDialog::getOpenFileName(this,tr("Choose JSON File to Import"),
                                                "/Users/alatypov/Documents/",
                                                tr("*.json"));
	if(!jsonFileName.isEmpty()){
		if(scene->importFromJSON(jsonFileName)){
			QString pref = jsonFileName.section("/",-1,-1);
			pref.chop(5);
			DiagramScene::setPrefixAndResetCounters(pref);
			this->setWindowTitle(tr("Drow Diagram JSON: %1").arg(jsonFileName));
		}
	}
}

void MainWindow::clearScene(){
	scene->clear();
}

void MainWindow::loadHelperDb(){
	QString dirName = QFileDialog::getExistingDirectory();
	if(!dirName.isEmpty()){
		jsonDB = dirName;
		reloadHelperDb();
	}
}

void MainWindow::showToolpad(){
}

void MainWindow::reloadHelperDb(){
	if(!jsonDB.isEmpty()){
		scene->loadHelperDbFromDir(jsonDB);
		infoDialog->headers.setStringList(scene->helper.headers.toList());
		infoDialog->bodies.setStringList(scene->helper.bodies.toList());
		infoDialog->footers.setStringList(scene->helper.footers.toList());
		infoDialog->db = scene->helper.db;
	}
}

void MainWindow::selectAllItems(){
	QPainterPath area;
	area.addRect(0,0,3000,3000);
	scene->setSelectionArea(area);
}
