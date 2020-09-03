#ifndef RECURSIVESEARCHER_H
#define RECURSIVESEARCHER_H

#include "searcherbase.h"



class RecursiveSearcher : public SearcherBase
{
public:
	void FindAllPaths(Graph *graph, QVector<Path*> *pathsList) override;

	~RecursiveSearcher();
private:
	void RecursiveSearch(Graph *graph, GraphNode* currentNode, Path* currentPath,  QVector<Path*> *pathsList);
};

#endif // RECURSIVESEARCHER_H
