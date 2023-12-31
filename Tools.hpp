#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include "Time.hpp"
#include "tinyxml2.hpp"

/*Klasa zawierająca potrzebne narzędzia
oraz przechowujaca informacje o ostatnio wygenerowanych danych i rozwiazaniu.*/

class Tools
{
public:

	std::vector<std::vector<int>> matrix;
	int numberOfCities;
	//std::vector<int> minPathEnd;
	std::vector<int> minPath;
	int minCost;
	//int minCostEnd;
	long bestTimeStamp;
	long time;

	Tools();
	~Tools();

	void readFromTXT(std::string filename);
	void readFromXML(const char* filename);
	void saveToFile(std::string lastFilename);
	void readFromFile(std::string filename);

	void random();

	void print();
	void printSolution();

};