#pragma once

#include <QUndoCommand>
#include <QPointF>
#include <QSize>
#include <QGraphicsItem>

#include "diagramitem.h"

class MoveItemCommand : public QUndoCommand {
	double prevX,prevY,x,y;
	QGraphicsItem *item;

	MoveItemCommand(QGraphicsItem *item, double x, double y){
		this->item = item;
		this->x = x;
		this->y = y;
		this->prevX = this->item->x();
		this->prevY = this->item->y();
	}
	void undo(){
		this->item->setPos(this->prevX, this->prevY);
	}
	void redo(){
		this->item->setPos(this->x,this->y);
	}
};

/*
class ResizeItemCommand(){
	QSize prevSz,sz;
	ResizeItemCommand(QGraphicsItem *item, QPointF sz){
		this->item = item;
		this->prevSz = QSize();
		this->sz = sz;
	}
	void undo(){
		this->item->setSize(sz);
	}
	void redo(){
		this->item->setHeight()
	}
}*/