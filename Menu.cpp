#include "Menu.hpp"
#include <iostream>


void Menu::start()
{
    std::string lastFilename;
    std::string results;
    double stopTime = 60 * 1000000;
    int startPopulationAmount = 1000;
    int mutationMethod = 2;
    // factors
    double crossoverFactor = 0.8;
    double mutationalFactor = 0.1;

    while (true) {
        int creation = 0;
        std::cout << "Select options:\n";
        std::cout << "Any key - EXIT\n";
        std::cout << "1. Read from the file\n";
        std::cout << "2. Stop critera\n";
        std::cout << "3. Check results\n";
        std::cout << "4. Criteria\n";
        std::cout << "5. Mutation method\n";
        std::cout << "6. Genetic Algorithm - Start\n";
        std::cout << "7. Print Matrix\n";
        std::cin >> creation;

        switch (creation) {
        case 1:
        {
            std::string filename;
            std::cout << "Insert the filename: ";
            std::cin >> filename;
            lastFilename = filename;
            const char* filenameXML = filename.c_str();               
            lastSolution.readFromXML(filenameXML);
            std::cout << std::endl;
        }
        break;

        case 2:
        {
            std::cout << "Insert the amount of time to stop in seconds: ";
            std::cin >> stopTime;
            // bo mikrosekundy
            stopTime = stopTime * 1000000;
        }
        break;

        case 3:
        {
            std::cout << "Insert the results filename: ";
            std::cin >> results;
            lastSolution.readFromFile(results);
        }
        break;

        case 4:
        {
            std::cout << "Insert the amount of starting population: ";
            std::cin >> startPopulationAmount;
            std::cout << "Insert the mutational factor: ";
            std::cin >> crossoverFactor;
            std::cout << "Insert the crossing factor: ";
            std::cin >> mutationalFactor;
        }
        break;

        case 5:
        {
            std::cout << "Choose mutation method:\n";
            std::cout << "1. Swap Mutation\n";
            std::cout << "2. Scramble Mutation\n";
            std::cin >> mutationMethod;
            std::cout << std::endl;
        }
        break;

        case 6:
        {
            GeneticAlgorithm geneticAlgorithm(lastSolution);
            geneticAlgorithm.geneticAlgorithm(stopTime, startPopulationAmount, crossoverFactor, mutationalFactor, mutationMethod);
            lastSolution.minCost = geneticAlgorithm.bestPopulation.populationCost;
            lastSolution.minPath = geneticAlgorithm.bestPopulation.path;
            lastSolution.bestTimeStamp = geneticAlgorithm.bestTimeStamp;
            lastSolution.time = geneticAlgorithm.executionTime;
            lastSolution.printSolution();
            lastSolution.saveToFile(lastFilename);
        }
        break;

        case 7:
        {
            lastSolution.print();
            std::cout << std::endl;
        }
        break;

        default:
            return;
        }
    }
    return;
}