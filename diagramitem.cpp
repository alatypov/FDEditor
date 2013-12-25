#include <QtGui>

#include "diagramitem.h"
#include "diagramscene.h"
#include "ipad.h"
#include <QPen>
#include "mainwindow.h"
#include "line.h"
#include "linepoint.h"

extern MainWindow *mainWindow;

QJsonObject DiagramItem::toJson(){
    QJsonObject obj;
    QRectF rect = mapRectToScene(boundingRect());
    int x,y,h,w;
    x = rect.x();
    y = rect.y();
    h = rect.height();
    w = rect.width();

    obj["type"] = QJsonValue((int)iosDiagramType);
    if(!header.isEmpty())
        obj["header"] = QJsonValue(header.trimmed());
    if(!body.isEmpty())
        obj["body"] = QJsonValue(body.trimmed());
    if(!footer.isEmpty())
        obj["footer"] = QJsonValue(footer.trimmed());

    obj["x"] = QJsonValue(x);
    obj["y"] = QJsonValue(y);
    obj["height"] = QJsonValue(h);
    obj["width"] = QJsonValue(w);
    return obj;
}

void DiagramItem::fromJson(QJsonObject &params){
	int x,y;
    x = params["x"].toInt()+1;
    y = params["y"].toInt()+1;
    _height = params["height"].toInt();
    _width = params["width"].toInt();
    header = params["header"].toString();
    body = params["body"].toString();
    footer = params["footer"].toString();
	update();
	setPos(QPointF(x,y));
}

void DiagramItem::setSize(QSize sz){
	_height = sz.height();
	_width = sz.width();
	update();
}
void DiagramItem::setHeight(int h){
	_height = h;
	update();
}
void DiagramItem::setWidth(int w){
	_width = w;
	update();
}

void DiagramItem::update(){
    myPath = QPainterPath();
	iosDiagramType = myDiagramType;
	QPixmap pixmap;
	QImage image;
	QString imageName;
	switch (myDiagramType) {
				case Level0:
					//this->setBrush(QBrush(Qt::blue));
					//myPath.addRect(-w/2,-h/2,w,h);
					imageName = QString(":/images/level0.png");
					break;
				case Level1:
					//this->setBrush(QBrush(Qt::green));
					//myPath.addRect(-w/2,-h/2,w,h);
					imageName = QString(":/images/level1.png");
					break;
				case Level2:
					//this->setBrush(QBrush(Qt::red));
					//myPath.addRect(-w/2,-h/2,w,h);
					imageName = QString(":/images/level2.png");
					break;
				case Level3:
					//this->setBrush(QBrush(Qt::red));
					//myPath.addRect(-w/2,-h/2,w,h);
					imageName = QString(":/images/level2.png");
					break;
				case GrayBox:
					//this->setBrush(QBrush(Qt::lightGray));
					//myPath.addRect(-w/2,-h/2,w,h);
					imageName = QString(":/images/level0.png");
					break;
				case Decision:
					imageName = QString(":/images/decision.png");
					//myPath.moveTo(-w/2,0);
					//myPath.lineTo(0,h/2);
					//myPath.lineTo(w/2,0);
					//myPath.lineTo(0,-h/2);
					//myPath.lineTo(-w/2,0);
					//this->setBrush(QBrush(Qt::cyan));
					break;
					
				case Handbook:
				case HandbookBlue:
					imageName = QString(":/images/handbook.png");
					//hh = w/8;
					//myPath.moveTo(-w/2,h/2);
					//myPath.lineTo(w/2-hh,h/2);
					//myPath.lineTo(w/2,-h/2);
					//myPath.lineTo(-(w/2-hh),-h/2);
					//myPath.lineTo(-w/2,h/2);
					//this->setBrush(QBrush(Qt::blue));
					break;
				case HandbookGreen:
					imageName = QString(":/images/handbook_green.png");
					//hh = w/8;
					//myPath.moveTo(-w/2,h/2);
					//myPath.lineTo(w/2-hh,h/2);
					//myPath.lineTo(w/2,-h/2);
					//myPath.lineTo(-(w/2-hh),-h/2);
					//myPath.lineTo(-w/2,h/2);
					//this->setBrush(QBrush(Qt::green));
					break;
				case HandbookYellow:
					imageName = QString(":/images/handbook_yellow.png");
					//hh = w/8;
					//myPath.moveTo(-w/2,h/2);
					//myPath.lineTo(w/2-hh,h/2);
					//myPath.lineTo(w/2,-h/2);
					//myPath.lineTo(-(w/2-hh),-h/2);
					//myPath.lineTo(-w/2,h/2);
					//this->setBrush(QBrush(Qt::yellow));
					break;
				case Storage:
					imageName = QString(":/images/storage.png");
					//hh = h/8;
					//myPath.addEllipse(-w/2, -(h/4+hh/2), w, hh);
					//myPath.addEllipse(-w/2, h/4+hh/2, w, hh);
					//myPath.moveTo(-w/2, -h/4);
					//myPath.lineTo(-w/2, h/4+hh);
					//myPath.moveTo(w/2, -h/4);
					//myPath.lineTo(w/2, h/4+hh);
					//this->setBrush(QBrush(Qt::lightGray));
					break;
				case GroupBox:
					imageName = QString(":/images/graybox.png");
					//myPath.addRect(-w/2,-h/2,w,h);
					//this->setBrush(QBrush(Qt::magenta));
					this->setZValue(-2000);
					break;
				case UArrow:
					imageName = QString(":/images/arrow_up.png");
					setZValue(2000.0);
					//this->setBrush(QBrush(Qt::green));
					//myPath.moveTo(4,4);
					//myPath.lineTo(-4,4);
					//myPath.lineTo(0,-4);
					//myPath.lineTo(4,4);
					break;
				case DArrow:
					imageName = QString(":/images/arrow_down.png");
					setZValue(2000.0);
					//this->setBrush(QBrush(Qt::green));
					//myPath.moveTo(-4,-4);
					//myPath.lineTo(4,-4);
					//myPath.lineTo(0,4);
					//myPath.lineTo(-4,-4);
					break;
				case RArrow:
					imageName = QString(":/images/arrow_right.png");
					setZValue(2000.0);
					//this->setBrush(QBrush(Qt::green));
					//myPath.moveTo(-4,4);
					//myPath.lineTo(-4,-4);
					//myPath.lineTo(4,0);
					//myPath.lineTo(-4,4);
					break;
				case LArrow:
					imageName = QString(":/images/arrow_left.png");
					setZValue(2000.0);
					//this->setBrush(QBrush(Qt::green));
					//myPath.moveTo(4,-4);
					//myPath.lineTo(4,4);
					//myPath.lineTo(-4,0);
					//myPath.lineTo(4,-4);
					break;
				default:
					break;
	}
	image.load(imageName);
    pixmap = QPixmap::fromImage(image.scaled(_width-1,_height-1));
	setPixmap(pixmap);
	//this->setPen(pen);
	//setPath(myPath);
}
//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
             QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPixmapItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
		if(mainWindow!=NULL){
			QSize sz = DiagramScene::getDefaultSize(myDiagramType);
			_height = sz.height();
			_width = sz.width();
		}else{
			switch (myDiagramType) {
				case Level0:
				case Level1:
				case Level2:
				case Level3:
				case GroupBox:
					_height = 100;
					_width = 120;
					break;
				case Handbook:
				case HandbookGreen:
				case HandbookBlue:
				case HandbookYellow:
                    _height = 80;
                    _width = 140;
					break;
				case Decision:
                    _height = 100;
					_width = 100;
					break;
				default:
					_height = 100;
					_width = 100;
					
			}
		}
		
    //QPainterPath path;
		//QFont headerFont, bodyFont, footerFont;
		
    update();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
//! [0]
//!
bool DiagramItem::isArrow(){
    return myDiagramType==LArrow || myDiagramType==RArrow || myDiagramType==UArrow || myDiagramType==DArrow;
}

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        updateLines();
    }
    return value;
}
//! [6]
//!

void DiagramItem::updateLines(){
    Q_FOREACH(AttachedLinePoint *a, attachedLinePoints){
        QPointF newPos = mapToScene(a->postionWithinTheItem);
        a->lp->setPos(newPos);
        a->lp->updateLines();
    }
}

void DiagramItem::attachLinePoint(LinePoint *lp){
    AttachedLinePoint *a = new AttachedLinePoint;
    a->lp = lp;
    a->postionWithinTheItem = mapFromScene(lp->mapToScene(lp->boundingRect().center()));
    a->lp->attachedToItem = this;
    attachedLinePoints.append(a);
}
