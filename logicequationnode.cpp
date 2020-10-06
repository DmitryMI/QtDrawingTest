#include "logicequationnode.h"

LogicEquationNode::LogicEquationNode(LogicOperation op, LogicEquationNode *a, LogicEquationNode *b)
{
    operation = op;
    firstOperand = a;
    secondOperand = b;
}

LogicEquationNode::LogicEquationNode(LogicOperation op, LogicEquationNode *a)
{
    operation = op;
    firstOperand = a;
    secondOperand = nullptr;
}

LogicEquationNode::LogicEquationNode(int eventIndex)
{
    this->eventIndex = eventIndex;
}

int LogicEquationNode::getEventIndex()
{
    return eventIndex;
}

LogicOperation LogicEquationNode::getLogicOperation()
{
    return operation;
}
