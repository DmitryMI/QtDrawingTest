#include "graph.h"
#include "logicequation.h"
#include <cmath>

LogicEquation::~LogicEquation()
{
    for(int i = 0; i < equationNodes->length(); i++)
    {
        delete equationNodes->at(i);
    }
    delete equationNodes;
    delete primitivesList;
}

int LogicEquation::getEventsCount()
{
    return primitivesList->length();
}

int LogicEquation::getEquationNodesCount()
{
    return equationNodes->length();
}

bool LogicEquation::ResolveRecursively(LogicEquationNode *node, bool *input, int length)
{
    bool first, second;
    switch (node->getLogicOperation())
    {
    case LogicOperation::None:
        return input[node->getEventIndex()];
    case LogicOperation::Negation:
        return !ResolveRecursively(node->getFirstOperand(), input, length);
    case LogicOperation::Conjunction:
        first = ResolveRecursively(node->getFirstOperand(), input, length);
        second = ResolveRecursively(node->getSecondOperand(), input, length);
        return first || second;
    case LogicOperation::Disjunction:
        first = ResolveRecursively(node->getFirstOperand(), input, length);
        second = ResolveRecursively(node->getSecondOperand(), input, length);
        return first && second;
    default:
        return false;
    }
}

LogicEquationNode* LogicEquation::AddUniquePrimitive(int eventIndex)
{
    for(int i = 0; i < primitivesList->length(); i++)
    {
        LogicEquationNode *node = primitivesList->at(i);
        if(node->getEventIndex() == eventIndex)
        {
            return node;
        }
    }

    LogicEquationNode *node = new LogicEquationNode(eventIndex);

    primitivesList->append(node);
    equationNodes->append(node);

    return node;
}

void LogicEquation::AddEquationNode(LogicEquationNode *node)
{
    equationNodes->append(node);
}

void LogicEquation::SetRootNode(LogicEquationNode *node)
{
    coreNode = node;
}

LogicEquation::LogicEquation()
{
    coreNode = nullptr;
    equationNodes = new QVector<LogicEquationNode*>();
    primitivesList = new QVector<LogicEquationNode*>();
}

double LogicEquation::ResolveProbabilityRecursively(EventProbabilityProvider *provider, LogicEquationNode *node)
{
    double first, second;
    switch (node->getLogicOperation())
    {
    case None:
        return provider->GetEventProbability(node->getEventIndex());
        break;
    case Conjunction:
        first = ResolveProbabilityRecursively(provider, node->getFirstOperand());
        second = ResolveProbabilityRecursively(provider, node->getSecondOperand());
        return first + second;
        break;
    case Disjunction:
        first = ResolveProbabilityRecursively(provider, node->getFirstOperand());
        second = ResolveProbabilityRecursively(provider, node->getSecondOperand());
        return first * second;
        break;
    case Negation:
        first = ResolveProbabilityRecursively(provider, node->getFirstOperand());
        return 1 - first;
        break;

    }
}

double LogicEquation::ResolveProbability(EventProbabilityProvider *provider)
{
    return ResolveProbabilityRecursively(provider, coreNode);
}

LogicEquationNode *LogicEquation::FindPrimitive(int eventIndex)
{
    for(int i = 0; i < primitivesList->length(); i++)
    {
        LogicEquationNode *node = primitivesList->at(i);
        if(node->getEventIndex() == eventIndex)
        {
            return node;
        }
    }

    return nullptr;
}

bool LogicEquation::Resolve(bool *input, int length)
{
    return ResolveRecursively(coreNode, input, length);
}


double LogicEquation::GetProbability(EventProbabilityProvider *provider)
{
    //LogicEquation *dnf = LogicEquation::GetPerfectDisjunctiveNormalForm(this);
    LogicEquation *dnf = this;
    double result = dnf->ResolveProbability(provider);

    //delete dnf;
    return result;
}
