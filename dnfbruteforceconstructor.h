#ifndef DNFBRUTEFORCECONSTRUCTOR_H
#define DNFBRUTEFORCECONSTRUCTOR_H

#include "logicequation.h"
#include "netparams.h"
#include "path.h"
#include <mutex>

#define USE_PARALLEL    1
#define USE_THREADS     32



class DnfBruteforceConstructor
{
    DnfBruteforceConstructor();
public:

    static LogicEquation *GetPdnf(QVector<Path<NetParams>*> *pathList);

private:
    static LogicEquation *GetPerfectDisjunctiveNormalForm(LogicEquation *sourceEquation);
//#if USE_PARALLEL == 1
    static void ProcessVariations(LogicEquation *result, LogicEquation* sourceEquation, LogicEquationNode **outOperand, int maskStart, int maskEnd);
    static LogicEquation *GetPerfectDisjunctiveNormalForm(LogicEquation* sourceEquation, int threads_count);
//#endif
    static LogicEquationNode *ConstructMultiplicative(LogicEquation *equation, Path<NetParams> *path);
    static void Construct(LogicEquation *equation, QVector<Path<NetParams> *> *pathList);
};

#endif // DNFBRUTEFORCECONSTRUCTOR_H
