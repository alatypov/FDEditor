#ifndef LINE_H_DEFINED
#define LINE_H_DEFINED

#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>

class LinePoint;

#include "iojson.h"
#include "diagramscene.h"

//! [0]
class Line : public QGraphicsLineItem, public IoJson
{
public:
    friend class DiagramScene;

    enum { Type = UserType + 24 };

    Line(LinePoint *startItem=0, LinePoint *endItem=0, int mode=DiagramScene::InsertLine,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    LinePoint *startItem() const
        { return myStartItem; }
    LinePoint *endItem() const
        { return myEndItem; }

        QJsonObject toJson();
        void fromJson(QJsonObject&);
		int iosDiagramType;

		void setPen(const QPen &pen){
			QGraphicsLineItem::setPen(pen);
		}

public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    LinePoint *myStartItem;
    LinePoint *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};
//! [0]

#endif
