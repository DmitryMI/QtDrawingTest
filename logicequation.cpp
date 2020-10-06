#include "graph.h"
#include "logicequation.h"
#include <cmath>

LogicEquation::LogicEquation(QVector<Path<NetParams>*> *pathList)
{
    coreNode = nullptr;
    equationNodes = new QVector<LogicEquationNode*>();
    primitivesList = new QVector<LogicEquationNode*>();

    Construct(pathList);
}

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

void LogicEquation::Construct(QVector<Path<NetParams> *> *pathList)
{
    LogicEquationNode *prev = nullptr;

    for(int i = 0; i < pathList->length(); i++)
    {
        Path<NetParams> *path = pathList->at(i);

        LogicEquationNode* disjunction = ConstructMultiplicative(path);
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

LogicEquationNode*  LogicEquation::ConstructMultiplicative(Path<NetParams> *path)
{
    LogicEquationNode *prev = nullptr;
    for(int j = 0; j < path->length(); j++)
    {
        GraphNode<NetParams> *node = path->at(j);
        int nodeKey = node->GetKey();
        LogicEquationNode *multiplier = AddUniquePrimitive(nodeKey);
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

LogicEquation *LogicEquation::GetPerfectDisjunctiveNormalForm(LogicEquation* sourceEquation)
{
    LogicEquation *result = new LogicEquation();

    int eventsCount = sourceEquation->getEventsCount();
    bool *input = new bool[eventsCount];

    int variationsCount = 1 << eventsCount; // 2 ^ eventsCount;

    LogicEquationNode *prevConjunction = nullptr;

    for(int mask = 0; mask < variationsCount; mask++)
    {
        for(int i = 0; i < eventsCount; i++)
        {
            input[i] = mask & (1 << i);
        }

        bool combinationResult = sourceEquation->Resolve(input, eventsCount);
        if(combinationResult == true)
        {
            LogicEquationNode *prevDisjunction = nullptr;
            for(int i = 0; i < eventsCount; i++)
            {
                LogicEquationNode *disjunction;
                if(input[i])
                {
                    disjunction = result->AddUniquePrimitive(i);
                }
                else
                {
                    LogicEquationNode *negatedEvent = result->AddUniquePrimitive(i);
                    disjunction = new LogicEquationNode(LogicOperation::Negation, negatedEvent, nullptr);
                    result->equationNodes->append(disjunction);
                }

                if(prevDisjunction == nullptr)
                {
                    prevDisjunction = disjunction;
                }
                else
                {
                    prevDisjunction = new LogicEquationNode(LogicOperation::Disjunction, prevDisjunction, disjunction);
                    result->equationNodes->append(prevDisjunction);
                }
            }

            if(prevConjunction == nullptr)
            {
                prevConjunction = prevDisjunction;
            }
            else
            {
                prevConjunction = new LogicEquationNode(LogicOperation::Conjunction, prevConjunction, prevDisjunction);
                result->equationNodes->append(prevConjunction);
            }
        }
    }

    delete[] input;

    result->coreNode = prevConjunction;
    return result;
}

double LogicEquation::GetProbability(EventProbabilityProvider *provider)
{
    LogicEquation *dnf = LogicEquation::GetPerfectDisjunctiveNormalForm(this);

    double result = dnf->ResolveProbability(provider);

    delete dnf;
    return result;
}
