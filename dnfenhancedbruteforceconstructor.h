#ifndef DNFENHANCEDBRUTEFORCECONSTRUCTOR_H
#define DNFENHANCEDBRUTEFORCECONSTRUCTOR_H

#include "linearlogicequation.h"
#include "logicequation.h"



class DnfEnhancedBruteforceConstructor
{
    DnfEnhancedBruteforceConstructor();

    static void GetPdnfConjunction(QVector<Path<NetParams> *> *pathList, QVector<QVector<int> *> *resultVectorMatrix, int threads_count);
    static void GetPathMask(Path<NetParams> *path, uint64_t *pathMask);

    static void GetPathMasks(QVector<Path<NetParams> *> *pathList, uint64_t *pathMasks);

    static void GetPresentMissingNodes(uint64_t mask,  QVector<int> *nodeIndexList, QVector<int> *presentNodes, QVector<int> *missingNodes);

    static bool HasNewZeros(uint64_t a, uint64_t b);

    static bool IsSuperset(uint64_t mask, uint64_t possibleSuperset);

    static void AppendInvertedNodes(QVector<int> *source, QVector<int> *target);

    static void AppendNodesByMask(QVector<int> *source, QVector<int> *target, uint64_t mask);

    static uint64_t GetMaskFromMissingNodesCombination(uint64_t baseMask, QVector<int> *missingNodes, uint64_t combination);

    static int FindSubsetIndex(QVector<uint64_t> *subsetList, uint64_t mask);

    static int SkipSubsetCombinations(uint64_t combination);

    static void GetNodeSet(QVector<Path<NetParams>*> pathList, QVector<int> *nodeIndexList);

    static void GetSimplePathMasks(QVector<Path<NetParams>*> *pathList, QVector<int> *pathMasks);
public:
    static LinearLogicEquation *GetPdnfLinear(QVector<Path<NetParams>*> *pathList, int thread_count = 1);
};

#endif // DNFENHANCEDBRUTEFORCECONSTRUCTOR_H
