#include "intpair.h"

IntPair::IntPair()
{
	val1 = val2 = 0;
}

IntPair::IntPair(int i1, int i2)
{
	val1 = i1;
	val2 = i2;
}

int IntPair::getStart()
{
	return val1;
}

int IntPair::getEnd()
{
	return val2;
}
