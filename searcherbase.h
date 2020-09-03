#ifndef SEARCHERBASE_H
#define SEARCHERBASE_H

#include "graph.h"
#include "path.h"

#include <qvector.h>



class SearcherBase
{
public:
	virtual void FindAllPaths(Graph *graph, QVector<Path*> *pathsList) = 0;
	virtual ~SearcherBase(){}
};

#endif // SEARCHERBASE_H
