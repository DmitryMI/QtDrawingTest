#include "calculator.h"

#define DELETE_VECTOR(vec) \
	for(int i = 0; i < vec->length(); i++)	\
	{										\
		delete vec->at(i);					\
	}										\
	delete vec;

double Calculator::GetMathematicalProbability(Graph *graph)
{
	QVector<Path*> *pathList = new QVector<Path*>();
	searcher->FindAllPaths(graph, pathList);

	int pathsCount = pathList->length();

	DELETE_VECTOR(pathList);

	return pathsCount;
}

double Calculator::GetExperimentalProbability(Graph *graph, int experimentsCount)
{
	return 0;
}

Calculator::Calculator(SearcherBase *searcher)
{
	this->searcher = searcher;
}
