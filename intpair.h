#ifndef POINTPAIR_H
#define POINTPAIR_H

#include <QPoint>


class IntPair
{
public:
	IntPair();
	IntPair(int i1, int i2);

	int getStart();
	int getEnd();

private:
	int val1, val2;
};

#endif // POINTPAIR_H
