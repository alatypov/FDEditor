#include <QtGui>

#include "diagramscene.h"
#include "diagramitem.h"
//#include "arrow.h"
#include <stdio.h>
#include <fstream>
#include <QList>
#include <QMessageBox>
#include <QDir>
#include <QPen>
#include <QColor>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QRegExp>
#include <QLatin1String>
#include <string>
#include <QByteArray>

#include "linepoint.h"
#include "line.h"
#include "mainwindow.h"

#include "commands.h"

static int counter[3];
static QString headerPrefix;
static QMap<int,QSize> defaults;

extern MainWindow *mainWindow;

QSize DiagramScene::getDefaultSize(int forType){return defaults[forType];}
void DiagramScene::setDefaultSize(int forType, QSize sz){defaults[forType]=sz;}

void DiagramScene::alignItemsByTop(){
	int minTop=100000,y;
	QList<QGraphicsItem*>items = selectedItems();
	Q_FOREACH(QGraphicsItem *item, items){
		y = item->mapRectToScene(item->boundingRect()).y();
		if(y<minTop) minTop = y;
	}
	Q_FOREACH(QGraphicsItem *item, items){
		item->setPos(item->pos().x(),minTop);
	}
}
void DiagramScene::alignItemsByButtom(){

}
void DiagramScene::alignItemsByRight(){

}
void DiagramScene::adjustItemSizes(){
	QRectF maxSz(0,0,0,0),sz;
	QList<QGraphicsItem*>items = selectedItems();
	DiagramItem *di;
	Q_FOREACH(QGraphicsItem *item, items){
		sz = item->mapRectToScene(item->boundingRect());
		if(sz.height()>maxSz.height()) maxSz.setHeight(sz.height());
		if(sz.width()>maxSz.width()) maxSz.setWidth(sz.width());
	}
	Q_FOREACH(QGraphicsItem *item, items){
		di = dynamic_cast<DiagramItem*>(item);
		if(di!=0){
			di->setSize(QSize(maxSz.width(),maxSz.height()));
			di->update();
		}
	}
}

void DiagramScene::alignItemsByLeft(){
	int minLeft=100000,x;
	QList<QGraphicsItem*>items = selectedItems();
	Q_FOREACH(QGraphicsItem *item, items){
		x = item->mapRectToScene(item->boundingRect()).x();
		if(x<minLeft) minLeft = x;
	}
	Q_FOREACH(QGraphicsItem *item, items){
		item->setPos(minLeft,item->pos().y());
	}
}

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
: QGraphicsScene(parent)
{
	myItemMenu = itemMenu;
	myMode = MoveItem;
	myItemType = DiagramItem::Level0;
	line = 0;
	textItem = 0;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;

	counter[0]=0;
	counter[1]=0;
	counter[2]=0;

    currentLinePoint = 0;
}
//! [0]

void DiagramScene::drawBackground(QPainter * painter, const QRectF & rect){
	painter->drawLine(-rect.width()/2, 0, rect.width()/2, 0);
	painter->drawLine(0, -rect.height()/2, 0, rect.height()/2);
	painter->drawRect(0,0,IPAD_WIDTH,IPAD_HEIGHT);
	painter->drawRect(IPAD_WIDTH,0,IPAD_WIDTH,IPAD_HEIGHT);
	painter->drawRect(0,IPAD_HEIGHT,IPAD_WIDTH,IPAD_HEIGHT);
	painter->drawRect(IPAD_WIDTH,IPAD_HEIGHT,IPAD_WIDTH,IPAD_HEIGHT);
	int x,y;
	painter->setPen(QPen(Qt::lightGray));
	for(x=0; x<IPAD_WIDTH*2; x+=10){
		painter->drawLine(x, 0, x, IPAD_HEIGHT*2);
	}
	for(y=0; y<IPAD_HEIGHT*2; y+=10){
		painter->drawLine(0, y, IPAD_WIDTH*2, y);
	}
	painter->setPen(QPen(Qt::blue));
	painter->drawLine(0, HEADER_W_OFFSET, IPAD_WIDTH*2, HEADER_W_OFFSET);
}

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
	/*myLineColor = color;
	if (isItemChange(Arrow::Type)) {
		Arrow *item =
			qgraphicsitem_cast<Arrow *>(selectedItems().first());
		item->setColor(myLineColor);
		update();
	}*/
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
	myTextColor = color;
	if (isItemChange(DiagramTextItem::Type)) {
		DiagramTextItem *item =
			qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
	}
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    /*myItemColor = color;
	if (isItemChange(DiagramItem::Type)) {
		DiagramItem *item =
			qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
		item->setBrush(myItemColor);
    }*/
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
	myFont = font;

	if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item =
            qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
		//At this point the selection can change so the first selected item might not be a DiagramTextItem
		if (item)
			item->setFont(myFont);
	}
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
	myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
	myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
		removeItem(item);
		item->deleteLater();
	}
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	DiagramItem *item;
	LinePoint *lp;
	mainWindow->view->setDragMode(QGraphicsView::RubberBandDrag);	
	switch (myMode) {
				case InsertItem:
					item = new DiagramItem(myItemType, myItemMenu);
					if(myItemType<=2){
						counter[myItemType]++;
                        if(counter[myItemType]<10){
                            item->header = QString("%1.0%2").arg(headerPrefix).arg(counter[myItemType]);
                        }else{
                            item->header = QString("%1.%2").arg(headerPrefix).arg(counter[myItemType]);
                        }
					}
					//item->setBrush(myItemColor);
					addItem(item);

//                    if(item->isArrow()){
//                        QList<QGraphicsItem *> nearItems = item->collidingItems();
//                        QList<QGraphicsItem *>::iterator it;
//                        LinePoint* lp;
//                        Line *l;
//                        QPointF p;
//                        for(it=nearItems.begin(); it!=nearItems.end(); it++){
//                            lp = dynamic_cast<LinePoint*>(*it);
//                            if(lp!=0){
//                                //we correct mouse pos so it point right to this lineoint
//                                p = lp->boundingRect().center();
//                                mouseEvent->setScenePos(p);
//                            }else{
//                               l = dynamic_cast<Line*>(*it);
//                               if(l!=0){
//                               }
//                            }
//                        }
//                    }

                    item->setPos(mouseEvent->scenePos());

					emit itemInserted(item);
					break;
				case InsertLine:
				case InsertDashLine:
					mainWindow->view->setDragMode(QGraphicsView::NoDrag);
					line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
						mouseEvent->scenePos()));
					line->setPen(QPen(myLineColor, 1));
					addItem(line);
					break;
				case InsertText:
					textItem = new DiagramTextItem();
					textItem->setFont(myFont);
                    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
					textItem->setZValue(1000.0);
					connect(textItem, SIGNAL(lostFocus(DiagramTextItem *)),
						this, SLOT(editorLostFocus(DiagramTextItem *)));
					connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
						this, SIGNAL(itemSelected(QGraphicsItem *)));
					addItem(textItem);
					textItem->setDefaultTextColor(myTextColor);
					textItem->setPos(mouseEvent->scenePos());
					emit textInserted(textItem);
				default:
                    QGraphicsScene::mousePressEvent(mouseEvent);
                    QList<QGraphicsItem *> s = selectedItems();
                    if(s.size()==1){
                        currentLinePoint = dynamic_cast<LinePoint*>(s[0]);
                    }
                    return;
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if ((myMode == InsertLine || myMode == InsertDashLine) && line != 0){
		QLineF newLine(line->line().p1(), mouseEvent->scenePos());
		/*QPointF p1,p2;
		p1 = line->line().p1();
		p2 = line->line().p2();
		if((p1.x()==p2.x())||(p1.y()==p2.y())){
			line->setPen(QPen(Qt::black));
		}else{
			line->setPen(QPen(Qt::red));
		}*/
		line->setLine(newLine);
    }else{
        //we want to check if we can glue up two lines together
        if(currentLinePoint!=0){
            QList<QGraphicsItem*> s = currentLinePoint->collidingItems(Qt::IntersectsItemBoundingRect);
            LinePoint *anotherLinePoint;
            if(s.size()>0){
                Q_FOREACH(QGraphicsItem* it, s){
                    anotherLinePoint = dynamic_cast<LinePoint*>(&(*it));
                    if(anotherLinePoint!=0){ //its the line point
                        QList<Line*> a = anotherLinePoint->arrows;
                        Q_FOREACH(Line* p, a){
                            currentLinePoint->arrows.append(p);
                            if(anotherLinePoint == p->myStartItem){
                                //if(p->myEndItem == currentLinePoint) //we should not merge points of the same line
                                //    continue;
                                p->myStartItem = currentLinePoint;
                            }else{
                                //if(p->myStartItem == currentLinePoint) //we should not merge points of the same line
                                //    continue;
                                p->myEndItem = currentLinePoint;
                            }
                        }
                        //clearSelection();
                        removeItem(anotherLinePoint);
                        delete anotherLinePoint;
                        //currentLinePoint = anotherLinePoint;
                        //currentLinePoint=0;
                        break;
                    }else{
                        DiagramItem *item;
                        item = dynamic_cast<DiagramItem*>(&(*it));
                        if(item!=0){ //its the DiagramItem
                            bool contains=false;
                            AttachedLinePoint *a;
                            Q_FOREACH(a, item->attachedLinePoints){
                                if(a->lp==currentLinePoint){
                                    contains=true;
                                    break;
                                }
                            }
                            if(contains){
                                a->postionWithinTheItem = item->mapFromScene(currentLinePoint->mapToScene(currentLinePoint->boundingRect().center()));
                            }else{
                                item->attachLinePoint(currentLinePoint);
                            }
                        }
                    }
                }
            }
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (line != 0 && (myMode == InsertLine || myMode == InsertDashLine)) {
		/*QList<QGraphicsItem *> startItems = items(line->line().p1());
		if (startItems.count() && startItems.first() == line)
			startItems.removeFirst();
		QList<QGraphicsItem *> endItems = items(line->line().p2());
		if (endItems.count() && endItems.first() == line)
			endItems.removeFirst();*/
		
		//item->setBrush(myItemColor);
		LinePoint *startItem = new LinePoint(), 
			*endItem = new LinePoint();
		addItem(startItem);
		addItem(endItem);
		startItem->setPos(line->line().p1());
		endItem->setPos(line->line().p2());

		Line *l = new Line(startItem, endItem, myMode);
		startItem->addLine(l);
		endItem->addLine(l);
		l->setZValue(-1000.0);
		addItem(l);
		l->updatePosition();
		
		removeItem(line);
		delete line;
    }

	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	line = 0;
    currentLinePoint=0;
}

bool DiagramScene::isItemChange(int type)
{
	foreach (QGraphicsItem *item, selectedItems()) {
		if (item->type() == type)
			return true;
	}
	return false;
}

#include "ipad.h"

bool lessThenByZValue(const QGraphicsItem &it1, const QGraphicsItem &it2){
	return it1.zValue() < it2.zValue();
}

void sortlist(QList<QGraphicsItem *> its){
    int i,j,n;
    n = its.size();
    QGraphicsItem *p;
    for(i=0;i<n-1;i++){
        for(j=i+1;j<n;j++){
            if(lessThenByZValue(*its[i], *its[j])){
                p = its[i];
                its[i]=its[j];
                its[j]=p;
            }
        }
    }
}

void DiagramScene::exportToJSON(QString &fileName){
    QJsonDocument jsonDocument;
    QList<QGraphicsItem *> its = this->items();
    QGraphicsItem *it;
    sortlist(its);

    QJsonArray jsonItems;
    QJsonObject obj;
    IoJson *jsonObj;
    bool objectIsEmpty;
    int n = its.size();
    for(int i=0; i<n; i++){
        it = its[n-1-i];
        jsonObj = dynamic_cast<IoJson *>(it);
        if(!jsonObj)
            continue;
        obj = jsonObj->toJson();
        objectIsEmpty = obj.isEmpty();
        if(objectIsEmpty)
            continue;

        jsonItems.append(obj);
    }

    jsonDocument.setArray(jsonItems);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(NULL, QString("JSON Export Error"),
            QString("Can not open file ''%1''").arg(fileName));
        return;
    }
    file.write(jsonDocument.toJson());
    file.close();
}

bool DiagramScene::importFromJSON(QString &fileName){
	DiagramItem *di;
	DiagramTextItem *ti;
	IoJson *jsonObj;

    QJsonDocument jsonDocument;
    QJsonParseError error;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(NULL, QString("JSON Import Error"),
            QString("Can not open file ''%1''").arg(fileName));
        return false;
    }

    QTextStream inf(&file);
    QString content = inf.readAll();
    jsonDocument = QJsonDocument::fromJson(content.toUtf8().data(), &error);
    if( jsonDocument.isNull() ) {
        QMessageBox::critical(NULL, QString("JSON Import Error"),
            QString("Failed to parse JSON file ''%1'', due to ''%2''.").arg(fileName).arg(error.errorString()));
        return false;
    }

    if( !jsonDocument.isArray() ){
        QMessageBox::critical(NULL, QString("JSON Import Error"),
            QString("Document should contain an Array."));
        return false;
    }

    QJsonArray items = jsonDocument.array();
    QJsonObject item;
	QPointF pt1, pt2;
	clear();

	int diType, x, y, h, w;
	for(int i=0; i<items.size(); i++){
        item = items[i].toObject();
        diType = item["type"].toInt();
		if(diType == LABEL){
			ti = new DiagramTextItem();
            ti->fromJson(item);
			addItem(ti);
			emit textInserted(ti);
		}else if((diType == LINE) || (diType==DASH_LINE)){
			int x1,x2,y1,y2;
            x1 = item["start"].toObject()["x"].toInt();
            y1 = item["start"].toObject()["y"].toInt();
            x2 = item["end"].toObject()["x"].toInt();
            y2 = item["end"].toObject()["y"].toInt();

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

			LinePoint *startItem = new LinePoint(), 
				*endItem = new LinePoint();
			addItem(startItem);
			addItem(endItem);
			startItem->setPos(pt1);
			endItem->setPos(pt2);
			int mode;
			if(diType == LINE)
				mode = DiagramScene::InsertLine;
			else if(diType == DASH_LINE)
				mode = DiagramScene::InsertDashLine;
			Line *l = new Line(startItem, endItem, mode);
			startItem->addLine(l);
			endItem->addLine(l);
			l->setZValue(-1000.0);
			addItem(l);
			l->updatePosition();
		}else{
			di = new DiagramItem(((DiagramItem::DiagramType)diType), myItemMenu);
			di->fromJson(item);
			addItem(di);
			//emit itemInserted(di);
		}
    }
	return true;
}

void DiagramScene::setPrefixAndResetCounters(QString pref){
	headerPrefix = pref;
	counter[0]=0;
	counter[1]=0;
	counter[2]=0;
}

void DiagramScene::loadDefaults(QString fileName){
	QSize sz;

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

    QJsonDocument jsonDocument;
	QTextStream inf(&file);
    QString content = inf.readAll();
    QJsonParseError error;
    jsonDocument = QJsonDocument::fromJson(content.toUtf8().data(), &error);
    if( jsonDocument.isNull() ) {
        QMessageBox::critical(NULL, QString("JSON Import Error"),
            QString("Failed to parse JSON file ''%1'', due to ''%2''.").arg(fileName).arg(error.errorString()));
		return;
	}

    QJsonArray items;
    QJsonObject item;
    items = jsonDocument.array();
    int type, h, w;
    for(int i=0; i<items.size(); i++){
        item = items[i].toObject();
        type = item["type"].toInt();
        h = item["height"].toInt();
        w = item["width"].toInt();
		sz.setHeight(h);
		sz.setWidth(w);
		defaults[type]=sz;
    }
}

void DiagramScene::copyItem(){
	clipboard = selectedItems();
}

void DiagramScene::pasteItem(){
	clearSelection();
	QPointF posOffset(20,20);
	DiagramItem *newItem;
	QRectF rec;
	Q_FOREACH(QGraphicsItem *item, clipboard){
		DiagramItem *d = dynamic_cast<DiagramItem*>(item);
		if(d==0)
			continue;

		newItem = new DiagramItem(d->myDiagramType, d->myContextMenu);
		newItem->header = d->header;
		newItem->body = d->body;
		newItem->footer = d->footer;
		rec = d->mapRectFromScene(d->boundingRect());
		newItem->setSize(QSize(rec.width(),rec.height()));
		newItem->setPos(posOffset.x()-rec.x(),posOffset.y()-rec.y());

		addItem(newItem);
		Q_EMIT itemInserted(newItem);

		newItem->setSelected(true);
	}
}

bool lessThenByXValue(const QGraphicsItem &it1, const QGraphicsItem &it2){
	return it1.x() < it2.x();
}

bool lessThenByYValue(const QGraphicsItem &it1, const QGraphicsItem &it2){
	return it1.y() < it2.y();
}

void DiagramScene::setSameDistHorizontal(){
	QList<QGraphicsItem*> items = selectedItems();
	if(items.size()<2)
		return;
    //qStableSort(items.begin(), items.end(), lessThenByXValue);
	int i;
	QGraphicsItem *lastItem;
	lastItem = items[0];
	for(i=1; i<items.size(); i++){
		items[i]->setX(lastItem->x() + lastItem->boundingRect().width()+30);
		lastItem = items[i];
	}
}

void DiagramScene::setSameDistVertical(){
	QList<QGraphicsItem*> items = selectedItems();
	if(items.size()<2)
		return;
    //qStableSort(items.begin(), items.end(), lessThenByYValue);
	int i;
	QGraphicsItem *lastItem;
	lastItem = items[0];
	for(i=1; i<items.size(); i++){
		items[i]->setY(lastItem->y() + lastItem->boundingRect().height()+30);
		lastItem = items[i];
	}
}


void DiagramScene::fixLines(){
    QList<Line*> vlines, hlines;
    Line *l;
    QList<QGraphicsItem*> allItems = items();
    QPointF p1, p2;
    Q_FOREACH(QGraphicsItem* it, allItems){
        l = dynamic_cast<Line*>(&(*it));
        if(l!=0){
            p1 = l->mapToScene(l->line().p1());
            p2 = l->mapToScene(l->line().p2());
            if(fabs(p1.x()-p2.x()) < 3.0){ //horizontal line
                hlines << l;
            }else if(fabs(p1.y()-p2.y()) < 3.0){ //vertical line
                vlines << l;
            }
        }
    }
}

void DiagramScene::fixArrows(){

}

void DiagramScene::makeCurrentLinePointAnArrow(){
    QList<QGraphicsItem*> s = selectedItems();
    LinePoint *lp;
    Q_FOREACH(QGraphicsItem*it, s){
        lp = dynamic_cast<LinePoint*>(&(*it));
        if(lp!=0){
            lp->setIsArrow(!lp->isArrow());
        }
    }
}
