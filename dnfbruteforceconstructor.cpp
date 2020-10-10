#include "dnfbruteforceconstructor.h"
#include "setutils.h"

DnfBruteforceConstructor::DnfBruteforceConstructor()
{

}

LogicEquation *DnfBruteforceConstructor::GetPerfectDisjunctiveNormalForm(LogicEquation* sourceEquation)
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
                    result->AddEquationNode(disjunction);
                }

                if(prevDisjunction == nullptr)
                {
                    prevDisjunction = disjunction;
                }
                else
                {
                    prevDisjunction = new LogicEquationNode(LogicOperation::Disjunction, prevDisjunction, disjunction);
                   result->AddEquationNode(prevDisjunction);
                }
            }

            if(prevConjunction == nullptr)
            {
                prevConjunction = prevDisjunction;
            }
            else
            {
                prevConjunction = new LogicEquationNode(LogicOperation::Conjunction, prevConjunction, prevDisjunction);
                result->AddEquationNode(prevConjunction);
            }
        }
    }

    delete[] input;

    result->SetRootNode(prevConjunction);
    return result;
}


LogicEquationNode*  DnfBruteforceConstructor::ConstructMultiplicative(LogicEquation* equation, Path<NetParams> *path)
{
    LogicEquationNode *prev = nullptr;
    for(int j = 0; j < path->length(); j++)
    {
        GraphNode<NetParams> *node = path->at(j);
        int nodeKey = node->GetKey();
        LogicEquationNode *multiplier = equation->AddUniquePrimitive(nodeKey);
        if(prev == nullptr)
        {
            prev = multiplier;
        }
        else
        {
            LogicEquationNode *multiplication = new LogicEquationNode(Disjunction, prev, multiplier);
            equation->AddEquationNode(multiplication);
            prev = multiplication;
        }
    }

    return prev;
}

void DnfBruteforceConstructor::Construct(LogicEquation* equation, QVector<Path<NetParams> *> *pathList)
{
    LogicEquationNode *prev = nullptr;

    for(int i = 0; i < pathList->length(); i++)
    {
        Path<NetParams> *path = pathList->at(i);

        LogicEquationNode* disjunction = ConstructMultiplicative(equation, path);
        //equation->AddEquationNode(disjunction);

        if(prev == nullptr)
        {
            prev = disjunction;
        }
        else
        {
            LogicEquationNode *conjunction = new LogicEquationNode(Conjunction, prev, disjunction);
            equation->AddEquationNode(conjunction);
            prev = conjunction;
        }
    }

    equation->SetRootNode(prev);
}

LogicEquation* DnfBruteforceConstructor::GetPdnf(QVector<Path<NetParams>*> *pathList)
{    
    QVector<Path<NetParams>*> listClone = QVector<Path<NetParams>*>();
    SetUtils::CopyListContents(pathList, &listClone);

    SetUtils::RemoveComplexPaths(&listClone);

    LogicEquation *dnf = new LogicEquation();
    Construct(dnf, &listClone);
    LogicEquation *pdnf = GetPerfectDisjunctiveNormalForm(dnf);
    delete dnf;

    return pdnf;
}
