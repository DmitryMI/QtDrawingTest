#include "recursivesearcher.h"
#include "netparams.h"

#include <QQueue>


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

bool RecursiveSearcher::PathExists(Graph<NetParams> *graph)
{
	return BreadthFirstSearch(graph);
}

bool RecursiveSearcher::BreadthFirstSearch(Graph<NetParams> *graph)
{
	bool *visited = new bool[graph->length()];

	for(int i = 0; i < graph->length(); i++)
	{
		visited[i] = false;
	}

	QQueue<int> queue = QQueue<int>();

	auto startNode = graph->GetStartNode();
	auto endNode = graph->GetEndNode();

	if(startNode == nullptr || endNode == nullptr)
	{
		return false;
	}

	int startNodeIndex = graph->GetNodeIndex(startNode);
	int endNodeIndex = graph->GetNodeIndex(endNode);

	if(startNodeIndex == -1 || endNodeIndex == -1)
	{
		 return false;
	}

	queue.enqueue(startNodeIndex);

	while(!queue.isEmpty())
	{
		int nodeIndex = queue.dequeue();
		if(nodeIndex == endNodeIndex)
		{
			return true;
		}

		auto node = graph->at(nodeIndex);
		for(int i = 0; i < node->ConnetionsCount(); i++)
		{
			auto connectedNode = node->ConnectionAt(i);
			int connectionIndex = graph->GetNodeIndex(connectedNode);
			if(visited[connectionIndex])
			{
				continue;
			}
			visited[connectionIndex] = true;
			queue.enqueue(connectionIndex);
		}
	}

	delete[] visited;
	return false;
}

