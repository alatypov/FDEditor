#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "diagramitem.h"

class DiagramScene;

#include <QtGui>
#include <QMenuBar>
#include <QAction>
#include <QToolBox>
#include <QSpinBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QButtonGroup>
#include <QGraphicsTextItem>
#include <QFont>
#include <QToolButton>
#include <QAbstractButton>
#include <QGraphicsView>
#include <QLineEdit>
#include <QUndoStack>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QVariant>
#include <QToolBar>

#include "diagramscene.h"
#include "infodialog.h"

#include <QSize>
#include <QMap>

//! [0]
class MainWindow : public QMainWindow
{
	friend class InfoDialog;

    Q_OBJECT

public:
   MainWindow();
	 ~MainWindow();

private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void sceneScaleChanged(const QString &scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);
    void about();
		void loadDataForSelectedItem();
		void saveDataForSelectedItem();

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              DiagramItem::DiagramType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *buildAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *editMenu;
    QMenu *DbMenu;

    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

		QLineEdit *nameEdit;
		DiagramItem *prevItem;

		QLineEdit *filePathEdit;
		QPushButton *chooseFileButton, *saveJSONButton, *loadJSONButton;
		InfoDialog *infoDialog;
		QAction *loadJSONDataToDialogAction, *saveJSONDataToItem, *importJSONFromFile;
		QAction *clearSceneAction;
		QString jsonFileName;
		QAction *showToolpadAction;
		QAction *newFileAction, *closeFileAction;

		QAction *loadHelperDbAction;
		QAction *reloadHelperDbAction;
		QAction *selectAllAction;

		QAction *alignByTop, *alignByLeft, *alignByBottom, *alignByRight;
		QAction *adjustSizes;
	
		QString jsonDB,defaultSizesJson;
		QUndoStack undoStack;

		QAction *copyAction, *pasteAction;
		QAction *sameDistHorizontal, *sameDistVertical;
        QAction *makeArrowAction;

public:

	DiagramScene *scene;
	QGraphicsView *view;

	Q_SLOT void chooseFile();
	Q_SLOT void saveJSON();
	Q_SLOT void importJSON();
	Q_SLOT void clearScene();
	Q_SLOT void loadHelperDb();
	Q_SLOT void showToolpad();
	Q_SLOT void reloadHelperDb();
	Q_SLOT void selectAllItems();
};
//! [0]

#endif
