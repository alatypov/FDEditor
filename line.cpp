#include <QtGui>

#include "line.h"
#include "linepoint.h"
#include "ipad.h"
#include <math.h>
#include "diagramscene.h"

const qreal Pi = 3.14;

//! [0]
Line::Line(LinePoint *startItem, LinePoint *endItem, int mode,
					 QGraphicsItem *parent, QGraphicsScene *scene)
                     : QGraphicsLineItem(parent)
{
	myStartItem = startItem;
	myEndItem = endItem;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor = Qt::black;
	
	if(mode==DiagramScene::InsertLine){
		setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		iosDiagramType = LINE;
	}else{
		setPen(QPen(myColor, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
		iosDiagramType = DASH_LINE;
	}
}

void Line::updatePosition()
{
	QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
	/*QPointF p1,p2;
	p1 = this->line().p1();
	p2 = this->line().p2();
	if((p1.x()==p2.x())||(p1.y()==p2.y())){
		setPen(QPen(Qt::black));
	}else{
		setPen(QPen(Qt::red));
	}*/
	setLine(line);
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
								 QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

	//QPen myPen = pen();
	//myPen.setColor(myColor);
	//painter->setPen(myPen);
	//painter->setBrush(myColor);

	//qreal arrowSize = 20;

	QLineF centerLine(myStartItem->pos(), myEndItem->pos());
	QPolygonF endPolygon = myEndItem->boundingRect();
	QPointF p1 = endPolygon.first() + myEndItem->pos();
	QPointF p2;
	QPointF intersectPoint;
	QLineF polyLine;
	for (int i = 1; i < endPolygon.count(); ++i){
		p2 = endPolygon.at(i) + myEndItem->pos();
		polyLine = QLineF(p1, p2);
		QLineF::IntersectType intersectType =
			polyLine.intersect(centerLine, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection)
			break;
		p1 = p2;
	}

	setLine(QLineF(intersectPoint, myStartItem->pos()));

	/*p1 = this->line().p1();
	p2 = this->line().p2();
	if((p1.x()==p2.x())||(p1.y()==p2.y())){
		setPen(QPen(Qt::black));
	}else{
		setPen(QPen(Qt::red));
	}*/
	
	/*double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (Pi * 2) - angle;

	QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
		cos(angle + Pi / 3) * arrowSize);
	QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
		cos(angle + Pi - Pi / 3) * arrowSize);*/

	//arrowHead.clear();
	//arrowHead << line().p1() << arrowP1 << arrowP2;
	painter->drawLine(line());
	/*painter->drawPolygon(arrowHead);
	if (isSelected()) {
		painter->setPen(QPen(myColor, 1, Qt::DashLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0,-8.0);
		painter->drawLine(myLine);
	}*/
}

QJsonObject Line::toJson(){
    QJsonObject obj;
    QJsonObject pt1, pt2;
	QPointF p1,p2;
    int x1, x2, y1, y2;

    LinePoint *s,*e;
    s = this->startItem();
    e = this->endItem();
    QRectF a,b;
    a = s->rect();
    b = e->rect();
    p1 = s->mapToScene(a.center());
    p2 = e->mapToScene(b.center());

    x1 = p1.x();
    y1 = p1.y();
    x2 = p2.x();
    y2 = p2.y();

    pt1["x"] = x1;
    pt1["y"] = y1;
    pt2["x"] = x2;
    pt2["y"] = y2;

    if(s->isArrow())
        pt1["isArrow"] = true;

    if(e->isArrow())
        pt2["isArrow"] = true;

    obj["type"] = (int)iosDiagramType;
	obj["start"] = pt1;
	obj["end"] = pt2;
    return obj;
}

void Line::fromJson(QJsonObject &params){
    int x1,x2,y1,y2;
    QPointF pt1,pt2;
    iosDiagramType = params["type"].toInt();
    x1 = params["start"].toObject()["x"].toInt();
    y1 = params["start"].toObject()["y"].toInt();
    x2 = params["end"].toObject()["x"].toInt();
    y2 = params["end"].toObject()["y"].toInt();

    //fix stupid lines
    /*if(abs(x1-x2)<=2){
        x1=x2;
        if(y1>y2) {y1+=1;y2-=1;}
        else {y2+=1;y1-=1;}
    }
    if(abs(y1-y2)<=2){
        y1=y2;
        if(x1>x2) {x1+=1;x2-=1;}
        else {x2+=1;x1-=1;}
    }*/

    pt1.setX(x1);
    pt1.setY(y1);
    pt2.setX(x2);
    pt2.setY(y2);

    myStartItem = new LinePoint(),
    myEndItem = new LinePoint();

    if(params["start"].toObject()["isArrow"].toBool()){
        myStartItem->setIsArrow(true);
    }

    if(params["end"].toObject()["isArrow"].toBool()){
        myEndItem->setIsArrow(true);
    }

    scene()->addItem(myStartItem);
    scene()->addItem(myEndItem);
    myStartItem->setPos(pt1);
    myEndItem->setPos(pt2);

    if(iosDiagramType == LINE){
        setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }else{
        setPen(QPen(myColor, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    }


    myStartItem->addLine(this);
    myEndItem->addLine(this);
    setZValue(-1000.0);
    scene()->addItem(this);
    updatePosition();
}
