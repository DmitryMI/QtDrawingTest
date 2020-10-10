#include "dnfanalyticalconstructor.h"
#include "setutils.h"

#include <QMessageBox>
#include <QQueue>
#include <QSet>
#include <QStack>
#include <mutex>
#include <shared_mutex>
#include <thread>

std::mutex operandsQueueMutex;
std::shared_timed_mutex appendixCombinationListMutex;
std::shared_timed_mutex conjunctionListMutex;



DnfAnalyticalConstructor::DnfAnalyticalConstructor()
{

}


void FreeVectorMatrix(QVector<QVector<int>*> *vectorMatrix)
{
    for(int i = 0; i < vectorMatrix->length(); i++)
    {
        delete (vectorMatrix->at(i));
    }

    delete vectorMatrix;
}

void SortedInsertUnique(QVector<int> *nodeIndexList, int value)
{
    if(nodeIndexList->length() == 0)
    {
        nodeIndexList->append(value);
        return;
    }

    int index = 0;
    while(index < nodeIndexList->length())
    {
        int valueAt = nodeIndexList->at(index);
        int comparison = value - valueAt;
        if(comparison > 0)
        {
            index++;
        }
        else if(comparison < 0)
        {
            break;
        }
        else
        {
            return;
        }
    }
    nodeIndexList->insert(index, value);
}

void GetNodeSet(QVector<Path<NetParams>*> pathList, QVector<int> *nodeIndexList)
{
    for(int i = 0; i < pathList.length(); i++)
    {
        Path<NetParams>* path = pathList.at(i);
        for(int j = 0; j < path->length(); j++)
        {
            GraphNode<NetParams> *node = path->at(j);
            int nodeKey = node->GetKey();
            SortedInsertUnique(nodeIndexList, nodeKey);
        }
    }
}

int GetNodeIndexByKey(Path<NetParams> *path, int key)
{
    for(int j = 0; j < path->length(); j++)
    {
        auto node = path->at(j);
        int nodeKey = node->GetKey();
        if(nodeKey == key)
        {
            return j;
            break;
        }
    }

    return -1;
}

void GetPathNodes(Path<NetParams> *path, QVector<int> *nodeSet, QVector<int> *presentNodes, QVector<int> *missingNodes)
{
    for(int i = 0; i < nodeSet->length(); i++)
    {
        int value = nodeSet->at(i);
        int index = GetNodeIndexByKey(path, value);
        if(index == -1)
        {
            if(missingNodes != nullptr)
            {
                missingNodes->append(value);
            }
        }
        else
        {
            if(presentNodes != nullptr)
            {
                presentNodes->append(value);
            }
        }
    }
}

void GetSetPresentMissing(QVector<int> *set, QVector<int> *presentNodes, QVector<int> *missingNodes, QVector<int> *fullSet)
{
    for(int i = 0; i < fullSet->length(); i++)
    {
        int value = fullSet->at(i);
        int index = SetUtils::SetIndexOf(set, value);
        if(index == -1)
        {
            if(missingNodes != nullptr)
            {
                missingNodes->append(value);
            }
        }
        else
        {
            if(presentNodes != nullptr)
            {
                presentNodes->append(value);
            }
        }
    }
}

void AppendInvertedNodes(QVector<int> *presentNodes, QVector<int> *missingNodes)
{
    for(int i = 0; i < missingNodes->length(); i++)
    {
        int nodeKey = missingNodes->at(i);
        presentNodes->append(-nodeKey);
    }
}



LogicEquationNode* BuildDisjunction(LogicEquation* equation, QVector<int>* disjunction)
{
    LogicEquationNode *prev = nullptr;
    for(int i = 0 ; i < disjunction->length(); i++)
    {
        int eventIndex = disjunction->at(i);
        LogicEquationNode* operand;
        if(eventIndex < 0)
        {
            eventIndex = -eventIndex;
            operand = equation->AddUniquePrimitive(eventIndex);
            LogicEquationNode *negation = new LogicEquationNode(LogicOperation::Negation, operand, nullptr);
            equation->AddEquationNode(negation);
            operand = negation;
        }
        else
        {
            operand = equation->AddUniquePrimitive(eventIndex);
        }

        if(prev == nullptr)
        {
            prev = operand;
        }
        else
        {
            LogicEquationNode *disjunction = new LogicEquationNode(LogicOperation::Disjunction, prev, operand);
            equation->AddEquationNode(disjunction);
            prev = disjunction;
        }
    }

    return prev;
}

LogicEquation* BuildEquation( QVector<QVector<int>*> *conjunctionList)
{
    LogicEquation *equation = new LogicEquation();
    LogicEquationNode *prev = nullptr;
    for(int i = 0; i < conjunctionList->length(); i++)
    {
        QVector<int>* disjunction = conjunctionList->at(i);
        LogicEquationNode* disjunctionNode = BuildDisjunction(equation, disjunction);
        equation->AddEquationNode(disjunctionNode);
        if(prev == nullptr)
        {
            prev = disjunctionNode;
        }
        else
        {
            LogicEquationNode *conjunction = new LogicEquationNode(LogicOperation::Conjunction, prev, disjunctionNode);
            equation->AddEquationNode(conjunction);
            prev = conjunction;
        }
    }

    equation->SetRootNode(prev);

    return equation;
}

QString PrintPdnf(QVector<QVector<int>*> *conjunctionList)
{
    char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    QString string = "";
    for(int i = 0; i < conjunctionList->length(); i++)
    {
        QVector<int>* disjunction = conjunctionList->at(i);
        for(int j = 0; j < disjunction->length(); j++)
        {
            int eventIndex = disjunction->at(j);
            int absEvent = abs(eventIndex);
            char symbol = toupper(letters[absEvent]);
            if(eventIndex < 0)
            {
                string += '-';
            }
            string += symbol;
        }
        if(i < conjunctionList->length() - 1)
        {
            string += " + ";
        }
    }

    return string;
}

void RemoveSupersets( QVector<QVector<int>*> *conjunctionList)
{
    for(int i = 0; i < conjunctionList->count(); i++)
    {
        QVector<int> *current = conjunctionList->at(i);
        for(int j = i + 1; j < conjunctionList->count(); j++)
        {
            QVector<int> *comparable = conjunctionList->at(j);
            SetComparisonResult comparisonResult = SetUtils::CompareSets(current, comparable);

            if(comparisonResult == SetComparisonResult::Less)
            {
                conjunctionList->removeAt(j);
                j--;
            }
            else if(comparisonResult == SetComparisonResult::Equal)
            {
                conjunctionList->removeAt(j);
                j--;
            }
            else if(comparisonResult == SetComparisonResult::Greater)
            {
                conjunctionList->removeAt(i);
                i--;
                break;
            }
            else
            {
                // Uncomparible, do nothing
            }
        }
    }
}

bool ContainsSet(QVector<QVector<int>*> *setList, QVector<int>* set)
{
    for(int i = 0; i < setList->length(); i++)
    {
        QVector<int>* listAt = setList->at(i);
        int areEqual = true;
        for(int j = 0; j < listAt->length(); j++)
        {
            if(listAt->at(j) != set->at(j))
            {
                areEqual = false;
                break;
            }
        }

        if(areEqual)
        {
            return true;
        }
    }

    return false;
}

bool ContainsSuperset(QVector<QVector<int>> *setList, QVector<int>* set)
{
    for(int i = 0; i < setList->length(); i++)
    {
        QVector<int> listAt = setList->at(i);
        if(SetUtils::IsSubset(set, &listAt))
        {
            return true;
        }
    }

    return false;
}

void ProcessOperand(QVector<QVector<int> *> *conjunctionList, int nodeIndex, QVector<int> *nodeIndexSet, QVector<QVector<int>> *appendixCombinationList)
{
    QVector<int> *operand = conjunctionList->at(nodeIndex);
    QVector<int> presentNodes = QVector<int>();
    QVector<int> missingNodes = QVector<int>();
    GetSetPresentMissing(operand, &presentNodes, &missingNodes, nodeIndexSet);
    if(missingNodes.length() == 0)
    {
        return;
    }
    AppendInvertedNodes(operand, &missingNodes);

    uint64_t combinations = (1 << missingNodes.length()) - 1;
    for(uint64_t combination = 1; combination < combinations; combination++)
    {
        QVector<int> *appendixOperand = new QVector<int>();
        QVector<int> combinationVector = QVector<int>();
        for(int k = 0; k < missingNodes.length(); k++)
        {
            int nodeIndex;
            if(combination & (1 << k))
            {
                nodeIndex =  missingNodes[k];
            }
            else
            {
                nodeIndex = -missingNodes[k];
                combinationVector.append(missingNodes[k]);
            }
            appendixOperand->append(nodeIndex);
        }

        bool supersetFound = ContainsSuperset(appendixCombinationList, &combinationVector);

        if(supersetFound)
        {
            delete appendixOperand;
        }
        else
        {
            SetUtils::CopyListContents(&presentNodes, appendixOperand);
            conjunctionList->append(appendixOperand);
        }
    }
    appendixCombinationList->append(missingNodes);
}

void DnfAnalyticalConstructor::GetPdnfConjunction(QVector<Path<NetParams> *> *pathList, QVector<QVector<int> *> *conjunctionList)
{
    QVector<Path<NetParams>*> listClone = QVector<Path<NetParams>*>();
    SetUtils::CopyListContents(pathList, &listClone);

    SetUtils::RemoveComplexPaths(&listClone);

    QVector<int> *nodeIndexSet = new QVector<int>();
    GetNodeSet(listClone, nodeIndexSet);

    for(int i = 0; i < listClone.length(); i++)
    {
        Path<NetParams> *path = listClone.at(i);
        QVector<int> *conjunctionOperand = new QVector<int>();
        GetPathNodes(path, nodeIndexSet, conjunctionOperand, nullptr);
        conjunctionList->append(conjunctionOperand);
    }

    int operandsCount = conjunctionList->length();

    QVector<QVector<int>> appendixCombinationList = QVector<QVector<int>>();

    for(int i = 0; i < operandsCount; i++)
    {
        ProcessOperand(conjunctionList, i, nodeIndexSet, &appendixCombinationList);
    }
    conjunctionList->append(nodeIndexSet);

    //QString afterReduction = PrintPdnf(conjunctionList);

    //QMessageBox::information(nullptr, "LogicEquation", afterReduction);
}

void DnfAnalyticalConstructor::GetPdnfConjunction(QVector<Path<NetParams> *> *pathList, QVector<QVector<int> *> *resultVectorMatrix, int threads_count)
{
    GetPdnfConjunction(pathList, resultVectorMatrix);
}


LogicEquation *DnfAnalyticalConstructor::GetPdnfTree(QVector<Path<NetParams> *> *pathList)
{
    QVector<QVector<int> *> *conjunctionList = new QVector<QVector<int>*>();
    GetPdnfConjunction(pathList, conjunctionList);

    LogicEquation *equation = BuildEquation(conjunctionList);

    FreeVectorMatrix(conjunctionList);

    return equation;
}

LinearLogicEquation *DnfAnalyticalConstructor::GetPdnfLinear(QVector<Path<NetParams> *> *pathList, int thread_count)
{
    QVector<QVector<int> *> *conjunctionList = new QVector<QVector<int>*>();
    GetPdnfConjunction(pathList, conjunctionList, thread_count);

    LinearLogicEquation *equation = new LinearLogicEquation(conjunctionList);

    FreeVectorMatrix(conjunctionList);

    return equation;
}


