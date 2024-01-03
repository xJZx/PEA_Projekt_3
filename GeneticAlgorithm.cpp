#include "GeneticAlgorithm.hpp"
#include <random>

GeneticAlgorithm::GeneticAlgorithm(Tools lastSolution) {
	numberOfCities = lastSolution.numberOfCities;
	// ustalenie domyœlnej pocz¹tkowej populacji
	population bestPopulation;
	matrix = lastSolution.matrix;
	bestTimeStamp = 0;
	executionTime = 0;
}

GeneticAlgorithm::~GeneticAlgorithm() {

}

// porownywarka dwóch populacji, która daje krótsz¹ drogê
bool comparator(population& first, population& second) {
	if (first.populationCost < second.populationCost) {
		return true;
	}
	else {
		return false;
	}
}

void GeneticAlgorithm::geneticAlgorithm(double stopTime, int startPopulationAmount, double crossoverFactor, double mutationFactor, int mutationMethod) {
	// nadpisanie najlepszej populacji
	bestPopulation.populationCost = INT_MAX;

	// zaludnienie pocz¹tkowych populacji
	std::vector<population> populations = findRandomStartPopulation(startPopulationAmount);

	// kiedy poszczególne rozwi¹zania zosta³y znalezione
	std::vector<int> costs;
	std::vector<long> times;

	// start mierzenia czasu
	Time time;
	time.start();

	while (time.totalTime() < stopTime) {
		// utworzenie nowej populacji, w celu selekcji
		std::vector<population> evolvedPopulation;

		// posortowanie populacji w celu wyznaczenia najlepszych do "rozp³odu"
		std::sort(populations.begin(), populations.end(), comparator);

		// czynnik o ile mniej bierzemy z g³ównej ju¿ posortowanej populacji ca³kowitej
		int shorteningFactor = 10;
		for (int i = 0; i < populations.size() / shorteningFactor; i++) {
			evolvedPopulation.push_back(populations[i]);
		}

		// utworzenie nowej populacji poprzez "rozmna¿anie" dzieci obecnej wyselekcjonowanej ju¿ populacji
		while (evolvedPopulation.size() < startPopulationAmount) {
			// tworzymy teraz rodziców przysz³ych dzieci i potem ich przypisujemy
			// stosujemy metodê SUS stochastyczne uniwersalne próbkowanie
			std::vector<population> selectedParents = stochasticUniversalSampling(populations, 2);
			population firstParent = selectedParents[0];
			population secondParent = selectedParents[1];
			selectedParents.clear();
			
			// stworzenie dzieci
			population firstChild = firstParent;
			population secondChild = secondParent;

			// wylosowanie wartoœci dla crossoveru
			double randForCrossover = realNumberDraw(0.0, 1.0);

			// warunek wykonania crossoveru
			if (randForCrossover < crossoverFactor) {
				firstChild = orderCrossover(firstParent, secondParent);
				secondChild = orderCrossover(firstParent, secondParent);
			}

			double firstRandForMutation = realNumberDraw(0.0, 1.0);

			// pierwsza mutacja
			if (firstRandForMutation < mutationFactor) {
				if (mutationMethod == 1) {
					swapMutation(firstChild);
				}
				else if (mutationMethod == 2) {
					scrambleMutation(firstChild);
				}
			}

			double secondRandForMutation = realNumberDraw(0.0, 1.0);

			// druga mutacja
			if (secondRandForMutation < mutationFactor) {
				if (mutationMethod == 1) {
					swapMutation(secondChild);
				}
				else if (mutationMethod == 2) {
					scrambleMutation(secondChild);
				}
			}

			firstChild.populationCost = findCost(firstChild.path);
			secondChild.populationCost = findCost(secondChild.path);

			evolvedPopulation.push_back(firstChild);
			evolvedPopulation.push_back(secondChild);

			if (comparator(firstChild, bestPopulation)) {
				bestPopulation = firstChild;
			}

			if (comparator(secondChild, bestPopulation)) {
				bestPopulation = secondChild;
			}

			time.check();
			executionTime = time.totalTime();
		}

		populations = evolvedPopulation;

		time.check();
		executionTime = time.totalTime();
	}

	std::sort(populations.begin(), populations.end(), comparator);
}

// znalezienie pocz¹tkowej populacji
std::vector<population> GeneticAlgorithm::findRandomStartPopulation(int startPopulationAmount) {
	// ca³oœæ populacji, która zostanie zwrócona i zapisana w domyœlnej zmiennej
	std::vector<population> populations;

	for (int i = 0; i < startPopulationAmount; i++) {
		// pojedyncza populacja, która zape³ni ca³oœæ (zmienna pomocnicza)
		population population;
		
		for (int j = 0; j < matrix.size(); j++) {
			population.path.push_back(j);
		}

		// losowo mieszamy wierzcho³ki w populacji
		std::random_shuffle(population.path.begin(), population.path.end());
		population.populationCost = findCost(population.path);
		populations.push_back(population);

		// szukamy najlepszej z losowo pomieszanych populacji
		if (comparator(population, bestPopulation)) {
			bestPopulation = population;
		}
	}

	return populations;
}

// znalezieniu kosztu dla danej listy kandydatów
int GeneticAlgorithm::findCost(std::vector<int>& path) {
	int cost = 0;
	int iterator = 0;

	for (int i = 1; i < path.size(); i++) {
		cost += matrix[path[iterator]][path[i]];
		iterator++;
	}

	cost += matrix[path[iterator]][path[0]];

	return cost;
}

std::vector<population> GeneticAlgorithm::stochasticUniversalSampling(const std::vector<population>& populations, int numberOfParents) {
	// obliczanie ca³kowitej sprawnoœci i œredniej sprawnoœci (d³ugoœci œcie¿ki)
	double totalFitness = 0.0;
	for (const auto& population : populations) {
		totalFitness += population.populationCost;
	}

	double averageFitness = totalFitness / populations.size();

	// wygenerowanie wskaŸników do ko³a (to ko³o bêdzie mia³o 2 wskaŸniki)
	std::vector<double> pointers;
	for (int i = 0; i < numberOfParents; i++) {
		// generowanie losowego miejsca pocz¹tkowego
		double randStart = realNumberDraw(0.0, averageFitness);
		pointers.push_back(randStart + i * averageFitness);
	}

	// wybór rodziców
	std::vector<population> selectedParents;
	double currentSum = populations[0].populationCost;
	int parentIndex = 0;

	for (double pointer : pointers) {
		// suma musi byæ mniejsza od wskaŸnika, aby by³ on optymalny
		while (currentSum < pointer) {
			++parentIndex;
			currentSum += populations[parentIndex].populationCost;
		}

		selectedParents.push_back(populations[parentIndex]);
	}

	return selectedParents;
}

// order crossover
population GeneticAlgorithm::orderCrossover(const population& firstParent, const population& secondParent) {
	population newChild;
	//newChild.path.resize(numberOfCities);
	newChild.path.clear();

	// wygenerowanie przedzia³u, z którego bêdzie kopiowany kod gen.
	int startPosition = intNumberDraw(0, numberOfCities - 1);
	int endPosition = intNumberDraw(0, numberOfCities - 1);

	if (startPosition > endPosition) {
		std::swap(startPosition, endPosition);
	}
	
	// skopiowanie materia³u gen. miêdzy konkretnymi pozycjami
	for (int i = startPosition; i <= endPosition; i++) {
		newChild.path.push_back(firstParent.path.at(i));
	}

	// zape³nienie pozosta³ych miejsc
	int index = (endPosition + 1) % numberOfCities;
	int secondParentIndex = index;

	// do naprawy
	while (newChild.path.size() < secondParent.path.size()){
		// std::find jeœli nie znajdzie miasta w nowej œcie¿ce, to zwraca iterator na ostatni element tablicy wektorowej, czyli newChild.path.end()
		// tak szukamy czy element jeszcze nie istnieje w tablicy wektorowej
		if (std::find(newChild.path.begin(), newChild.path.end(), secondParent.path[secondParentIndex]) == newChild.path.end()) {
			newChild.path.push_back(secondParent.path.at(secondParentIndex));
		}
		index = (index + 1) % numberOfCities;
		secondParentIndex = (secondParentIndex + 1) % numberOfCities;
	}

	return newChild;
}

// mutacja typu swap
void GeneticAlgorithm::swapMutation(population& child) {
	int firstRandPosition = intNumberDraw(0, numberOfCities - 1);
	int secondRandPosition = intNumberDraw(0, numberOfCities - 1);

	std::swap(child.path[firstRandPosition], child.path[secondRandPosition]);
}

// mutacja typu scramble
void GeneticAlgorithm::scrambleMutation(population& child) {
	int firstRandPosition = intNumberDraw(0, numberOfCities - 1);
	int secondRandPosition = intNumberDraw(0, numberOfCities - 1);

	if (firstRandPosition > secondRandPosition) {
		std::swap(firstRandPosition, secondRandPosition);
	}

	std::random_shuffle(child.path.begin() + firstRandPosition, child.path.begin() + secondRandPosition + 1);
}


// generowanie losowej liczby
double GeneticAlgorithm::realNumberDraw(double rangeLow, double rangeHigh) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(rangeLow, rangeHigh);
	double randStart = dis(gen);

	return randStart;
}

// generowanie losowej liczby
int GeneticAlgorithm::intNumberDraw(int rangeLow, int rangeHigh) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(rangeLow, rangeHigh);
	double randStart = dis(gen);

	return randStart;
}