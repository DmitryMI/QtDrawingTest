#ifndef NETPARAMS_H
#define NETPARAMS_H


class NetParams
{
public:
	NetParams(double probability)
	{
		this->probability = probability;
	}

	NetParams()
	{
		this->probability = 1;
	}

	double GetProbability()
	{
		return probability;
	}

	void SetProbability(double probability)
	{
		this->probability = probability;
	}

private:
	double probability;
};

#endif // NETPARAMS_H
