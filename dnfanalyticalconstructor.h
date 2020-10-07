#ifndef DNFANALYTICALCONSTRUCTOR_H
#define DNFANALYTICALCONSTRUCTOR_H

#include "logicequation.h"
#include "path.h"


class DnfAnalyticalConstructor
{
    DnfAnalyticalConstructor();
public:
    static LogicEquation *GetPdnf(QVector<Path<NetParams>*> *pathList);
};

#endif // DNFANALYTICALCONSTRUCTOR_H
