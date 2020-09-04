#ifndef PATH_H
#define PATH_H

#include "graphnode.h"


template<class T>
class Path
{
public:
	Path()
	{
		path = new QVector<GraphNode<T>*>();
	}

	~Path()
	{
		delete path;
	}

	int length()
	{
		return path->length();
	}

	GraphNode<T>* at(int i)
	{
		return path->at(i);
	}

	void AddPathFragment(GraphNode<T>* node)
	{
		path->push_back(node);
	}

	void AddPathFragment(QVector<GraphNode<T>*> *fragment)
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

	bool Contains(GraphNode<T> *node)
	{
		return path->contains(node);
	}

private:
	QVector<GraphNode<T>*> *path = nullptr;
};

#endif // PATH_H
