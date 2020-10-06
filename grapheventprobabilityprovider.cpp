#include "grapheventprobabilityprovider.h"

GraphEventProbabilityProvider::GraphEventProbabilityProvider(Graph<NetParams> *graph)
{
    this->graph = graph;
}

double GraphEventProbabilityProvider::GetEventProbability(int index)
{
    GraphNode<NetParams> *node = graph->GetNodeByKey(index);
    return node->GetData().GetProbability();
}
