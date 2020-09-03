#include "probabilityprovider.h"
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


RenderArea::RenderArea(ProbabilityProvider *probabilityProvider, QWidget *parent) : QWidget(parent)
{
	graphNodesList = new QVector<QPoint>();
	graphEdgesList = new QVector<IntPair>();
	probabilitiesList = new QVector<double>();
	setMouseTracking(true);

	this->probabilityProvider = probabilityProvider;
}

RenderArea::~RenderArea()
{
	delete graphNodesList;
	delete graphEdgesList;
	delete probabilitiesList;
}

void RenderArea::DrawGraphNode(int x, int y, double probability)
{
	QPoint nodeCenter = QPoint(x, y);
	graphNodesList->push_back(nodeCenter);
	probabilitiesList->push_back(probability);
}

void RenderArea::DrawGraphEdge(int nodeIndex1, int nodeIndex2)
{
	IntPair pair = IntPair(nodeIndex1, nodeIndex2);
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

	int nodeOnPointIndex = getNodeOnPointIndex(x, y);

	Qt::MouseButton button = event->button();

	switch(button)
	{
	case Qt::MouseButton::LeftButton:
		if(nodeOnPointIndex == -1)
		{
			double probability;
			if(probabilityProvider->GetCurrentProbability(&probability))
			{
				DrawGraphNode(x, y, probability);
			}
		}
		break;
	case Qt::MouseButton::RightButton:
		if(prevSelectedNode == -1)
		{
			prevSelectedNode = nodeOnPointIndex;
		}
		else
		{
			int selectedNodeIndex = nodeOnPointIndex;
			if(selectedNodeIndex == prevSelectedNode)
			{
				prevSelectedNode = -1;
			}
			else if(selectedNodeIndex != -1 )
			{
				DrawGraphEdge(prevSelectedNode, selectedNodeIndex);
				prevSelectedNode = -1;
			}
		}
		break;
	case Qt::MouseButton::MiddleButton:

		if(nodeOnPointIndex == -1)
		{
			startNodeIndex = -1;
			endNodeIndex = -1;
		}
		else if(nodeOnPointIndex == startNodeIndex || nodeOnPointIndex == endNodeIndex)
		{
			// Do nothing
		}
		else
		{
			if(startNodeIndex == -1)
			{
				startNodeIndex = nodeOnPointIndex;
			}
			else if(endNodeIndex == -1)
			{
				endNodeIndex = nodeOnPointIndex;
			}
			else
			{
				startNodeIndex = nodeOnPointIndex;
				endNodeIndex = -1;
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
			IntPair pointerPair = graphEdgesList->at(i);
			int start = pointerPair.getStart();
			int end = pointerPair.getEnd();

			QPoint startPos = graphNodesList->at(start);
			QPoint endPos = graphNodesList->at(end);

			painter->drawLine(startPos, endPos);
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
	nodesFillingBrush.setColor(QColor::fromRgb(100, 100, 100));
	nodesFillingBrush.setStyle(Qt::BrushStyle::SolidPattern);

	QBrush hoveredNodeBrush = QBrush();
	hoveredNodeBrush.setColor(QColor::fromRgb(100, 100, 255));
	hoveredNodeBrush.setStyle(Qt::BrushStyle::SolidPattern);

	QBrush startNodeBrush = QBrush();
	startNodeBrush.setColor(QColor::fromRgb(100, 255, 100));
	startNodeBrush.setStyle(Qt::BrushStyle::SolidPattern);

	QBrush endNodeBrush = QBrush();
	endNodeBrush.setColor(QColor::fromRgb(255, 100, 100));
	endNodeBrush.setStyle(Qt::BrushStyle::SolidPattern);

	// Setting outlining pen
	// Pen is used for DRAWING BORDERS AND LINES. Outlining depends on this
	QPen nodesOutlinePen = QPen();
	nodesOutlinePen.setColor(QColor::fromRgb(0, 0, 0));
	nodesOutlinePen.setWidth(2);

	QPen nodesLabelPen = QPen();
	nodesLabelPen.setColor(QColor::fromRgb(255, 255, 255));
	nodesLabelPen.setWidth(2);

	{ //*** Rendering saved graphical shapes ****

		// Drawing ellipses

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
			else if(i == startNodeIndex)
			{
				painter->setBrush(startNodeBrush);
			}
			else if(i == endNodeIndex)
			{
				painter->setBrush(endNodeBrush);
			}
			else
			{
				painter->setBrush(nodesFillingBrush);
			}

			// Setting pen and brush. This will affect any shape that will be drawn next
			//painter->setBrush(nodesFillingBrush);
			painter->setPen(nodesOutlinePen);

			painter->drawEllipse(shiftedX, shiftedY, RENDER_NODE_SIZE_PX, RENDER_NODE_SIZE_PX);

			// Setting pen and brush. This will affect any shape that will be drawn next
			//painter->setBrush(nodesFillingBrush);
			painter->setPen(nodesLabelPen);
			QString text = "%1";
			double probability = probabilitiesList->at(i);
			text = text.arg(probability);
			painter->drawText(center.x(), center.y(), text);
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

Graph<NetParams> *RenderArea::GetGraph()
{
	Graph<NetParams> *graph = new Graph<NetParams>();

	for(int i = 0; i < graphNodesList->length(); i++)
	{
		graph->AddNode(nullptr);
	}

	for(int i = 0; i < graphEdgesList->length(); i++)
	{
		IntPair pair = graphEdgesList->at(i);
		graph->AddConnectionByIndex(pair.getStart(), pair.getEnd());
	}

	graph->SetStartNodeByIndex(startNodeIndex);
	graph->SetEndNodeByIndex(endNodeIndex);

	return graph;
}
