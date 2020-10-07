#include "combinator.h"
#include "dnfanalyticalconstructor.h"

#include <QMessageBox>
#include <QSet>
#include <QStack>

typedef enum SetComparisonResult
{
    Less = -1,
    Equal = 0,
    Greater = 1,
    Uncomparable = 2
} SetComparisonResult;

DnfAnalyticalConstructor::DnfAnalyticalConstructor()
{

}

bool IsSubset(QSet<int> *set1, QSet<int> *set2)
{
    QList<int> values1 = set1->values();
    for(int i = 0; i < values1.count(); i++)
    {
        int value = values1.at(i);
        if(!set2->contains(value))
        {
            return false;
            break;
        }
    }

    return true;
}

SetComparisonResult CompareSets(QSet<int> *set1, QSet<int> *set2)
{
    bool isSubset12 = IsSubset(set1, set2);

    bool isSubset21 = IsSubset(set2, set1);


    if(isSubset12)
    {
        return SetComparisonResult::Less;
    }
    else if(isSubset12 && isSubset21)
    {
        return SetComparisonResult::Equal;
    }
    else if(isSubset21)
    {
        return SetComparisonResult::Greater;
    }
    else
    {
        return SetComparisonResult::Uncomparable;
    }
}

SetComparisonResult CompareSets(QVector<int> *setVector1, QVector<int> *setVector2)
{
    QSet<int> set1 = QSet<int>();
    QSet<int> set2 = QSet<int>();

    for(int i = 0; i < setVector1->length(); i++)
    {
        set1.insert(setVector1->at(i));
    }
    for(int i = 0; i < setVector2->length(); i++)
    {
        set2.insert(setVector2->at(i));
    }

    return CompareSets(&set1, &set2);
}


SetComparisonResult ComparePaths(Path<NetParams> *path1, Path<NetParams> *path2)
{
    QSet<int> set1 = QSet<int>();
    QSet<int> set2 = QSet<int>();

    for(int i = 0; i < path1->length(); i++)
    {
        set1.insert(path1->at(i)->GetKey());
    }
    for(int i = 0; i < path2->length(); i++)
    {
        set2.insert(path2->at(i)->GetKey());
    }

    return CompareSets(&set1, &set2);
}

void FreeVectorMatrix(QVector<QVector<int>*> *vectorMatrix)
{
    for(int i = 0; i < vectorMatrix->length(); i++)
    {
        delete (vectorMatrix->at(i));
    }

    delete vectorMatrix;
}

void RemoveComplexPaths(QVector<Path<NetParams> *> *pathList)
{
    for(int i = 0; i < pathList->count(); i++)
    {
        Path<NetParams> *current = pathList->at(i);
        for(int j = i + 1; j < pathList->count(); j++)
        {
            Path<NetParams> *comparable = pathList->at(j);
            SetComparisonResult comparisonResult = ComparePaths(current, comparable);

            if(comparisonResult == SetComparisonResult::Less)
            {
                pathList->removeAt(j);
                j--;
            }
            else if(comparisonResult == SetComparisonResult::Equal)
            {
                // This must not happen!
                pathList->removeAt(i);
                i--;
                break;
            }
            else if(comparisonResult == SetComparisonResult::Greater)
            {
                pathList->removeAt(i);
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

int SetIndexOf(QVector<int> *set, int value)
{
    for(int i = 0; i < set->length(); i++)
    {
        int setAt = set->at(i);
        if(setAt == value || setAt == -value)
        {
            return i;
        }
    }

    return -1;
}

void GetSetPresentMissing(QVector<int> *set, QVector<int> *presentNodes, QVector<int> *missingNodes, QVector<int> *fullSet)
{
    for(int i = 0; i < fullSet->length(); i++)
    {
        int value = fullSet->at(i);
        int index = SetIndexOf(set, value);
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

template <class T>
void CopyListContents(QVector<T> *source, QVector<T> *clone)
{
    for(int i =0; i < source->length(); i++)
    {
        clone->append(source->at(i));
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
            SetComparisonResult comparisonResult = CompareSets(current, comparable);

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

LogicEquation *DnfAnalyticalConstructor::GetPdnf(QVector<Path<NetParams> *> *pathList)
{
    QVector<Path<NetParams>*> listClone = QVector<Path<NetParams>*>();
    CopyListContents(pathList, &listClone);

    RemoveComplexPaths(&listClone);

    QVector<int> *nodeIndexSet = new QVector<int>();
    GetNodeSet(listClone, nodeIndexSet);

    QVector<QVector<int>*> *conjunctionList = new QVector<QVector<int>*>();
    for(int i = 0; i < listClone.length(); i++)
    {
        Path<NetParams> *path = listClone.at(i);
        QVector<int> *conjunctionOperand = new QVector<int>();
        GetPathNodes(path, nodeIndexSet, conjunctionOperand, nullptr);
        conjunctionList->append(conjunctionOperand);
    }

    int operandsCount = conjunctionList->length();

    for(int i = 0; i < operandsCount; i++)
    {
        QVector<int> *operand = conjunctionList->at(i);
        QVector<int> presentNodes = QVector<int>();
        QVector<int> missingNodes = QVector<int>();
        GetSetPresentMissing(operand, &presentNodes, &missingNodes, nodeIndexSet);
        if(missingNodes.length() == 0)
        {
            continue;
        }
        AppendInvertedNodes(operand, &missingNodes);

        int combinations = (1 << missingNodes.length()) - 1;
        for(int combination = 0; combination < combinations; combination++)
        {
            QVector<int> *appendixOperand = new QVector<int>();
            CopyListContents(&presentNodes, appendixOperand);
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
                }
                appendixOperand->append(nodeIndex);
            }
            conjunctionList->append(appendixOperand);
        }
    }


    conjunctionList->append(nodeIndexSet);

    //QString beforeReductionStr = PrintPdnf(conjunctionList);

    RemoveSupersets(conjunctionList);

    QString afterReduction = PrintPdnf(conjunctionList);

    QMessageBox::information(nullptr, "LogicEquation", afterReduction);

    LogicEquation *equation = BuildEquation(conjunctionList);

    FreeVectorMatrix(conjunctionList);

    return equation;
}
