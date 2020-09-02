#include "pointpair.h"

PointPair::PointPair()
{
	x1 = x2 = y1 = y2 = 0;
}

PointPair::PointPair(int x1, int y1, int x2, int y2)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}

QPoint PointPair::getStart()
{
	return QPoint(x1, y1);
}

QPoint PointPair::getEnd()
{
	return QPoint(x2, y2);
}
