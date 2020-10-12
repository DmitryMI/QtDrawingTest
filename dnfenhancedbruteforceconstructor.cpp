#include "dnfenhancedbruteforceconstructor.h"
#include "setutils.h"
#include "setutils.h"

DnfEnhancedBruteforceConstructor::DnfEnhancedBruteforceConstructor()
{

}






void DnfEnhancedBruteforceConstructor::GetPdnfConjunction(QVector<Path<NetParams> *> *pathList, QVector<QVector<int> *> *resultVectorMatrix, int threads_count)
{
    QVector<Path<NetParams>*> listClone = QVector<Path<NetParams>*>();
    SetUtils::CopyListContents(pathList, &listClone);
    SetUtils::RemoveComplexPaths(&listClone);

    QVector<int> *nodeIndexSet = new QVector<int>();
    GetNodeSet(listClone, nodeIndexSet);

    uint64_t *pathMasks = new uint64_t[listClone.length()];
    GetPathMasks(&listClone, pathMasks);

    QVector<uint64_t> conjunctionList = QVector<uint64_t>();
    QVector<uint64_t> subsetList = QVector<uint64_t>();

    for(int i = 0; i < listClone.length(); i++)
    {
        uint64_t pathMask = pathMasks[i];
        QVector<int> presentNodes = QVector<int>();
        QVector<int> missingNodes = QVector<int>();
        QVector<int> *conjunctionOperand = new QVector<int>();
        GetPresentMissingNodes(pathMask, nodeIndexSet, &presentNodes, &missingNodes);
        SetUtils::CopyListContents(&presentNodes, conjunctionOperand);
        AppendInvertedNodes(&missingNodes, conjunctionOperand);
        resultVectorMatrix->append(conjunctionOperand);
        conjunctionOperand = nullptr;

        uint64_t combinationsCount = (1 << missingNodes.length()) - 1;

        for(uint64_t combination = 1; combination < combinationsCount; combination++)
        {
            uint64_t combinationMask = GetMaskFromMissingNodesCombination(pathMask, &missingNodes, combination);
            int subsetIndex = FindSubsetIndex(&subsetList, combinationMask);
            if(subsetIndex != -1)
            {
                combination = SkipSubsetCombinations(combination) - 1;
            }
            else
            {
                conjunctionOperand = new QVector<int>();
                AppendNodesByMask(nodeIndexSet, conjunctionOperand, combinationMask);
                resultVectorMatrix->append(conjunctionOperand);
                conjunctionOperand = nullptr;
            }
        }

        subsetList.append(pathMask);
    }

    if(listClone.length() > 1)
    {
        resultVectorMatrix->append(nodeIndexSet);
    }
}

void DnfEnhancedBruteforceConstructor::GetPathMask(Path<NetParams> *path, uint64_t *pathMask)
{
    *pathMask = 0;
    for(int i = 0; i < path->length(); i++)
    {
        auto node = path->at(i);
        *pathMask |= (1 << node->GetKey());
    }
}

void DnfEnhancedBruteforceConstructor::GetPathMasks(QVector<Path<NetParams> *> *pathList, uint64_t *pathMasks)
{
    for(int i = 0; i < pathList->length(); i++)
    {
        Path<NetParams> *path = pathList->at(i);
        uint64_t *mask = pathMasks + i;
        GetPathMask(path, mask);
    }
}

void DnfEnhancedBruteforceConstructor::GetPresentMissingNodes(uint64_t mask,  QVector<int> *nodeIndexList, QVector<int> *presentNodes, QVector<int> *missingNodes)
{
    for(int i = 0; i < nodeIndexList->length(); i++)
    {
        int nodeIndex = nodeIndexList->at(i);
        if(mask & (1 << nodeIndex))
        {
            presentNodes->append(nodeIndex);
        }
        else if(missingNodes != nullptr)
        {
            missingNodes->append(nodeIndex);
        }
    }
}

bool DnfEnhancedBruteforceConstructor::HasNewZeros(uint64_t a, uint64_t b)
{
    uint64_t mask = ~(~a | b);
    return mask;
}

bool DnfEnhancedBruteforceConstructor::IsSuperset(uint64_t mask, uint64_t possibleSuperset)
{
    return !HasNewZeros(mask, possibleSuperset);
}

void DnfEnhancedBruteforceConstructor::AppendInvertedNodes(QVector<int> *source, QVector<int> *target)
{
    for(int i = 0; i < source->length(); i++)
    {
        int nodeKey = source->at(i);
        target->append(-nodeKey);
    }
}

void DnfEnhancedBruteforceConstructor::AppendNodesByMask(QVector<int> *source, QVector<int> *target, uint64_t mask)
{
    for(int i = 0; i < source->length(); i++)
    {
        int nodeKey = source->at(i);
        if(mask & (1 << nodeKey))
        {
            target->append(nodeKey);
        }
        else
        {
            target->append(-nodeKey);
        }
    }
}

uint64_t DnfEnhancedBruteforceConstructor::GetMaskFromMissingNodesCombination(uint64_t baseMask, QVector<int> *missingNodes, uint64_t combination)
{
    //int bit0 = baseMask & (1 << 0);
    //int bit1 = baseMask & (1 << 1);
    for(int i = 0; i < missingNodes->length(); i++)
    {
        if(combination & 1 << i)
        {
            baseMask |= (1 << missingNodes->at(i));
        }
    }

    return baseMask;
}

int DnfEnhancedBruteforceConstructor::FindSubsetIndex(QVector<uint64_t> *subsetList, uint64_t mask)
{
    for(int i = 0; i < subsetList->length(); i++)
    {
        uint64_t possibleSubset = subsetList->at(i);
        bool isSuperset = IsSuperset(possibleSubset, mask);
        if(isSuperset)
        {
            return i;
        }
    }

    return -1;
}

int DnfEnhancedBruteforceConstructor::SkipSubsetCombinations(uint64_t combination)
{
    int mask = 1;
    while(!(combination & mask))
    {
        mask <<= 1;
    }
    return combination + mask;
}

void DnfEnhancedBruteforceConstructor::GetNodeSet(QVector<Path<NetParams> *> pathList, QVector<int> *nodeIndexList)
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

LinearLogicEquation *DnfEnhancedBruteforceConstructor::GetPdnfLinear(QVector<Path<NetParams> *> *pathList, int thread_count)
{
    QVector<QVector<int> *> *conjunctionList = new QVector<QVector<int>*>();
    GetPdnfConjunction(pathList, conjunctionList, thread_count);

    LinearLogicEquation *equation = new LinearLogicEquation(conjunctionList);

    SetUtils::FreeVectorMatrix(conjunctionList);

    return equation;
}
