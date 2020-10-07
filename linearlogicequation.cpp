#include "linearlogicequation.h"


LinearLogicEquation::LinearLogicEquation(QVector<QVector<int>*> *conjunctionList)
{
    this->conjunctionList = new QVector<QVector<int>*>();
    for(int i = 0; i < conjunctionList->length(); i++)
    {
        QVector<int> *disjunction = conjunctionList->at(i);
        QVector<int> *disjunctionClone = new QVector<int>();

        for(int j = 0; j < disjunction->length(); j++)
        {
            disjunctionClone->append(disjunction->at(j));
        }
        this->conjunctionList->append(disjunctionClone);
    }
}

double LinearLogicEquation::GetProbability(EventProbabilityProvider *provider)
{
    double probabilitySumm = 0;
    for(int i = 0; i < conjunctionList->length(); i++)
    {
        double probabilityMult = 1;
        QVector<int> *disjunction = conjunctionList->at(i);
        for(int j = 0; j < disjunction->length(); j++)
        {
            int nodeKey = disjunction->at(j);
            double mult;
            if(nodeKey < 0)
            {
                mult = 1 - provider->GetEventProbability(-nodeKey);
            }
            else
            {
                mult = provider->GetEventProbability(nodeKey);
            }

            probabilityMult *= mult;
        }
        probabilitySumm += probabilityMult;
    }

    return probabilitySumm;
}
