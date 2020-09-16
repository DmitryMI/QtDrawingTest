#ifndef GRAPH_H
#define GRAPH_H

#include "graphnode.h"

#include <qvector.h>

template <class T>
class Graph
{
public:
	Graph();

	~Graph();

	// Returns unique key of created GraphNode<T>
	GraphNode<T>* AddNode();
	GraphNode<T>* AddNode(T data);

	void AddConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void RemoveConnectionByIndex(int nodeIndex1, int nodeIndex2);

	void AddConnectionByKey(int key1, int key2);

	void RemoveConnectionByKey(int key1, int key2);

	void RemoveNodeByIndex(int index);

	void RemoveNodeByKey(int key);

	void RemoveNode(GraphNode<T>* node);

	void AddConnection(GraphNode<T>* a, GraphNode<T>* b);

	void RemoveConnection(GraphNode<T> *a, GraphNode<T>* b);

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

	int GetNodeIndex(GraphNode<T> *node);

private:
	int keyCounter = 1;
	QVector<GraphNode<T>*> *nodeList = nullptr;

	GraphNode<T>* startNode = nullptr;
	GraphNode<T>* endNode = nullptr;
};

#endif // GRAPH_H
