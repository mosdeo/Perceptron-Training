#include <iostream>
#include <string>
using namespace std;

class Perceptron
{
private:
	double WeightVector[3];
	

public:
	double InnerProductSum;

	Perceptron(double WeightVector0,double WeightVector1, double WeightVector2)
	{
		WeightVector[0] = WeightVector0;
		WeightVector[1] = WeightVector1;
		WeightVector[2] = WeightVector2;
	};

	string GetWeightVector(void)
	{
		string strWeightVector = std::to_string(WeightVector[0]) + "," + std::to_string(WeightVector[1]) + "," + std::to_string(WeightVector[2]);
		return strWeightVector;
	};

	double GetWeightValue(int index)
	{
		return WeightVector[index];
	}

	void SetWeightVector(int WeightVector0, int WeightVector1, int WeightVector2)
	{
		WeightVector[0] = WeightVector0;
		WeightVector[1] = WeightVector1;
		WeightVector[2] = WeightVector2;
	};

	void ErrSubWeightVector(double WeightVector0, double WeightVector1, double WeightVector2)
	{
		WeightVector[0] -= WeightVector0;
		WeightVector[1] -= WeightVector1;
		WeightVector[2] -= WeightVector2;
	};

	void ErrAddWeightVector(int WeightVector0, int WeightVector1, int WeightVector2)
	{
		WeightVector[0] += WeightVector0;
		WeightVector[1] += WeightVector1;
		WeightVector[2] += WeightVector2;
	};

	double SumOfAction(double a0, double a1, double a2=1)
	{
		//double sigmoid;
		//sigmoid = (1+exp(-(a0*WeightVector[0] + a1*WeightVector[1] + a2*WeightVector[2])));
		//sigmoid = 1/sigmoid;
		//return sigmoid;

		return tanh(a0*WeightVector[0] + a1*WeightVector[1] + a2*WeightVector[2]);
	}

};