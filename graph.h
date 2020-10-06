#ifndef GRAPH_H
#define GRAPH_H

#include "eventprobabilityprovider.h"
#include "graphnode.h"

#include <qvector.h>

template <class T>
class Graph
{
public:
	Graph();

	~Graph();

	GraphNode<T>* AddNode();
	GraphNode<T>* AddNode(T data);	

	void AddConnectionByKey(int key1, int key2);

	void RemoveConnectionByKey(int key1, int key2);	

	void RemoveNodeByKey(int key);

	void RemoveNode(GraphNode<T>* node);

	void AddConnection(GraphNode<T>* a, GraphNode<T>* b);

	void RemoveConnection(GraphNode<T> *a, GraphNode<T>* b);

	GraphNode<T>* at(int i);

	int length();	

	GraphNode<T>* GetNodeByKey(int key);

	void Clear();		

	GraphNode<T>* GetStartNode();
	GraphNode<T>* GetEndNode();

    void SetStartNodeByKey(int nodeKey);
    void SetEndNodeByKey(int nodeKey);

    int GetNodeKey(GraphNode<T> *node);

private:
    int keyCounter = 0;
	QVector<GraphNode<T>*> *nodeList = nullptr;

	GraphNode<T>* startNode = nullptr;
	GraphNode<T>* endNode = nullptr;

    void SetStartNode(GraphNode<T>* node);
    void SetEndNode(GraphNode<T> *node);
};

#endif // GRAPH_H
