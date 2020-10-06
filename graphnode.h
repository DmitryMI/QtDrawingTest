#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QVector>


/* All code of this class is placed in single .h file
 * There is no associated .cpp file for this class
 * Header-CPP problem is widely discussed
 * I just wanted to show that it is possible to do so.
 * Here is a discussion, advisable to read:
 * https://stackoverflow.com/questions/583255/is-it-a-good-practice-to-place-c-definitions-in-header-files
*/

/* Do not allocate memory for GraphNode manually
 * Memory management is delegated to Graph class
 * GraphNode::~GraphNode DOES NOT deallocate connected nodes
*/
template <class T>
class GraphNode
{
public:
	GraphNode(int key, T data )
	{
		this->key = key;
		this->data = data;
		connectionList = new QVector<GraphNode*>();
	}

	~GraphNode()
	{
		delete connectionList;
		connectionList = nullptr;
	}

	int GetKey()
	{
		return key;
	}

    void SetKey(int key)
    {
        this->key = key;
    }

	T GetData()
	{
		return data;
	}

	void SetData(T data)
	{
		this->data = data;
	}

	void AddConnection(GraphNode* node)
	{
		if(connectionList == nullptr)
		{
			return;
		}
		if(IsConnected(node))
		{
			return;
		}
		connectionList->push_back(node);
	}

	void RemoveConnection(GraphNode *node)
	{
		if(connectionList == nullptr)
		{
			return;
		}
		connectionList->removeOne(node);
	}

    void RemoveConnection(int index)
    {
        if(connectionList == nullptr)
        {
            return;
        }
        connectionList->removeAt(index);
    }

	bool IsConnected(GraphNode* node)
	{
		if(connectionList == nullptr)
		{
			return false;
		}
		return connectionList->contains(node);
	}

	int ConnetionsCount()
	{
		if(connectionList == nullptr)
		{
			return 0;
		}
		return connectionList->length();
	}

	GraphNode* ConnectionAt(int i)
	{
		if(connectionList == nullptr)
		{
			return nullptr;
		}
		if(i >= connectionList->length())
		{
			return nullptr;
		}
		return connectionList->at(i);
	}


private:
	int key;
	T data;
	QVector<GraphNode*> *connectionList = nullptr;
};

#endif // GRAPHNODE_H
