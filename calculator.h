#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "searcherbase.h"



class Calculator
{
public:

	explicit Calculator(SearcherBase *searcher);

	double GetMathematicalProbability(Graph *graph);
	double GetExperimentalProbability(Graph *graph, int experimentsCount);

private:
	SearcherBase* searcher;
};

#endif // CALCULATOR_H
