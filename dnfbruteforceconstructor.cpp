#include "dnfbruteforceconstructor.h"
#include "setutils.h"
#include <thread>

std::mutex m_mutex;


DnfBruteforceConstructor::DnfBruteforceConstructor()
{

}

void DnfBruteforceConstructor::ProcessVariations(LogicEquation *result, LogicEquation* sourceEquation, LogicEquationNode **outOperand, int maskStart, int maskEnd)
{
    int eventsCount = sourceEquation->getEventsCount();
    bool *input = new bool[eventsCount];
    LogicEquationNode *prevConjunction = nullptr;

    for(int mask = maskStart; mask <= maskEnd; mask++)
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
                    std::lock_guard<std::mutex> lock(m_mutex);
                    disjunction = result->AddUniquePrimitive(i);
                }
                else
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
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
                    std::lock_guard<std::mutex> lock(m_mutex);
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
                std::lock_guard<std::mutex> lock(m_mutex);
                result->AddEquationNode(prevConjunction);
            }
        }
    }

    *outOperand = prevConjunction;

    delete[] input;
}

LogicEquation *DnfBruteforceConstructor::GetPerfectDisjunctiveNormalForm(LogicEquation* sourceEquation, int threads_count)
{
    LogicEquation *result = new LogicEquation();

    int eventsCount = sourceEquation->getEventsCount();

    int variationsCount = 1 << eventsCount; // 2 ^ eventsCount;

    int variationsPerThread = variationsCount / threads_count;

    QVector<std::thread*> threadList = QVector<std::thread*>();
    QVector<LogicEquationNode**> operandsList = QVector<LogicEquationNode**>();

    int variationsLeft = variationsCount;
    for(int i = 0; i < threads_count; i++)
    {
        int maskStart = variationsPerThread * i;
        int maskEnd = maskStart + variationsPerThread;
        variationsLeft -= variationsPerThread;
        LogicEquationNode **outOperand = new LogicEquationNode*;
        operandsList.append(outOperand);
        std::thread *thr = new std::thread(ProcessVariations, result, sourceEquation, outOperand, maskStart, maskEnd);
        threadList.append(thr);
    }

    if(variationsLeft > 0)
    {
        int maskEnd = variationsCount;
        int maskStart = variationsCount - variationsLeft;
        LogicEquationNode **outOperand = new LogicEquationNode*;
        operandsList.append(outOperand);
        std::thread *thr = new std::thread(ProcessVariations, result, sourceEquation, outOperand, maskStart, maskEnd);
        threadList.append(thr);
    }

    for(int i = 0; i < threadList.length(); i++)
    {
        std::thread *thr = threadList.at(i);
        thr->join();
        delete thr;
    }

    LogicEquationNode *prevOperand = nullptr;
    for(int i = 0; i < operandsList.length(); i++)
    {
        LogicEquationNode **operandPointer = operandsList[i];
        LogicEquationNode *operand = *operandPointer;
        delete operandPointer;
        if(operand == nullptr)
        {
            continue;
        }
        if(prevOperand == nullptr)
        {
            prevOperand = operand;
        }
        else
        {
            LogicEquationNode *conjunction = new LogicEquationNode(LogicOperation::Conjunction, prevOperand, operand);
            result->AddEquationNode(conjunction);
            prevOperand = conjunction;
        }
    }

    result->SetRootNode(prevOperand);
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

LogicEquation* DnfBruteforceConstructor::GetPdnf(QVector<Path<NetParams>*> *pathList, int thread_count)
{    
    QVector<Path<NetParams>*> listClone = QVector<Path<NetParams>*>();
    SetUtils::CopyListContents(pathList, &listClone);

    SetUtils::RemoveComplexPaths(&listClone);

    LogicEquation *dnf = new LogicEquation();
    Construct(dnf, &listClone);

    LogicEquation *pdnf = GetPerfectDisjunctiveNormalForm(dnf, thread_count);

    delete dnf;

    return pdnf;
}
