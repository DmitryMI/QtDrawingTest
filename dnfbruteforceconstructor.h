#ifndef DNFBRUTEFORCECONSTRUCTOR_H
#define DNFBRUTEFORCECONSTRUCTOR_H

#include "logicequation.h"
#include "netparams.h"
#include "path.h"



class DnfBruteforceConstructor
{
    DnfBruteforceConstructor();
public:

    static LogicEquation *GetPdnf(QVector<Path<NetParams>*> *pathList);
private:
    static LogicEquation *GetPerfectDisjunctiveNormalForm(LogicEquation *sourceEquation);
    static LogicEquationNode *ConstructMultiplicative(LogicEquation *equation, Path<NetParams> *path);
    static void Construct(LogicEquation *equation, QVector<Path<NetParams> *> *pathList);
};

#endif // DNFBRUTEFORCECONSTRUCTOR_H
