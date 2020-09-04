#include "graph.h"
#include "netparams.h"

template <class T>
Graph<T>::Graph()
{
	nodeList = new QVector<GraphNode<T>*>();
}

template <class T>
Graph<T>::~Graph()
{
	Clear();
}

template <class T>
GraphNode<T>* Graph<T>::AddNode()
{
	return AddNode(NetParams());
}

template <class T>
GraphNode<T>* Graph<T>::AddNode(T data)
{
	if(nodeList == nullptr)
	{
		return nullptr;
	}
	GraphNode<T> *node = new GraphNode<T>(keyCounter, data);
	keyCounter++;
	nodeList->push_back(node);
	return node;
}

template <class T>
void Graph<T>::AddConnectionByIndex(int nodeIndex1, int nodeIndex2)
{
	GraphNode<T> *node1 = at(nodeIndex1);
	GraphNode<T> *node2 = at(nodeIndex2);
	node1->AddConnection(node2);
	node2->AddConnection(node1);
}

template <class T>
void Graph<T>::RemoveConnectionByIndex(int nodeIndex1, int nodeIndex2)
{
	GraphNode<T> *node1 = at(nodeIndex1);
	GraphNode<T> *node2 = at(nodeIndex2);
	node1->RemoveConnection(node2);
	node2->RemoveConnection(node1);
}

template <class T>
void Graph<T>::AddConnectionByKey(int key1, int key2)
{
	int index1 = GetNodeIndexByKey(key1);
	int index2 = GetNodeIndexByKey(key2);
	AddConnectionByIndex(index1, index2);
}

template <class T>
void Graph<T>::RemoveConnectionByKey(int key1, int key2)
{
	int index1 = GetNodeIndexByKey(key1);
	int index2 = GetNodeIndexByKey(key2);
	RemoveConnectionByIndex(index1, index2);
}

template <class T>
void Graph<T>::RemoveNodeByIndex(int index)
{
	if(nodeList == nullptr)
	{
		return;
	}

	if(index < 0 || index >= nodeList->length())
	{
		return;
	}

	GraphNode<T> *node = at(index);

	for(int i = 0; i < node->ConnetionsCount(); i++)
	{
		GraphNode<T> *connection = node->ConnectionAt(i);
		connection->RemoveConnection(node);
	}

	nodeList->removeAt(index);

	delete node;
}

template <class T>
void Graph<T>::RemoveNodeByKey(int key)
{
	int index = GetNodeIndexByKey(key);
	RemoveNodeByIndex(index);
}

template <class T>
GraphNode<T> *Graph<T>::at(int i)
{
	if(nodeList == nullptr)
	{
		return nullptr;
	}
	if(i >= nodeList->length())
	{
		return nullptr;
	}
	return nodeList->at(i);
}

template <class T>
int Graph<T>::length()
{
	if(nodeList == nullptr)
	{
		return 0;
	}
	return nodeList->length();
}

template <class T>
int Graph<T>::GetNodeIndexByKey(int key)
{
	for(int i = 0; i < length(); i++)
	{
		GraphNode<T> *node = at(i);
		if(node->GetKey() == key)
		{
			return i;
		}
	}

	return -1;
}

template <class T>
GraphNode<T> *Graph<T>::GetNodeByKey(int key)
{
	int index = GetNodeIndexByKey(key);
	if(index == -1)
	{
		return nullptr;
	}
	return at(index);
}

template <class T>
void Graph<T>::Clear()
{
	for(int i = 0; i < length(); i ++)
	{
		delete nodeList->at(i);
	}

	delete nodeList;

	nodeList = new QVector<GraphNode<T>*>;
	keyCounter = 1;
}

template <class T>
void Graph<T>::SetStartNode(GraphNode<T> *node)
{
	startNode = node;
}

template <class T>
void Graph<T>::SetEndNode(GraphNode<T> *node)
{
	endNode = node;
}

template <class T>
void Graph<T>::SetStartNodeByIndex(int nodeIndex)
{
	if(nodeIndex != -1)
	{
		startNode = nodeList->at(nodeIndex);
	}
	else
	{
		startNode = nullptr;
	}
}

template <class T>
void Graph<T>::SetEndNodeByIndex(int nodeIndex)
{
	if(nodeIndex != -1)
	{
		endNode = nodeList->at(nodeIndex);
	}
	else
	{
		endNode = nullptr;
	}
}

template <class T>
GraphNode<T> *Graph<T>::GetStartNode()
{
	return startNode;
}

template <class T>
GraphNode<T> *Graph<T>::GetEndNode()
{
	return endNode;
}

template class Graph<NetParams>;
