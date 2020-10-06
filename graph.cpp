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
void Graph<T>::AddConnectionByKey(int key1, int key2)
{
    GraphNode<T> *node1 = at(key1);
    GraphNode<T> *node2 = at(key2);
    node1->AddConnection(node2);
    node2->AddConnection(node1);
}

template <class T>
void Graph<T>::RemoveConnectionByKey(int key1, int key2)
{
    GraphNode<T> *node1 = at(key1);
    GraphNode<T> *node2 = at(key2);
    node1->RemoveConnection(node2);
    node2->RemoveConnection(node1);
}

template <class T>
void Graph<T>::RemoveNodeByKey(int key)
{
	if(nodeList == nullptr)
	{
		return;
	}

    if(key < 0 || key >= nodeList->length())
	{
		return;
	}

    GraphNode<T> *node = at(key);

	if(node == startNode)
	{
		startNode = nullptr;
	}
	if(node == endNode)
	{
		endNode = nullptr;
	}

	for(int i = 0; i < node->ConnetionsCount(); i++)
	{
        GraphNode<T> *connection = node->ConnectionAt(i);
        connection->RemoveConnection(node);
	}

    for(int i = 0; i < node->ConnetionsCount(); i++)
    {
        node->RemoveConnection(i);
    }

    nodeList->removeAt(key);

    for(int i = key; i < nodeList->length(); i++)
    {
        GraphNode<T> *node = nodeList->at(i);
        node->SetKey(i);
    }

	delete node;
}

template<class T>
void Graph<T>::RemoveNode(GraphNode<T> *node)
{
    int key = node->GetKey();
    RemoveNodeByKey(key);
}

template<class T>
void Graph<T>::AddConnection(GraphNode<T> *a, GraphNode<T> *b)
{
	if(nodeList->contains(a) && nodeList->contains(b))
	{
		a->AddConnection(b);
		b->AddConnection(a);
	}
}

template<class T>
void Graph<T>::RemoveConnection(GraphNode<T> *a, GraphNode<T> *b)
{
	a->RemoveConnection(b);
	b->RemoveConnection(a);
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
GraphNode<T> *Graph<T>::GetNodeByKey(int key)
{	
    return at(key);
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
void Graph<T>::SetStartNodeByKey(int nodeKey)
{
    if(nodeKey != -1)
	{
        startNode = nodeList->at(nodeKey);
	}
	else
	{
		startNode = nullptr;
	}
}

template <class T>
void Graph<T>::SetEndNodeByKey(int nodeKey)
{
    if(nodeKey != -1)
	{
        endNode = nodeList->at(nodeKey);
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

template<class T>
int Graph<T>::GetNodeKey(GraphNode<T> *node)
{
    return node->GetKey();
}

template class Graph<NetParams>;
