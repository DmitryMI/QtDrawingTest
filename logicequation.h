#ifndef LOGICEQUATION_H
#define LOGICEQUATION_H

#include "logicequationnode.h"
#include "netparams.h"
#include "path.h"



class LogicEquation
{
    QVector<LogicEquationNode*> *equationNodes;
    QVector<LogicEquationNode*> *primitivesList;
    LogicEquationNode *coreNode;
    bool ResolveRecursively(LogicEquationNode* node, bool *input, int length);
    double ResolveProbabilityRecursively(EventProbabilityProvider *provider, LogicEquationNode *node);
    double ResolveProbability(EventProbabilityProvider *provider);

    double ResolveProbabilityIteratively(EventProbabilityProvider *provider);

public:
    LogicEquation();

    bool Resolve(bool *input, int length);

    ~LogicEquation();

    int getEventsCount();

    int getEquationNodesCount();

    LogicEquationNode* FindPrimitive(int eventIndex);

    double GetProbability(EventProbabilityProvider *provider);

    LogicEquationNode* AddUniquePrimitive(int eventIndex);

    void AddEquationNode(LogicEquationNode *node);

    void SetRootNode(LogicEquationNode *node);

};

#endif // LOGICEQUATION_H
