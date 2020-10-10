#ifndef LINEARLOGICEQUATION_H
#define LINEARLOGICEQUATION_H

#include "eventprobabilityprovider.h"

#include <QVector>



class LinearLogicEquation
{
private:
    QVector<QVector<int>*> *conjunctionList = nullptr;

public:
    LinearLogicEquation(QVector<QVector<int>*> *conjunctionList);
    double GetProbability(EventProbabilityProvider* provider);

    ~LinearLogicEquation();
};

#endif // LINEARLOGICEQUATION_H
