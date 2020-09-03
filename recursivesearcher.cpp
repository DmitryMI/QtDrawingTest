#include "recursivesearcher.h"


void RecursiveSearcher::FindAllPaths(Graph *graph, QVector<Path*> *pathsList)
{
	Path *initialPath = new Path();
	GraphNode* startNode = graph->GetStartNode();

	RecursiveSearch(graph, startNode, initialPath, pathsList);
}

RecursiveSearcher::~RecursiveSearcher()
{

}

void RecursiveSearcher::RecursiveSearch(Graph *graph, GraphNode *currentNode, Path *currentPath, QVector<Path*> *pathsList)
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
		GraphNode* connection = currentNode->ConnectionAt(i);
		if(currentPath->Contains(connection))
		{
			continue;
		}
		Path* pathClone = new Path();
		pathClone->AddPathFragment(currentPath);
		RecursiveSearch(graph, connection, pathClone, pathsList);
	}

	delete currentPath;
}


