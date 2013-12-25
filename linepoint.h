#ifndef LINEPOINT_H_DEFINED
#define LINEPOINT_H_DEFINED

#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QList>
#include "line.h"

#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <QWidget>
#include <QPolygonF>
#include <QStyleOptionGraphicsItem>

class Line;
class QString;
class DiagramItem;

//! [0]
class LinePoint : public QGraphicsEllipseItem
{
public:
    friend class DiagramScene;
    friend class DiagramItem;

    enum { Type = UserType + 25 };

		LinePoint(QGraphicsItem *parent = 0);

    void removeLine(Line *arrow);
    void removeLines();
    void addLine(Line *arrow);
    int type() const
        { return Type;}

		int iosDiagramType;

    Q_SLOT void updateLines();

    Q_SLOT void setIsArrow(bool b=true);
    bool isArrow(){return _isArrow;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QMenu *myContextMenu;
    QList<Line *> arrows;
    bool _isArrow;
    DiagramItem *attachedToItem;
};
//! [0]

#endif
