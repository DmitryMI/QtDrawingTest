#ifndef GRAPHEVENTPROBABILITYPROVIDER_H
#define GRAPHEVENTPROBABILITYPROVIDER_H

#include "eventprobabilityprovider.h"
#include "graph.h"
#include "netparams.h"



class GraphEventProbabilityProvider : public EventProbabilityProvider
{
    Graph<NetParams> *graph;
public:
    GraphEventProbabilityProvider(Graph<NetParams> *graph)
    {
        this->graph = graph;
    }

    virtual double  GetEventProbability(int index)
    {
       GraphNode<NetParams> *node = graph->GetNodeByKey(index);
       return node->GetData().GetProbability();
    }
};

#endif // GRAPHEVENTPROBABILITYPROVIDER_H
