#include <QtGui>

#include "linepoint.h"
#include "diagramscene.h"
#include "ipad.h"

LinePoint::LinePoint(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    iosDiagramType = LINEPOINT;
    _isArrow=false;

    setBrush(QBrush(Qt::red));
    setRect(QRectF(-2,-2,4,4));

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
		setZValue(1000.0);
}

void LinePoint::removeLine(Line *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void LinePoint::removeLines()
{
    foreach (Line *arrow, arrows) {
        arrow->startItem()->removeLine(arrow);
        arrow->endItem()->removeLine(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void LinePoint::addLine(Line *arrow)
{
    arrows.append(arrow);
}

/*QPixmap LinePoint::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    //painter.draw(rect());
    return pixmap;
}*/

void LinePoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //scene()->clearSelection();
    //setSelected(true);
    //myContextMenu->exec(event->screenPos());
}

QVariant LinePoint::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        updateLines();
    }
    return value;
}

void LinePoint::updateLines(){
    Q_FOREACH (Line *arrow, arrows) {
        arrow->updatePosition();
    }
}

void LinePoint::setIsArrow(bool b){
    if(_isArrow==b)
        return;

    _isArrow=b;
    if(b){
        setBrush(QBrush(Qt::blue));
    }else{
        setBrush(QBrush(Qt::red));
    }
    update();
}
