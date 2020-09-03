#ifndef PROBABILITYPROVIDER_H
#define PROBABILITYPROVIDER_H


class ProbabilityProvider
{
public:
	virtual bool GetCurrentProbability(double* probability) = 0;
};

#endif // PROBABILITYPROVIDER_H
