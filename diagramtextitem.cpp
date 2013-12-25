#include <QtGui>

#include "diagramtextitem.h"
#include "diagramscene.h"
#include "ipad.h"

QJsonObject DiagramTextItem::toJson(){
    QJsonObject obj;
    int x,y,h,w;
    QString text;
    text = toPlainText().trimmed();
    if(text.isEmpty())
        return obj;

    QRectF rect = mapRectToScene(boundingRect());
    x = rect.x();
    y = rect.y();
    h = rect.height();
    w = rect.width();

    obj["type"] = QJsonValue((int)iosDiagramType);
    obj["text"] = QJsonValue(text);
    obj["x"] = QJsonValue(x);
    obj["y"] = QJsonValue(y);
    obj["height"] = QJsonValue(h);
    obj["width"] = QJsonValue(w);
    return obj;
}

void DiagramTextItem::fromJson(QJsonObject &params){
    double x,y,w;
    iosDiagramType = params["type"].toInt();
    x = params["x"].toInt();
    y = params["y"].toInt();
    w = params["width"].toInt();
    //setTextWidth(w);
    setPlainText(params["text"].toString());
	setPos(QPointF(x,y));
}

//! [0]
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
		iosDiagramType = LABEL;

    QFont sansFont("Helvetica [Cronyx]", 12);
    setFont(sansFont);
}
//! [0]

//! [1]
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}
//! [1]

//! [2]
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
//! [2]

//! [5]
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
//! [5]
