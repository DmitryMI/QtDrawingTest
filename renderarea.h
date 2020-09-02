#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "intpair.h"

#include <QWidget>

#define RENDER_NODE_SIZE_PX 50
#define RENDER_EDGE_SIZE_PX 25

class RenderArea : public QWidget
{
	Q_OBJECT
public:
	explicit RenderArea(QWidget *parent = nullptr);

	~RenderArea();

	void DrawGraphNode(int x, int y);

	void DrawGraphEdge(int nodeIndex1, int nodeIndex2);

protected:
	void paintEvent(QPaintEvent *event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

private:
	QVector<QPoint> *graphNodesList;
	QVector<IntPair> *graphEdgesList;

	int prevSelectedNode = -1;

	void paintEdges();
	void paintNodes();
	void paintRoutine();

	int getNodeOnPointIndex(int x, int y);

	int hoveredNodeIndex = -1;

signals:

};

#endif // RENDERAREA_H
