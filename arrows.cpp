#include <QtGui>

#include "arrows.h"
#include "ipad.h"
#include <QPen>

QJsonObject Arrow::toJson(){
    QJsonObject obj;
    obj["type"] = QJsonValue(iosDiagramType);
	QRectF rect = mapRectToScene(boundingRect());
    obj["x"] = QJsonValue((int)rect.x());
    obj["y"] = QJsonValue((int)rect.y());
    obj["height"] = QJsonValue((int)8);
    obj["width"] = QJsonValue((int)8);
    return obj;
}

void Arrow::fromJson(QJsonValue &params){
    /*QJsonObject obj = params.object();
	int x,y;
	iosDiagramType = params.get("type",ARROW_UP).asInt();
	x = params.get("x",100).asInt();
	y = params.get("y",100).asInt();
	buildPath();
    setPos(QPointF(x+4,y+4));*/
}

void Arrow::buildPath(){
	int w = 8, h = 8, hh;
	QPen pen;
	myPath = QPainterPath();
	switch (myDiagramType) {
				case uarrow:
					iosDiagramType = ARROW_UP;
					myPath.moveTo(-4,-4);
					myPath.lineTo(4,-4);
					myPath.lineTo(0,4);
					myPath.lineTo(-4,-4);
					break;
				case rarrow:
					iosDiagramType = ARROW_RIGHT;
					//myPath.addRect(-w/2,-h/2,w,h);
					break;
				case darrow:
					iosDiagramType = ARROW_DOWN;
					//myPath.addRect(-w/2,-h/2,w,h);
					break;
				case larrow:
					iosDiagramType = ARROW_LEFT;
					//myPath.addRect(-w/2,-h/2,w,h);
					break;
				default:
					break;
	}
	this->setPen(pen);
	setPath(myPath);
}
//! [0]
Arrow::Arrow(DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPathItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    buildPath();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}
//! [0]


//! [4]
QPixmap Arrow::image() const
{
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));
    painter.translate(8, 8);
    painter.drawPath(myPath);
    return pixmap;
}
//! [4]

//! [5]
void Arrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
