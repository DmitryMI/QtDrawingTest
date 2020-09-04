#include "recursivesearcher.h"
#include "netparams.h"


void RecursiveSearcher::FindAllPaths(Graph<NetParams> *graph, QVector<Path<NetParams>*> *pathsList)
{
	Path<NetParams> *initialPath = new Path<NetParams>();
	GraphNode<NetParams>* startNode = graph->GetStartNode();

	RecursiveSearch(graph, startNode, initialPath, pathsList);
}

RecursiveSearcher::~RecursiveSearcher()
{

}

void RecursiveSearcher::RecursiveSearch(Graph<NetParams> *graph, GraphNode<NetParams> *currentNode, Path<NetParams> *currentPath, QVector<Path<NetParams>*> *pathsList)
{
	currentPath->AddPathFragment(currentNode);

	if(currentNode == graph->GetEndNode())
	{
		pathsList->push_back(currentPath);
		return;
	}

	if(currentNode->ConnetionsCount() == 0)
	{
		return;
	}

	for(int i = 0; i < currentNode->ConnetionsCount(); i++)
	{
		GraphNode<NetParams>* connection = currentNode->ConnectionAt(i);
		if(currentPath->Contains(connection))
		{
			continue;
		}
		Path<NetParams>* pathClone = new Path<NetParams>();
		pathClone->AddPathFragment(currentPath);
		RecursiveSearch(graph, connection, pathClone, pathsList);
	}

	delete currentPath;
}


