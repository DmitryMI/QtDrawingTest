#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "graph.h"
#include "intpair.h"
#include "netparams.h"
#include "probabilityprovider.h"

#include <QWidget>

#define RENDER_NODE_SIZE_PX 50
#define RENDER_EDGE_SIZE_PX 25

class RenderArea : public QWidget
{
	Q_OBJECT
public:
	explicit RenderArea(ProbabilityProvider *probabilityProvider, QWidget *parent = nullptr);

	~RenderArea();

	void DrawGraphNode(int x, int y, double probability = 1);

	void DrawGraphEdge(int nodeIndex1, int nodeIndex2);

	// User must deallocate memory mannually for the returned object
	Graph<NetParams>* GetGraph();

protected:
	void paintEvent(QPaintEvent *event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

private:
	QVector<QPoint> *graphNodesList;
	QVector<double> *probabilitiesList;
	QVector<IntPair> *graphEdgesList;

	ProbabilityProvider *probabilityProvider;

	int prevSelectedNode = -1;

	void paintEdges();
	void paintNodes();
	void paintRoutine();

	int getNodeOnPointIndex(int x, int y);

	int hoveredNodeIndex = -1;

	int startNodeIndex = -1;
	int endNodeIndex = -1;

signals:

};

#endif // RENDERAREA_H
