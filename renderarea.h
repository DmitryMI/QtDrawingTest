#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "pointpair.h"

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

	void DrawGraphEdge(int x1, int y1, int x2, int y2);

protected:
	void paintEvent(QPaintEvent *event) override;

	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	QVector<QPoint> *graphNodesList;
	QVector<PointPair> *graphEdgesList;

	QPoint *prevRightClick = nullptr;

signals:

};

#endif // RENDERAREA_H
