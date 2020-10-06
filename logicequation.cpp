#include "graph.h"
#include "logicequation.h"

LogicEquation::LogicEquation(EventProbabilityProvider *provider, QVector<Path<NetParams>*> *pathList)
{
    coreNode = nullptr;
    equationNodes = new QVector<LogicEquationNode*>();

    Construct(provider, pathList);
}

LogicEquation::~LogicEquation()
{
    for(int i = 0; i < equationNodes->length(); i++)
    {
        delete equationNodes->at(i);
    }
    delete equationNodes;
}

void LogicEquation::Construct(EventProbabilityProvider *provider, QVector<Path<NetParams> *> *pathList)
{
    LogicEquationNode *prev = nullptr;

    for(int i = 0; i < pathList->length(); i++)
    {
        Path<NetParams> *path = pathList->at(i);

        LogicEquationNode* disjunction = ConstructMultiplicative(provider, path);
        equationNodes->append(disjunction);

        if(prev == nullptr)
        {
            prev = disjunction;
        }
        else
        {
            LogicEquationNode *conjunction = new LogicEquationNode(Conjunction, prev, disjunction);
            equationNodes->append(conjunction);
            prev = conjunction;
        }
    }

    coreNode = prev;
}

LogicEquationNode*  LogicEquation::ConstructMultiplicative(EventProbabilityProvider *provider, Path<NetParams> *path)
{
    LogicEquationNode *prev = nullptr;
    for(int j = 0; j < path->length(); j++)
    {
        GraphNode<NetParams> *node = path->at(j);
        int nodeKey = node->GetKey();
        LogicEquationNode *multiplier = new LogicEquationNode(provider, nodeKey);
        equationNodes->append(multiplier);
        if(prev == nullptr)
        {
            prev = multiplier;
        }
        else
        {
            LogicEquationNode *multiplication = new LogicEquationNode(Disjunction, prev, multiplier);
            equationNodes->append(multiplier);
            prev = multiplication;
        }
    }

    return prev;
}

bool LogicEquation::ResolveRecursively(LogicEquationNode *node, bool *input, int length)
{

}

bool LogicEquation::Resolve(bool *input, int length)
{

}
