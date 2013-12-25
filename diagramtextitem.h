#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>
#include "iojson.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class DiagramTextItem : public QGraphicsTextItem, public IoJson
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    DiagramTextItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }

		int _height, _width;
		int iosDiagramType;

        QJsonObject toJson();
        void fromJson(QJsonObject&);

signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};
//! [0]

#endif
