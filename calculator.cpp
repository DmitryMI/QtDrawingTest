#include "calculator.h"

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

	double allPathsFaultProbability = 1;

	for(int i = 0; i < pathsCount; i++)
	{
		Path<NetParams> *path = pathList->at(i);

		// Get working probability of each path
		double workingProbability = 1;
		for(int j = 0; j < path->length(); j++)
		{
			auto node = path->at(j);
			double probability = node->GetData().GetProbability();
			workingProbability *= probability;
		}

		double faultProbability = 1 - workingProbability;
		allPathsFaultProbability *= faultProbability;
	}

	double result = 1 - allPathsFaultProbability;

	DELETE_VECTOR(pathList);

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
			auto node = graphClone.at(j);
			double probability = node->GetData().GetProbability();
			bool isBroken = RollDice(1 - probability);
			if(isBroken)
			{
				graphClone.RemoveNode(node);
			}
		}

		bool pathExists = searcher->PathExists(&graphClone);
		if(pathExists)
		{
			successedExperiments++;
		}
	}

	return (double)successedExperiments / experimentsCount;
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
		auto node = original->at(i);
		for(int j = 0; j < node->ConnetionsCount(); j++)
		{
			auto connection = node->ConnectionAt(j);
			int connectionOriginalIndex = original->GetNodeIndex(connection);

			clone->AddConnectionByIndex(i, connectionOriginalIndex);
		}
	}

	clone->SetStartNode(original->GetStartNode());
	clone->SetEndNode(original->GetEndNode());
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
