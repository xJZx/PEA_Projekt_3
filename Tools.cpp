#include "Tools.hpp"
#include <iomanip>

using namespace tinyxml2;

Tools::Tools() {
    numberOfCities = 0;
    //minPathEnd.resize(numberOfCities);
    minPath.resize(numberOfCities);
    minCost = INT_MAX;
    //minCostEnd = INT_MAX;
    typeOfSolution = 0;
    bestTimeStamp = 0;
    time = 0;
}

Tools::~Tools() {
    numberOfCities = 0;
    matrix.clear();
    //minPathEnd.clear();
    minPath.clear();
    minCost = 0;
    //minCostEnd = 0;
    typeOfSolution = 0;
    bestTimeStamp = 0;
    time = 0;
}

void Tools::readFromTXT(std::string filename) {
    std::fstream file;

    file.open(filename);

    if (file.good())
    {
        file >> numberOfCities;

        matrix.clear();
        matrix.resize(numberOfCities, std::vector<int>(numberOfCities));

        int weight;
        for (int first = 0; first < numberOfCities; first++)
        {
            for (int second = 0; second < numberOfCities; second++)
            {
                file >> weight;
                matrix[first][second] = weight;
                // bo graf asymetryczny skierowany
                //array[second][first] = weight;
            }
        }
        file.close();
    }
    else
    {
        std::cout << "Error occurred!\n";
    }
}

void Tools::readFromXML(const char* filename) {
    XMLDocument xmlDoc;

    if (xmlDoc.LoadFile(filename) != XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << filename << std::endl;
        return;
    }

    XMLElement* root = xmlDoc.RootElement();
    if (!root) {
        std::cerr << "Error: Root element not found." << std::endl;
        return;
    }

    // the hierarchy of the XML file
    XMLElement* graphElement = root->FirstChildElement("graph");

    XMLElement* vertexElement = graphElement->FirstChildElement("vertex");
    // Count the number of nodes
    numberOfCities = 0;
    while (vertexElement) {
        numberOfCities++;
        vertexElement = vertexElement->NextSiblingElement("vertex");
    }

    matrix.clear();
    matrix.resize(numberOfCities, std::vector<int>(numberOfCities, INT_MAX));

    // Iterate through the vertices and their edges, populate the adjacency matrix
    if (graphElement) {
        // Assuming the nodes are represented by "vertex" elements
        XMLElement* vertexElement = graphElement->FirstChildElement("vertex");
        int source = 0;
        while (vertexElement) {
            XMLElement* edgeElement = vertexElement->FirstChildElement("edge");
            while (edgeElement) {
                int target;
                double weight;
                edgeElement->QueryIntText(&target);
                edgeElement->QueryDoubleAttribute("cost", &weight);

                matrix[source][target] = weight;

                edgeElement = edgeElement->NextSiblingElement("edge");
            }

            vertexElement = vertexElement->NextSiblingElement("vertex");
            source++;
        }
    }
}

void Tools::saveToFile(std::string lastFilename) {
    std::ofstream file;
    file.open("results.txt");

    file << numberOfCities << "\n";

    file << lastFilename << "\n";

    file << minCost << "\n";

    file << minPath[numberOfCities - 1] << "\n";

    for (int i = 0; i < numberOfCities - 1; i++) {
        file << minPath[i] << "\n";
    }

    file << minPath[numberOfCities - 1] << "\n";

    /*file << "Time when Minimal Cost was found: " << bestTimeStamp << "[us]" << "\n";*/

    //------------------------------------------------------

    /*file << minPathEnd[numberOfCities - 1] << " -> ";

    for (int i = 0; i < numberOfCities - 1; i++) {
        file << minPathEnd[i] << " -> ";
    }

    file << minPathEnd[numberOfCities - 1] << "\n";

    file << "Minimal Cost at the end: " << minCostEnd << "\n";*/

    /*file << "Time at the end: " << time << "[us]" << "\n";*/
}

void Tools::readFromFile(std::string filename) {
    std::fstream file;
    std::string lastFilename;
    std::vector<int> path;
    int cities = 0;
    int firstCost = 0;

    file.open(filename);

    if (file.good())
    {
        file >> cities;
        std::cout << "City: " << cities << "\n";

        file >> lastFilename;

        file >> firstCost;

        int city = 0;
        for (int i = 0; i < cities; i++) {
            file >> city;
            path.push_back(city);
        }
        path.push_back(path[0]);

        for (int i = 0; i < path.size(); i++) {
            std::cout << path[i] << " ";
        }

        file.close();
    }
    else
    {
        std::cout << "Error occurred!\n";
    }

    const char* filenameXML = lastFilename.c_str();

    readFromXML(filenameXML);

    int cost = 0;
    for (int i = 0; i < cities - 1; i++) {
        cost += matrix[path[i]][path[i + 1]];
    }

    cost += matrix[path[cities - 1]][path[0]];

    std::cout << "Default cost: " << firstCost << "\n";
    std::cout << "Cost after recalculating: " << cost << "\n";

}

void Tools::print() {
    if (matrix.empty() == true) {
        std::cout << "Array is empty!\n";
        return;
    }

    std::cout << "Graph stored in the matrix: \n";
    for (int first = 0; first < numberOfCities; first++)
    {
        for (int second = 0; second < numberOfCities; second++)
        {
            std::cout << std::setw(4) << matrix[first][second];
        }

        std::cout << "\n";
    }
    std::cout << std::endl;
}

void Tools::printSolution() {
    std::cout << "Results:";

    if (!minPath.empty()) {

        std::cout << std::endl << "Best path:" << std::endl;

        std::cout << minPath[numberOfCities - 1] << " -> ";

        for (int i = 0; i < numberOfCities - 1; i++) {
            std::cout << minPath[i] << " -> ";
        }

        std::cout << minPath[numberOfCities - 1] << "\n";

        std::cout << "Minimal Cost: " << minCost << "\n";

        std::cout << "Time when Minimal Cost was found: " << bestTimeStamp << " [us]" << "\n";

        /*std::cout << std::endl << "Path at the end:" << std::endl;

        std::cout << minPathEnd[numberOfCities - 1] << " -> ";

        for (int i = 0; i < numberOfCities - 1; i++) {
            std::cout << minPathEnd[i] << " -> ";
        }

        std::cout << minPathEnd[numberOfCities - 1] << std::endl;

        std::cout << "Minimal Cost: " << minCostEnd << std::endl;*/

        std::cout << "Time: " << time << " [us]" << std::endl;

    }
}