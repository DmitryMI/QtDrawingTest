#ifndef EVENTPROBABILITYPROVIDER_H
#define EVENTPROBABILITYPROVIDER_H


class EventProbabilityProvider
{
public:
    virtual double GetEventProbability(int eventIndex) = 0;
};

#endif // EVENTPROBABILITYPROVIDER_H
