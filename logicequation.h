#ifndef LOGICEQUATION_H
#define LOGICEQUATION_H

#include "logicequationnode.h"
#include "netparams.h"
#include "path.h"



class LogicEquation
{
    QVector<LogicEquationNode*> *equationNodes;
    LogicEquationNode *coreNode;

    void Construct(EventProbabilityProvider *provider, QVector<Path<NetParams> *> *pathList);
    LogicEquationNode* ConstructMultiplicative(EventProbabilityProvider *provider, Path<NetParams> *path);

    bool ResolveRecursively(LogicEquationNode* node, bool *input, int length);

public:
    LogicEquation(EventProbabilityProvider *provider, QVector<Path<NetParams>*> *pathList);

    bool Resolve(bool *input, int length);

    ~LogicEquation();
};

#endif // LOGICEQUATION_H
