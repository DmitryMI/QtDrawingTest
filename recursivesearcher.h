#ifndef RECURSIVESEARCHER_H
#define RECURSIVESEARCHER_H

#include "searcherbase.h"



class RecursiveSearcher : public SearcherBase
{
public:
	void FindAllPaths(Graph<NetParams> *graph, QVector<Path<NetParams>*> *pathsList) override;

	~RecursiveSearcher();
private:
	void RecursiveSearch(Graph<NetParams> *graph, GraphNode<NetParams>* currentNode, Path<NetParams>* currentPath,  QVector<Path<NetParams>*> *pathsList);
};

#endif // RECURSIVESEARCHER_H
