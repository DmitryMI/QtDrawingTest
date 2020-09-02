#include "renderarea.h"

#include <QMouseEvent>
#include <QPainter>

#define IS_POINT_INSIDE_NODE_INT(cx, cy, x, y) (\
	(x <= cx + RENDER_NODE_SIZE_PX / 2) &&	\
	(x >= cx - RENDER_NODE_SIZE_PX / 2) &&	\
	(y <= cy + RENDER_NODE_SIZE_PX / 2) &&	\
	(y >= cy - RENDER_NODE_SIZE_PX / 2)		\
	)

#define IS_POINT_INSIDE_NODE(center, point) (IS_POINT_INSIDE_NODE_INT(center.x(), center.y(), point.x(), point.y()))


RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
	graphNodesList = new QVector<QPoint>();
	graphEdgesList = new QVector<PointPair>();
	setMouseTracking(true);
}

RenderArea::~RenderArea()
{
	delete graphNodesList;
	delete graphEdgesList;
}

void RenderArea::DrawGraphNode(int x, int y)
{
	QPoint nodeCenter = QPoint(x, y);
	graphNodesList->push_back(nodeCenter);
}

void RenderArea::DrawGraphEdge(int x1, int y1, int x2, int y2)
{
	PointPair pair = PointPair(x1, y1, x2, y2);
	graphEdgesList->push_back(pair);
}

// This method is called every time, when anybody calls update() or repaint() for this widget.
// You can consider this method to be an equivalent for Update() from Unity.
void RenderArea::paintEvent(QPaintEvent *event)
{
	paintRoutine();
}

void RenderArea::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	Qt::MouseButton button = event->button();

	switch(button)
	{
	case Qt::MouseButton::LeftButton:
		DrawGraphNode(x, y);
		break;
	case Qt::MouseButton::RightButton:
		if(prevSelectedNode == -1)
		{
			prevSelectedNode = getNodeOnPointIndex(x, y);
		}
		else
		{
			int selectedNodeIndex = getNodeOnPointIndex(x, y);
			if(selectedNodeIndex == prevSelectedNode)
			{
				prevSelectedNode = -1;
			}
			else if(selectedNodeIndex != -1 )
			{
				QPoint prevCenter = graphNodesList->at(prevSelectedNode);
				QPoint curCenter = graphNodesList->at(selectedNodeIndex);
				DrawGraphEdge(prevCenter.x(), prevCenter.y(), curCenter.x(), curCenter.y());
				prevSelectedNode = -1;
			}
		}
		break;
	default:
		asm("nop");
	}

}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();

	hoveredNodeIndex = getNodeOnPointIndex(x, y);
	update();
}

void RenderArea::paintEdges()
{
	// QPainter initialisation
	QPainter *painter = new QPainter(this);

	// Setting edges pen
	// Pen is used for DRAWING BORDERS AND LINES. Graph edges depend on this
	QPen edgesPen = QPen();
	edgesPen.setColor(QColor::fromRgb(50, 200, 50));
	edgesPen.setWidth(RENDER_EDGE_SIZE_PX);

	{ //*** Rendering saved graphical shapes ****

		// Draw edges
		// Setting pen. This will affect any shape that will be drawn next
		painter->setPen(edgesPen);
		for(int i = 0; i < graphEdgesList->length(); i++)
		{
			PointPair pointerPair = graphEdgesList->at(i);
			QPoint start = pointerPair.getStart();
			QPoint end = pointerPair.getEnd();
			painter->drawLine(start, end);
		}

	} // *** Shapes rendering ends here ***


	// Finalising painter. You must call the end() function.
	painter->end();
}

void RenderArea::paintNodes()
{
	// QPainter initialisation
	QPainter *painter = new QPainter(this);

	// Setting brush to Solid style. No graphical artifacts like transperency will be present
	// Brush is used for FILLING shapes. Filled graph nodes depend on this
	QBrush nodesFillingBrush = QBrush();
	nodesFillingBrush.setColor(QColor::fromRgb(100, 255, 100));
	nodesFillingBrush.setStyle(Qt::BrushStyle::SolidPattern);

	QBrush hoveredNodeBrush = QBrush();
	hoveredNodeBrush.setColor(QColor::fromRgb(100, 100, 255));
	hoveredNodeBrush.setStyle(Qt::BrushStyle::SolidPattern);

	// Setting outlining pen
	// Pen is used for DRAWING BORDERS AND LINES. Outlining depends on this
	QPen nodesOutlinePen = QPen();
	nodesOutlinePen.setColor(QColor::fromRgb(0, 0, 0));
	nodesOutlinePen.setWidth(2);

	{ //*** Rendering saved graphical shapes ****

		// Drawing ellipses
		// Setting pen and brush. This will affect any shape that will be drawn next
		//painter->setBrush(nodesFillingBrush);
		painter->setPen(nodesOutlinePen);
		for(int i = 0; i < graphNodesList->length(); i++)
		{
			QPoint center = graphNodesList->at(i);

			// drawEllipse gets coordinates of top-left edge, not center
			int shiftedX = center.x() - RENDER_NODE_SIZE_PX / 2;
			int shiftedY = center.y() - RENDER_NODE_SIZE_PX / 2;

			if(i == hoveredNodeIndex || i == prevSelectedNode)
			{
				painter->setBrush(hoveredNodeBrush);
			}
			else
			{
				painter->setBrush(nodesFillingBrush);
			}

			painter->drawEllipse(shiftedX, shiftedY, RENDER_NODE_SIZE_PX, RENDER_NODE_SIZE_PX);
		}

	} // *** Shapes rendering ends here ***


	// Finalising painter. You must call the end() function.
	painter->end();
}

void RenderArea::paintRoutine()
{
	paintEdges();
	paintNodes();

	// Not mandatory, but advised. update() forces redrawing all graphics of the Widget
	// No infinite recursion will occure, no idea why.
	update();
}

int RenderArea::getNodeOnPointIndex(int x, int y)
{
	QPoint point = QPoint(x, y);
	for(int i = 0; i < graphNodesList->length(); i++)
	{
		QPoint center = graphNodesList->at(i);
		if(IS_POINT_INSIDE_NODE(center, point))
		{
			return i;
		}
	}
	return -1;
}
