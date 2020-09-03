#include "graph.h"

Graph::Graph()
{
	nodeList = new QVector<GraphNode*>();
}

Graph::~Graph()
{
	Clear();
}

GraphNode* Graph::AddNode()
{
	return AddNode(nullptr);
}

GraphNode* Graph::AddNode(void *data)
{
	if(nodeList == nullptr)
	{
		return nullptr;
	}
	GraphNode *node = new GraphNode(keyCounter, data);
	keyCounter++;
	nodeList->push_back(node);
	return node;
}

void Graph::AddConnectionByIndex(int nodeIndex1, int nodeIndex2)
{
	GraphNode *node1 = at(nodeIndex1);
	GraphNode *node2 = at(nodeIndex2);
	node1->AddConnection(node2);
	node2->AddConnection(node1);
}

void Graph::RemoveConnectionByIndex(int nodeIndex1, int nodeIndex2)
{
	GraphNode *node1 = at(nodeIndex1);
	GraphNode *node2 = at(nodeIndex2);
	node1->RemoveConnection(node2);
	node2->RemoveConnection(node1);
}

void Graph::AddConnectionByKey(int key1, int key2)
{
	int index1 = GetNodeIndexByKey(key1);
	int index2 = GetNodeIndexByKey(key2);
	AddConnectionByIndex(index1, index2);
}

void Graph::RemoveConnectionByKey(int key1, int key2)
{
	int index1 = GetNodeIndexByKey(key1);
	int index2 = GetNodeIndexByKey(key2);
	RemoveConnectionByIndex(index1, index2);
}

void Graph::RemoveNodeByIndex(int index)
{
	if(nodeList == nullptr)
	{
		return;
	}

	if(index < 0 || index >= nodeList->length())
	{
		return;
	}

	GraphNode *node = at(index);

	for(int i = 0; i < node->ConnetionsCount(); i++)
	{
		GraphNode *connection = node->ConnectionAt(i);
		connection->RemoveConnection(node);
	}

	nodeList->removeAt(index);

	delete node;
}

void Graph::RemoveNodeByKey(int key)
{
	int index = GetNodeIndexByKey(key);
	RemoveNodeByIndex(index);
}

GraphNode *Graph::at(int i)
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

int Graph::length()
{
	if(nodeList == nullptr)
	{
		return 0;
	}
	return nodeList->length();
}

int Graph::GetNodeIndexByKey(int key)
{
	for(int i = 0; i < length(); i++)
	{
		GraphNode *node = at(i);
		if(node->GetKey() == key)
		{
			return i;
		}
	}

	return -1;
}

GraphNode *Graph::GetNodeByKey(int key)
{
	int index = GetNodeIndexByKey(key);
	if(index == -1)
	{
		return nullptr;
	}
	return at(index);
}

void Graph::Clear()
{
	for(int i = 0; i < length(); i ++)
	{
		delete nodeList->at(i);
	}

	delete nodeList;

	nodeList = new QVector<GraphNode*>;
	keyCounter = 1;
}

void Graph::SetStartNode(GraphNode *node)
{
	startNode = node;
}

void Graph::SetEndNode(GraphNode *node)
{
	endNode = node;
}

GraphNode *Graph::GetStartNode()
{
	return startNode;
}

GraphNode *Graph::GetEndNode()
{
	return endNode;
}
