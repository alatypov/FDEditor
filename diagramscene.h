#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "diagramtextitem.h"
#include <QMimeData>

#include "helper.h"

#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPointF>
#include <QGraphicsLineItem>
#include <QFont>
#include <QGraphicsTextItem>
#include <QColor>

class LinePoint;
class Line;

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertDashLine, InsertText, MoveItem, 
			InsertLinePoint, InsertArrow
		};

    DiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QFont font() const
        { return myFont; }
    QColor textColor() const
        { return myTextColor; }
    QColor itemColor() const
        { return myItemColor; }
    QColor lineColor() const
        { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

		void exportToJSON(QString &fileName);
		bool importFromJSON(QString &fileName);

		Helper helper;

		void loadDefaults(QString fileName);
		QString dbDirName;

		void loadHelperDbFromDir(QString dirName){
			helper.readDB(dirName);
		}

public:
	static QSize getDefaultSize(int forType);
	static void setDefaultSize(int forType, QSize sz);

	Q_SLOT void alignItemsByTop();
	Q_SLOT void alignItemsByButtom();
	Q_SLOT void alignItemsByRight();
	Q_SLOT void alignItemsByLeft();
	Q_SLOT void adjustItemSizes();

	Q_SLOT void copyItem();
	Q_SLOT void pasteItem();

	Q_SLOT void setSameDistHorizontal();
	Q_SLOT void setSameDistVertical();

    Q_SLOT void fixLines();
    Q_SLOT void fixArrows();
    Q_SLOT void makeCurrentLinePointAnArrow();

protected:
		void drawBackground(QPainter * painter, const QRectF & rect);

public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(DiagramTextItem *item);

public:
    Q_SIGNAL void itemInserted(DiagramItem *item);
		Q_SIGNAL void linePointInserted(LinePoint *lp);
    Q_SIGNAL void textInserted(QGraphicsTextItem *item);
    Q_SIGNAL void itemSelected(QGraphicsItem *item);

		static void setPrefixAndResetCounters(QString pref);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);

    DiagramItem::DiagramType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
		Arrow *arrow;
    QFont myFont;
    DiagramTextItem *textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;
    QList<QGraphicsItem *> clipboard;
    LinePoint *currentLinePoint;
};

#endif
