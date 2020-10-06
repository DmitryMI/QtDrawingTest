#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "searcherbase.h"
#include <random>
#include <chrono>



class Calculator
{
public:

	explicit Calculator(SearcherBase *searcher);

	double GetMathematicalProbability(Graph<NetParams> *graph);
	double GetExperimentalProbability(Graph<NetParams> *graph, int experimentsCount);

private:
	SearcherBase* searcher;
	std::uniform_real_distribution<double> unif;
	std::mt19937_64 rng;

	void CloneGraph(Graph<NetParams> *original, Graph<NetParams> *clone);

    bool RollDice(double probability);

};

#endif // CALCULATOR_H
