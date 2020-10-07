#ifndef DNFANALYTICALCONSTRUCTOR_H
#define DNFANALYTICALCONSTRUCTOR_H

#include "linearlogicequation.h"
#include "logicequation.h"
#include "path.h"


class DnfAnalyticalConstructor
{
    DnfAnalyticalConstructor();

    static void GetPdnfConjunction(QVector<Path<NetParams>*> *pathList, QVector<QVector<int>*> *resultVectorMatrix);
public:
    static LogicEquation *GetPdnfTree(QVector<Path<NetParams>*> *pathList);
    static LinearLogicEquation *GetPdnfLinear(QVector<Path<NetParams>*> *pathList);

};

#endif // DNFANALYTICALCONSTRUCTOR_H
