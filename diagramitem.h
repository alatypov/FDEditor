#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QList>

#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QWidget>
#include <QPolygonF>

class Arrow;
class QString;

#include "ipad.h"
#include "iojson.h"

class DiagramScene;
class Line;
class LinePoint;

struct AttachedLinePoint {
    QPointF postionWithinTheItem;
    LinePoint *lp;
};

//! [0]
class DiagramItem : public QGraphicsPixmapItem, public IoJson
{
	friend class DiagramScene;
public:
    enum { Type = UserType + 15 };
    enum DiagramType { 
			Level0=LEVEL0, 
			Level1=LEVEL1, 
			Level2=LEVEL2, 
			Level3=3,
			Handbook=HANDBOOK, 
			HandbookGreen=HANDBOOK_GREEN,
			Decision=DECISION, 
			Storage=STORAGE,
			GrayBox=GRAYBOX,
			GroupBox=GROUPBOX,
			HandbookYellow=HANDBOOK_YELLOW,
			HandbookBlue=HANDBOOK_BLUE,
			UArrow = ARROW_UP,
			RArrow = ARROW_RIGHT,
			DArrow = ARROW_DOWN,
			LArrow = ARROW_LEFT
		};

    DiagramItem(DiagramType diagramType, QMenu *contextMenu,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    DiagramType diagramType() const
        { return myDiagramType; }
    QPainterPath path() const
        { return myPath; }

    //QPixmap image() const;
    int type() const
        {return Type;}

		void setSize(QSize sz);
		void setHeight(int h);
		void setWidth(int w);
		
		QString header, body, footer;
		int iosDiagramType;
		
        QJsonObject toJson();
        void fromJson(QJsonObject&);

		Q_SLOT void update();

        bool isArrow();
        Q_SLOT void updateLines();
        Q_SLOT void attachLinePoint(LinePoint *lp);

protected:

	int _height, _width;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    DiagramType myDiagramType;
    QPainterPath myPath;
    QMenu *myContextMenu;
    QList<AttachedLinePoint*> attachedLinePoints;
};
//! [0]

#endif
