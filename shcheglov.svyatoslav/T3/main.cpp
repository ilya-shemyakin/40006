#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <iterator>
#include "Polygon.h"
#include "Functions.h"
#include "Functors.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR no filename" << '\n';
        return 1;
    }
    std::vector<Polygon> data;

    std::ifstream input(argv[1]);
    if (!input.is_open())
    {
        std::cerr << "ERROR\n";
        return 1;
    }

    while (!input.eof())
    {
        std::copy(
            std::istream_iterator<Polygon>(input),
            std::istream_iterator<Polygon>(),
            std::back_inserter(data)
        );
        if (input.fail() && !input.eof())
        {
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string command;
    std::cout << std::fixed << std::setprecision(1);
    while (std::cin >> command)
    {
        if (command == "AREA")
        {
            std::string arg;
            if (std::cin >> arg)
            {
                if (arg == "ODD" || arg == "EVEN")
                {
                    std::cout << areaEvenOdd(arg, data) << '\n';
                }
                else if (arg == "MEAN")
                {
                    if (isEmpty(data))
                    {
                        invalidCommand();
                    }
                    else
                    {
                        std::cout << areaMean(data) << '\n';
                    }
                }
                else if (isNumber(arg) && std::stoi(arg) >= 3)
                {
                    std::cout << areaNum(std::stoi(arg), data) << '\n';
                }
                else
                {
                    invalidCommand();
                }
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "MAX" || command == "MIN")
        {
            if (isEmpty(data))
            {
                invalidCommand();
                continue;
            }
            std::string arg;
            if (std::cin >> arg)
            {
                if (command == "MAX" && arg == "AREA")
                {
                    std::cout << maxMinArea(command, data) << '\n';
                }
                else if (command == "MAX" && arg == "VERTEXES")
                {
                    std::cout << maxMinVertexes(command, data) << '\n';
                }
                else if (command == "MIN" && arg == "AREA")
                {
                    std::cout << maxMinArea(command, data) << '\n';
                }
                else if (command == "MIN" && arg == "VERTEXES")
                {
                    std::cout << maxMinVertexes(command, data) << '\n';
                }
                else
                {
                    invalidCommand();
                }
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "COUNT")
        {
            std::string arg;
            if (std::cin >> arg)
            {
                if (arg == "EVEN" || arg == "ODD")
                {
                    std::cout << count(arg, data) << '\n';
                }
                else if (isNumber(arg) && std::stoi(arg) >= 3)
                {
                    std::cout << count(std::stoi(arg), data) << '\n';
                }
                else
                {
                    invalidCommand();
                }
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "INFRAME")
        {
            Polygon poly;
            std::string token;
            if (std::cin >> token)
            {
                if (!isNumber(token) || std::stoi(token) < 3)
                {
                    invalidCommand();
                    continue;
                }

                int numVertices = std::stoi(token);
                bool valid = true;
                poly.points.clear();

                for (int i = 0; i < numVertices && valid; ++i)
                {
                    Point p;
                    if (!(std::cin >> p))
                    {
                        valid = false;
                    }
                    poly.points.push_back(p);
                }

                if (valid)
                {
                    char nextChar = std::cin.peek();
                    if (nextChar != '\n' && nextChar != EOF)
                    {
                        valid = false;
                    }
                }

                if (valid && poly.points.size() == static_cast<size_t>(numVertices))
                {
                    std::cout << (inFrame(poly, data) ? "<TRUE>" : "<FALSE>") << '\n';
                }
                else
                {
                    invalidCommand();
                }
            }
            else
            {
                invalidCommand();
            }
        }
        else if (command == "RIGHTSHAPES")
        {
            std::cout << rightShapes(data) << '\n';
        }
        else
        {
            invalidCommand();
        }
    }
    return 0;
}
