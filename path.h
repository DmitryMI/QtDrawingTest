#ifndef PATH_H
#define PATH_H

#include "graphnode.h"



class Path
{
public:
	Path()
	{
		path = new QVector<GraphNode*>();
	}

	~Path()
	{
		delete path;
	}

	int length()
	{
		return path->length();
	}

	GraphNode* at(int i)
	{
		return path->at(i);
	}

	void AddPathFragment(GraphNode* node)
	{
		path->push_back(node);
	}

	void AddPathFragment(QVector<GraphNode*> *fragment)
	{
		if(fragment == nullptr)
		{
			return;
		}
		for(int i = 0; i < fragment->length(); i++)
		{
			AddPathFragment(fragment->at(i));
		}
	}

	void AddPathFragment(Path *fragment)
	{
		if(fragment == nullptr)
		{
			return;
		}
		for(int i = 0; i < fragment->length(); i++)
		{
			AddPathFragment(fragment->at(i));
		}
	}

	bool Contains(GraphNode *node)
	{
		return path->contains(node);
	}

private:
	QVector<GraphNode*> *path = nullptr;
};

#endif // PATH_H
