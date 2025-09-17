#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <iomanip>
#include "PolygonProcessor.h"
#include "CommandHandlers.h"
#include "AdditionalCommands.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Missing filename parameter\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: Cannot open file " << argv[1] << "\n";
        return 1;
    }

    std::vector<Shape> shapes;
    Shape tempShape;

    while (file >> tempShape) {
        shapes.push_back(tempShape);
        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cout << std::fixed << std::setprecision(1);

    std::string commandLine;
    while (std::getline(std::cin, commandLine)) {
        if (commandLine.empty()) continue;

        std::istringstream iss(commandLine);
        std::string command, argument;

        if (!(iss >> command)) {
            handleInvalidCommand();
            continue;
        }

        if (command == "AREA") {
            if (!(iss >> argument)) {
                handleInvalidCommand();
                continue;
            }

            if (argument == "EVEN") {
                std::cout << calculateEvenOddArea(shapes, VertexParity::EVEN) << "\n";
            }
            else if (argument == "ODD") {
                std::cout << calculateEvenOddArea(shapes, VertexParity::ODD) << "\n";
            }
            else if (argument == "MEAN") {
                if (shapes.empty()) {
                    handleInvalidCommand();
                }
                else {
                    std::cout << calculateAverageArea(shapes) << "\n";
                }
            }
            else if (std::all_of(argument.begin(), argument.end(), ::isdigit)) {
                int count = std::stoi(argument);
                if (count < 3) {
                    handleInvalidCommand();
                }
                else {
                    std::cout << calculateAreaByVertexCount(shapes, count) << "\n";
                }
            }
            else {
                handleInvalidCommand();
            }
        }
        else if (command == "MAX") {
            if (shapes.empty()) {
                handleInvalidCommand();
                continue;
            }

            if (iss >> argument) {
                if (argument == "AREA") {
                    std::cout << findExtremeArea(shapes, true) << "\n";
                }
                else if (argument == "VERTEXES") {
                    std::cout << findExtremeVertexCount(shapes, true) << "\n";
                }
                else {
                    handleInvalidCommand();
                }
            }
            else {
                handleInvalidCommand();
            }
        }
        else if (command == "MIN") {
            if (shapes.empty()) {
                handleInvalidCommand();
                continue;
            }

            if (iss >> argument) {
                if (argument == "AREA") {
                    std::cout << findExtremeArea(shapes, false) << "\n";
                }
                else if (argument == "VERTEXES") {
                    std::cout << findExtremeVertexCount(shapes, false) << "\n";
                }
                else {
                    handleInvalidCommand();
                }
            }
            else {
                handleInvalidCommand();
            }
        }
        else if (command == "COUNT") {
            if (!(iss >> argument)) {
                handleInvalidCommand();
                continue;
            }

            if (argument == "EVEN") {
                std::cout << countShapesByParity(shapes, VertexParity::EVEN) << "\n";
            }
            else if (argument == "ODD") {
                std::cout << countShapesByParity(shapes, VertexParity::ODD) << "\n";
            }
            else if (std::all_of(argument.begin(), argument.end(), ::isdigit)) {
                int count = std::stoi(argument);
                if (count < 3) {
                    handleInvalidCommand();
                }
                else {
                    std::cout << countShapesByVertexCount(shapes, count) << "\n";
                }
            }
            else {
                handleInvalidCommand();
            }
        }
        else if (command == "PERMS") {
            std::string remaining;
            std::getline(iss, remaining);

            std::istringstream shapeStream(argument + remaining);
            Shape target;
            if (!(shapeStream >> target)) {
                handleInvalidCommand();
                continue;
            }

            std::cout << countPermutationMatches(shapes, target) << "\n";
        }
        else if (command == "MAXSEQ") {
            std::string remaining;
            std::getline(iss, remaining);

            std::istringstream shapeStream(argument + remaining);
            Shape target;
            if (!(shapeStream >> target)) {
                handleInvalidCommand();
                continue;
            }

            std::cout << findMaxConsecutiveSequence(shapes, target) << "\n";
        }
        else {
            handleInvalidCommand();
        }
    }

    return 0;
}
