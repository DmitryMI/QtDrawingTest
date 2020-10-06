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
public:

    LogicEquationNode(LogicOperation op, LogicEquationNode *a, LogicEquationNode *b);

    LogicEquationNode(LogicOperation op, LogicEquationNode *a);

    LogicEquationNode(int eventIndex);

    int getEventIndex();

    LogicOperation getLogicOperation();

    LogicEquationNode *getFirstOperand()
    {
        return firstOperand;
    }

    LogicEquationNode *getSecondOperand()
    {
        return secondOperand;
    }
};

#endif // LOGICEQUATIONNODE_H
