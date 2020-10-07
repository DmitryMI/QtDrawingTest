#include "calculator.h"
#include "grapheventprobabilityprovider.h"
#include "logicequation.h"
#include "dnfanalyticalconstructor.h"
#include "dnfbruteforceconstructor.h"
#include<QDebug>

#define USE_ANALYTICAL 1



#define DELETE_VECTOR(vec) \
	for(int i = 0; i < vec->length(); i++)	\
	{										\
		delete vec->at(i);					\
	}										\
	delete vec;


double Calculator::GetMathematicalProbability(Graph<NetParams> *graph)
{

    QVector<Path<NetParams>*> *pathList = new QVector<Path<NetParams>*>();
    searcher->FindAllPaths(graph, pathList);

    int pathsCount = pathList->length();

    if(pathsCount == 0)
    {
        return 0;
    }

    GraphEventProbabilityProvider *provider = new GraphEventProbabilityProvider(graph);

    double result;
#if USE_ANALYTICAL == 1
    //LogicEquation *analitycallyDerivedEquation = DnfAnalyticalConstructor::GetPdnf(pathList);
    LinearLogicEquation *analitycallyDerivedEquation = DnfAnalyticalConstructor::GetPdnfLinear(pathList);
    double analitycallyDerivedResult = analitycallyDerivedEquation->GetProbability(provider);
    result = analitycallyDerivedResult;
#else
    LogicEquation *tableDerivedEquation = DnfBruteforceConstructor::GetPdnf(pathList);
    double tableDerivedResult = tableDerivedEquation->GetProbability(provider);
    result = tableDerivedResult;
#endif
    return result;
}

double Calculator::GetExperimentalProbability(Graph<NetParams> *graph, int experimentsCount)
{
	int successedExperiments = 0;
	for(int i = 0; i < experimentsCount; i++)
	{
		Graph<NetParams> graphClone = Graph<NetParams>();
		CloneGraph(graph, &graphClone);

		for(int j = 0; j < graphClone.length(); j++)
		{
            GraphNode<NetParams>* node = graphClone.at(j);
			double probability = node->GetData().GetProbability();
			bool isBroken = RollDice(1 - probability);
			if(isBroken)
			{
                graphClone.RemoveNode(node);
                j--;
			}
        }

		bool pathExists = searcher->PathExists(&graphClone);
		if(pathExists)
		{
			successedExperiments++;
		}
	}

    return ((double)successedExperiments) / experimentsCount;
}

void Calculator::CloneGraph(Graph<NetParams> *original, Graph<NetParams> *clone)
{
	for(int i = 0; i < original->length(); i++)
	{
		auto node = original->at(i);
		clone->AddNode(node->GetData());
	}

	for(int i = 0; i < original->length(); i++)
	{
        auto originalNode = original->at(i);

        for(int j = 0; j < originalNode->ConnetionsCount(); j++)
		{
            auto connection = originalNode->ConnectionAt(j);
            int connectionOriginalIndex = original->GetNodeKey(connection);

            clone->AddConnectionByKey(i, connectionOriginalIndex);
		}
	}

    auto startNode = original->GetStartNode();
    if(startNode == nullptr)
    {
        clone->SetStartNodeByKey(-1);
    }
    else
    {
        int startNodeIndex = original->GetNodeKey(startNode);
        clone->SetStartNodeByKey(startNodeIndex);
    }
    auto endNode = original->GetEndNode();
    if(endNode == nullptr)
    {
        clone->SetEndNodeByKey(-1);
    }
    else
    {
        int endNodeIndex = original->GetNodeKey(endNode);
        clone->SetEndNodeByKey(endNodeIndex);
    }
}

bool Calculator::RollDice(double probability)
{
	double currentRandomNumber = unif(rng);
	return currentRandomNumber < probability;
}

Calculator::Calculator(SearcherBase *searcher)
{
	this->searcher = searcher;

	// initialize the random number generator with time-dependent seed
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
	rng.seed(ss);

	unif = std::uniform_real_distribution<double>(0, 1);
}
