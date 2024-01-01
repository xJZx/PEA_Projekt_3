#include "Tools.hpp"

// struktura reprezentuj¹ca pojedyñcz¹ populacjê (œcie¿kê wraz z jej kosztem)
struct population {
	int populationCost;
	std::vector<int> path;

	bool comparator(population& first, population& second);
};

class GeneticAlgorithm
{
public:
	std::vector<std::vector<int>> matrix;
	population bestPopulation;
	int numberOfCities;
	long bestTimeStamp;
	long executionTime;

	GeneticAlgorithm(Tools lastSolution);
	~GeneticAlgorithm();

	void geneticAlgorithm(double stopTime, int startPopulationAmount, double crossoverFactor, double mutationFactor, int mutationMethod);
	int findCost(std::vector<int>& path);
	std::vector<population> findRandomStartPopulation(int startPopulationAmount);
	std::vector<population> stochasticUniversalSampling(const std::vector <population>& populations, int numberOfParents);
	double realNumberDraw(double rangeLow, double rangeHigh);
	int intNumberDraw(int rangeLow, int rangeHigh);
	population orderCrossover(const population& firstParent, const population& secondParent);
	void swapMutation(population& child);
};