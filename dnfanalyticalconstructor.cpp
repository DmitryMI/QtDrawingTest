#include "dnfanalyticalconstructor.h"
#include "setutils.h"

#include <QMessageBox>
#include <QQueue>
#include <QSet>
#include <QStack>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <cmath>

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



void GetNodeSet(QVector<Path<NetParams>*> pathList, QVector<int> *nodeIndexList)
{
    for(int i = 0; i < pathList.length(); i++)
    {
        Path<NetParams>* path = pathList.at(i);
        for(int j = 0; j < path->length(); j++)
        {
            GraphNode<NetParams> *node = path->at(j);
            int nodeKey = node->GetKey();
            SetUtils::SortedInsertUnique(nodeIndexList, nodeKey);
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

void PrintSet(QVector<int> *vector, char *buffer, int buf_len)
{
    char letters[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int i;
    for(i = 0; i < vector->length() && i < buf_len - 1; i++)
    {
        int eventIndex = vector->at(i);
        if(eventIndex >= 26)
        {
            buffer[i] = '#';
        }
        else
        {
            buffer[i] = toupper(letters[eventIndex]);
        }
    }

    if(i >= buf_len && i < vector->length())
    {
        buffer[i - 1] = '?';
    }

    buffer[i] = '\0';
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

QVector<int>* FindSuperset(QVector<QVector<int>*> *setList, QVector<int>* set)
{
    for(int i = 0; i < setList->length(); i++)
    {
        QVector<int> *listAt = setList->at(i);
        if(SetUtils::IsSubset(set, listAt))
        {
            return listAt;
        }
    }

    return nullptr;
}

int gcd (int a, int b) {
    while (a!=0 && b!=0) {
        if (a>b) {
            a=a%b;
        }
        else b=b%a;
    }
    return a+b;
}


int GetNcR(int n, int r)
{
    long long p = 1, k = 1;

    if (n - r < r)
        r = n - r;

    if (r != 0) {
        while (r) {
            p *= n;
            k *= r;

            long long m = gcd(p, k);
            p /= m;
            k /= m;

            n--;
            r--;
        }
    }

    else
        p = 1;
   return p;
}

int GetNcRAll(int n)
{
    int summ = 0;
    for(int i = 0; i < n; i++)
    {
        summ += GetNcR(n, i);
    }
    return summ;
}

int GetUnitPosition(uint64_t combination)
{
    int pos = 0;
    while(!(combination & 1))
    {
        combination >>= 1;
        pos++;
    }
    return pos;
}

int SkipSubsetCombinations(uint64_t combination)
{
    int mask = 1;
    while(!(combination & mask))
    {
        mask <<= 1;
    }
    return combination + mask;
}

void BuildCombination(QVector<int> *appendixOperand,  QVector<int> *combinationVector, QVector<int> *missingNodes, uint64_t combination)
{
    for(int k = 0; k < missingNodes->length(); k++)
    {
        int nodeIndex;
        if(combination & (1 << k))
        {
            nodeIndex =  missingNodes->at(k);
        }
        else
        {
            nodeIndex = -missingNodes->at(k);
            combinationVector->append(missingNodes->at(k));
        }
        if(appendixOperand != nullptr)
        {
            appendixOperand->append(nodeIndex);
        }
    }
}

void BuildCombination(QVector<int> *appendixOperand, QVector<int> *missingNodes, uint64_t combination)
{
    for(int k = 0; k < missingNodes->length(); k++)
    {
        int nodeIndex;
        if(combination & (1 << k))
        {
            nodeIndex =  missingNodes->at(k);
        }
        else
        {
            nodeIndex = -missingNodes->at(k);
        }
        appendixOperand->append(nodeIndex);
    }
}

bool HasNewUnits(uint64_t a, uint64_t b)
{
    uint64_t mask = ~(a | ~b);
    return mask;
}

bool HasNewZeros(uint64_t a, uint64_t b)
{
    uint64_t mask = ~(~a | b);
    return mask;
}

bool IsCombinationSuperset(uint64_t combination, uint64_t possibleSuperset)
{
    bool hasNoNewUnits = !HasNewUnits(combination, possibleSuperset);
    /*qInfo(QString("New units: %0 <> %1: No new units: %2")
          .arg(combination, 0, 2)
          .arg(possibleSuperset, 0, 2)
          .arg(hasNoNewUnits)
          .toStdString().c_str());*/
    bool hasNewZeros = HasNewZeros(combination, possibleSuperset);
    return hasNoNewUnits && hasNewZeros;
}

void ProcessOperand(QVector<QVector<int> *> *conjunctionList, int nodeIndex, QVector<int> *nodeIndexSet, QVector<QVector<int>*> *appendixCombinationList)
{
    QVector<int> *operand = conjunctionList->at(nodeIndex);
    QVector<int> presentNodes = QVector<int>();
    QVector<int> *missingNodes = new QVector<int>();
    GetSetPresentMissing(operand, &presentNodes, missingNodes, nodeIndexSet);
    if(missingNodes->length() == 0)
    {
        delete operand;
        delete missingNodes;
        return;
    }
    AppendInvertedNodes(operand, missingNodes);

    uint64_t combinations = (1 << missingNodes->length()) - 1;
    for(uint64_t combination = 1; combination < combinations; combination++)
    {
        QVector<int> *appendixOperand = new QVector<int>();
        QVector<int> *combinationVector = new QVector<int>();

        BuildCombination(appendixOperand, combinationVector, missingNodes, combination);

        QVector<int> *superset = nullptr;
        superset = FindSuperset(appendixCombinationList, combinationVector);

        if(superset != nullptr)
        {
            //uint64_t oldCombination = combination;
            combination = SkipSubsetCombinations(combination) - 1;

            delete appendixOperand;
            appendixOperand = nullptr;
            delete combinationVector;
            combinationVector = nullptr;
        }
        else
        {
            SetUtils::CopyListContents(&presentNodes, appendixOperand);
            conjunctionList->append(appendixOperand);

            appendixOperand = nullptr;
            delete combinationVector;
            combinationVector = nullptr;

            /*

            uint64_t lastSubsetCombination = SkipSubsetCombinations(combination) - 1;
            QVector<uint64_t> notFoundCombinations = QVector<uint64_t>();
            notFoundCombinations.append(combination);

            for(uint64_t subsetCombination = lastSubsetCombination; subsetCombination > combination; subsetCombination--)
            {
                int processedSubsetIndex = -1;
                for(int vectorIndex = 0; vectorIndex < notFoundCombinations.length(); vectorIndex++)
                {
                    uint64_t storedCombination = notFoundCombinations[vectorIndex];
                    if(IsCombinationSuperset(storedCombination, subsetCombination))
                    {
                        processedSubsetIndex = vectorIndex;
                        break;
                    }
                }
                if(processedSubsetIndex != -1)
                {
                    // This combination is surely not present in appendixCombinationList
                    appendixOperand = new QVector<int>();
                    combinationVector = new QVector<int>();

                    BuildCombination(appendixOperand, combinationVector, missingNodes, subsetCombination);

                    QVector<int> *supersetLocal = nullptr;
                    supersetLocal = FindSuperset(appendixCombinationList, combinationVector);
                    if(supersetLocal == nullptr)
                    {
                        SetUtils::CopyListContents(&presentNodes, appendixOperand);
                        conjunctionList->append(appendixOperand);
                    }
                    else
                    {
                        QVector<int> tempCombination = QVector<int>();
                        QVector<int> tempSubset = QVector<int>();
                        BuildCombination(nullptr, &tempCombination, missingNodes, subsetCombination);
                        BuildCombination(nullptr, &tempSubset, missingNodes, notFoundCombinations[processedSubsetIndex]);

                        char combinationBuffer[256];
                        char subsetBuffer[256];
                        PrintSet(&tempCombination, combinationBuffer, 256);
                        PrintSet(&tempSubset, subsetBuffer, 256);
                        qInfo(QString("Subset for %0 was processed (%1), but FindSuperset returned non-null")
                              .arg(combinationBuffer)
                              .arg(subsetBuffer)
                              .toStdString().c_str());
                    }

                    delete combinationVector;
                    combinationVector = nullptr;
                    appendixOperand = nullptr;
                }
                else
                {
                    notFoundCombinations.append(subsetCombination);

                    appendixOperand = new QVector<int>();
                    combinationVector = new QVector<int>();

                    BuildCombination(appendixOperand, combinationVector, missingNodes, subsetCombination);

                    QVector<int> *supersetLocal = nullptr;
                    supersetLocal = FindSuperset(appendixCombinationList, combinationVector);
                    if(supersetLocal == nullptr)
                    {
                        SetUtils::CopyListContents(&presentNodes, appendixOperand);
                        conjunctionList->append(appendixOperand);
                    }

                    delete combinationVector;
                    combinationVector = nullptr;
                    appendixOperand = nullptr;
                }
            }

            combination = lastSubsetCombination;
*/

/*
            char subsetBuffer[256];

            PrintSet(combinationVector, subsetBuffer, 256);
            qInfo(QString("END on %0. Superset for %1 was not found. Last subset: %2")
                  .arg(combination, missingNodes->length(), 2)
                  .arg(subsetBuffer)
                  .arg(lastSubsetCombination, 0, 2)
                  .toStdString().c_str());
*/

        }
    }

    appendixCombinationList->append(missingNodes);

    //qInfo("NEXT OPERAND");
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

    QVector<QVector<int>*> appendixCombinationList = QVector<QVector<int>*>();

    for(int i = 0; i < operandsCount; i++)
    {
        ProcessOperand(conjunctionList, i, nodeIndexSet, &appendixCombinationList);
    }

    if(listClone.length() > 1)
    {
        conjunctionList->append(nodeIndexSet);
    }

    for(int i = 0; i < appendixCombinationList.length(); i++)
    {
        delete appendixCombinationList[i];
    }

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


