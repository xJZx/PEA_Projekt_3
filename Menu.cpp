#include "Menu.hpp"
#include <iostream>


void Menu::start()
{
    std::string lastFilename;
    std::string results;
    double stopTime = 60 * 1000000;
    // temperature factor
    double a = 0.9999;
    int epoque = 1;

    while (true) {
        int creation = 0;
        std::cout << "Select options:\n";
        std::cout << "Any key - EXIT\n";
        std::cout << "1. Read from the file\n";
        std::cout << "2. Stop critera\n";
        std::cout << "3. Check results\n";
        std::cout << "4. Tabu Search - Start\n";
        std::cout << "5. Simulated Annealing - Criteria\n";
        std::cout << "6. Simulated Annealing - Start\n";
        std::cout << "7. Print Matrix\n";
        std::cin >> creation;

        switch (creation) {
        case 1:
        {
            int choose = 0;
            std::cout << "Select options:\n";
            std::cout << "Any key - BACK\n";
            std::cout << "1. TXT\n";
            std::cout << "2. XML\n";
            std::cin >> choose;

            if (choose == 1) {
                std::string filename;
                std::cout << "Insert the filename: ";
                std::cin >> filename;
                lastFilename = filename;
                lastSolution.readFromTXT(lastFilename);
                std::cout << std::endl;
            }
            else if (choose == 2) {
                std::string filename;
                std::cout << "Insert the filename: ";
                std::cin >> filename;
                lastFilename = filename;
                const char* filenameXML = filename.c_str();
                lastSolution.readFromXML(filenameXML);
                std::cout << std::endl;
            }

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
            /*TabuSearch tabu(lastSolution);
            tabu.tabuSearch(stopTime);
            lastSolution.minCost = tabu.minCost;
            lastSolution.minPath = tabu.minPath;
            lastSolution.bestTimeStamp = tabu.bestTimeStamp;
            lastSolution.time = tabu.executionTime;
            lastSolution.printSolution();
            lastSolution.saveToFile(lastFilename);*/
            std::cout << std::endl;
        }
        break;

        case 5:
        {
            std::cout << "Insert the a (cooling) factor: ";
            std::cin >> a;
            std::cout << "Insert amount of epoques: ";
            std::cin >> epoque;
        }
        break;

        case 6:
        {
            //SimulatedAnnealing simulatedAnnealing(lastSolution);
            //simulatedAnnealing.simulatedAnnealing(stopTime, a, epoque);
            //lastSolution.minCost = simulatedAnnealing.minCost;
            //lastSolution.minPath = simulatedAnnealing.minPath;
            ///*lastSolution.minCostEnd = simulatedAnnealing.minCostEnd;
            //lastSolution.minPathEnd = simulatedAnnealing.minPathEnd;*/
            //lastSolution.bestTimeStamp = simulatedAnnealing.bestTimeStamp;
            //lastSolution.time = simulatedAnnealing.executionTime;
            //lastSolution.printSolution();
            //lastSolution.saveToFile(lastFilename);
            std::cout << std::endl;
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