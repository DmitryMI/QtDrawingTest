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

    void Construct(QVector<Path<NetParams> *> *pathList);
    LogicEquationNode* ConstructMultiplicative(Path<NetParams> *path);

    bool ResolveRecursively(LogicEquationNode* node, bool *input, int length);

    double ResolveProbabilityRecursively(EventProbabilityProvider *provider, LogicEquationNode *node);
    double ResolveProbability(EventProbabilityProvider *provider);

public:
    LogicEquation(QVector<Path<NetParams>*> *pathList);
    LogicEquation();

    bool Resolve(bool *input, int length);

    ~LogicEquation();

    int getEventsCount();

    static LogicEquation *GetPerfectDisjunctiveNormalForm(LogicEquation* sourceEquation);

    LogicEquationNode* FindPrimitive(int eventIndex);

    double GetProbability(EventProbabilityProvider *provider);

    LogicEquationNode* AddUniquePrimitive(int eventIndex);

    void AddEquationNode(LogicEquationNode *node);

    void SetRootNode(LogicEquationNode *node);

};

#endif // LOGICEQUATION_H
