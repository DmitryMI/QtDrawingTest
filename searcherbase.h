#ifndef SEARCHERBASE_H
#define SEARCHERBASE_H

#include "graph.h"
#include "netparams.h"
#include "path.h"

#include <qvector.h>



class SearcherBase
{
public:
	virtual void FindAllPaths(Graph<NetParams> *graph, QVector<Path<NetParams>*> *pathsList) = 0;
	virtual ~SearcherBase(){}
};

#endif // SEARCHERBASE_H
