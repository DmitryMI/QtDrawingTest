#include "renderarea.h"

#include <QMouseEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
	graphNodesList = new QVector<QPoint>();
	graphEdgesList = new QVector<PointPair>();
}

RenderArea::~RenderArea()
{
	delete graphNodesList;
	delete graphEdgesList;
	delete prevRightClick;
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
	// QPainter initialisation
	QPainter *painter = new QPainter(this);

	// Setting brush to Solid style. No graphical artifacts like transperency will be present
	// Brush is used for FILLING shapes. Filled graph nodes depend on this
	QBrush nodesFillingBrush = QBrush();
	nodesFillingBrush.setColor(QColor::fromRgb(100, 255, 100));
	nodesFillingBrush.setStyle(Qt::BrushStyle::SolidPattern);

	// Setting outlining pen
	// Pen is used for DRAWING BORDERS AND LINES. Graph edges depend on this
	QPen nodesOutlinePen = QPen();
	nodesOutlinePen.setColor(QColor::fromRgb(0, 0, 0));
	nodesOutlinePen.setWidth(2);

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

		// Drawing ellipses
		// Setting pen and brush. This will affect any shape that will be drawn next
		painter->setBrush(nodesFillingBrush);
		painter->setPen(nodesOutlinePen);
		for(int i = 0; i < graphNodesList->length(); i++)
		{
			QPoint center = graphNodesList->at(i);

			// drawEllipse gets coordinates of top-left edge, not center
			int shiftedX = center.x() - RENDER_NODE_SIZE_PX / 2;
			int shiftedY = center.y() - RENDER_NODE_SIZE_PX / 2;
			painter->drawEllipse(shiftedX, shiftedY, RENDER_NODE_SIZE_PX, RENDER_NODE_SIZE_PX);
		}

	} // *** Shapes rendering ends here ***


	// Finalising painter. You must call the end() function.
	painter->end();

	// Not mandatory, but advised. update() forces redrawing all graphics of the Widget
	// No infinite recursion will occure, no idea why.
	update();
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
		if(prevRightClick == nullptr)
		{
			prevRightClick = new QPoint(x, y);
		}
		else
		{
			int prevX = prevRightClick->x();
			int prevY = prevRightClick->y();

			DrawGraphEdge(prevX, prevY, x, y);
			delete prevRightClick;
			prevRightClick = nullptr;
		}
		break;
	}

}
