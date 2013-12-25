#ifndef ARROWS_H
#define ARROWS_H

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
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsItem>
#include <QWidget>
#include <QPolygonF>

class Arrow;
class QString;

#include "ipad.h"
#include "iojson.h"

//! [0]
class Arrow : public QGraphicsPathItem, public IoJson
{
public:
    enum { Type = UserType + 75 };
    enum DiagramType { 
			uarrow=ARROW_UP, 
			rarrow=ARROW_RIGHT, 
			darrow=ARROW_DOWN, 
			larrow=ARROW_LEFT
		};

    Arrow(DiagramType diagramType, QMenu *contextMenu,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QPixmap image() const;
    int type() const
        { return Type;}
		DiagramType diagramType() const
		{ return myDiagramType; }

		void buildPath();
		
		QString header, body, footer;
		int iosDiagramType;
		
        QJsonObject toJson();
        void fromJson(QJsonValue&);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    DiagramType myDiagramType;
    QPainterPath myPath;
    QMenu *myContextMenu;
};
//! [0]

#endif
