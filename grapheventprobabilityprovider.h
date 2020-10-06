#ifndef GRAPHEVENTPROBABILITYPROVIDER_H
#define GRAPHEVENTPROBABILITYPROVIDER_H

#include "eventprobabilityprovider.h"
#include "graph.h"
#include "netparams.h"



class GraphEventProbabilityProvider : public EventProbabilityProvider
{
    Graph<NetParams> *graph;
public:
    GraphEventProbabilityProvider(Graph<NetParams> *graph);

    virtual double  GetEventProbability(int index);
};

#endif // GRAPHEVENTPROBABILITYPROVIDER_H
