#ifndef POINTPAIR_H
#define POINTPAIR_H

#include <QPoint>


class PointPair
{
public:
	PointPair();
	PointPair(int x1, int y1, int x2, int y2);

	QPoint getStart();
	QPoint getEnd();

private:
	int x1, y1, x2, y2;
};

#endif // POINTPAIR_H
