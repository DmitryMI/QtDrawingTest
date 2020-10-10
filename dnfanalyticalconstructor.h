#ifndef DNFANALYTICALCONSTRUCTOR_H
#define DNFANALYTICALCONSTRUCTOR_H

#include "linearlogicequation.h"
#include "logicequation.h"
#include "path.h"

class DnfAnalyticalConstructor
{
    DnfAnalyticalConstructor();

    static void GetPdnfConjunction(QVector<Path<NetParams>*> *pathList, QVector<QVector<int>*> *resultVectorMatrix);
    static void GetPdnfConjunction(QVector<Path<NetParams>*> *pathList, QVector<QVector<int>*> *resultVectorMatrix, int threads_count);
public:
    static LogicEquation *GetPdnfTree(QVector<Path<NetParams>*> *pathList);
    static LinearLogicEquation *GetPdnfLinear(QVector<Path<NetParams>*> *pathList, int thread_count = 1);

};

#endif // DNFANALYTICALCONSTRUCTOR_H
