#ifndef GRAPH_H
#define GRAPH_H

#include "graphnode.h"

#include <qvector.h>

class Graph
{
public:
	Graph();

	~Graph();

	// Returns unique key of created GraphNode
	GraphNode* AddNode();
	GraphNode* AddNode(void* data);

	void AddConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void RemoveConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void AddConnectionByKey(int key1, int key2);

	void RemoveConnectionByKey(int key1, int key2);

	void RemoveNodeByIndex(int index);

	void RemoveNodeByKey(int key);

	void AddConnection(GraphNode* a, GraphNode* b)
	{
		if(nodeList->contains(a) && nodeList->contains(b))
		{
			a->AddConnection(b);
			b->AddConnection(a);
		}
	}

	void RemoveConnection(GraphNode *a, GraphNode* b)
	{
		a->RemoveConnection(b);
		b->RemoveConnection(a);
	}

	GraphNode* at(int i);

	int length();

	int GetNodeIndexByKey(int key);

	GraphNode* GetNodeByKey(int key);

	void Clear();

	void SetStartNode(GraphNode* node);
	void SetEndNode(GraphNode *node);
	GraphNode* GetStartNode();
	GraphNode* GetEndNode();

private:
	int keyCounter = 1;
	QVector<GraphNode*> *nodeList = nullptr;

	GraphNode* startNode;
	GraphNode* endNode;
};

#endif // GRAPH_H
