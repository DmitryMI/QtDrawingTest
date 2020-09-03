#ifndef GRAPH_H
#define GRAPH_H

#include "GraphNode.h"

#include <qvector.h>

template <class T>
class Graph
{
public:
	Graph();

	~Graph();

	// Returns unique key of created GraphNode<T>
	GraphNode<T>* AddNode();
	GraphNode<T>* AddNode(void* data);

	void AddConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void RemoveConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void AddConnectionByKey(int key1, int key2);

	void RemoveConnectionByKey(int key1, int key2);

	void RemoveNodeByIndex(int index);

	void RemoveNodeByKey(int key);

	void AddConnection(GraphNode<T>* a, GraphNode<T>* b)
	{
		if(nodeList->contains(a) && nodeList->contains(b))
		{
			a->AddConnection(b);
			b->AddConnection(a);
		}
	}

	void RemoveConnection(GraphNode<T> *a, GraphNode<T>* b)
	{
		a->RemoveConnection(b);
		b->RemoveConnection(a);
	}

	GraphNode<T>* at(int i);

	int length();

	int GetNodeIndexByKey(int key);

	GraphNode<T>* GetNodeByKey(int key);

	void Clear();

	void SetStartNode(GraphNode<T>* node);
	void SetEndNode(GraphNode<T> *node);

	void SetStartNodeByIndex(int nodeIndex);
	void SetEndNodeByIndex(int nodeIndex);

	GraphNode<T>* GetStartNode();
	GraphNode<T>* GetEndNode();

private:
	int keyCounter = 1;
	QVector<GraphNode<T>*> *nodeList = nullptr;

	GraphNode<T>* startNode;
	GraphNode<T>* endNode;
};

#endif // GRAPH_H
