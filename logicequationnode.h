#ifndef LOGICEQUATIONNODE_H
#define LOGICEQUATIONNODE_H

#include "eventprobabilityprovider.h"
#include "logicoperation.h"


class LogicEquationNode
{
    LogicOperation operation = LogicOperation::None;
    int eventIndex = 0;
    LogicEquationNode *firstOperand = nullptr;
    LogicEquationNode *secondOperand = nullptr;
    EventProbabilityProvider *eventProbabilityProvider = nullptr;

public:

    LogicEquationNode(LogicOperation op, LogicEquationNode *a, LogicEquationNode *b);

    LogicEquationNode(LogicOperation op, LogicEquationNode *a);

    LogicEquationNode(EventProbabilityProvider *provider, int eventIndex);

    int getEventIndex();
};

#endif // LOGICEQUATIONNODE_H
