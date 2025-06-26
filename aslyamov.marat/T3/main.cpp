#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
#include <numeric>
#include <iterator>
#include <sstream>
#include "Func.h"
#include "Functors.h"
#include "Geom.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Error: Incorrect argument\n";
        return 1;
    }
    std::ifstream inp { argv[1]};
    std::vector<Polygon> data;
    while (!inp.eof()) {
        if (inp.fail()) {
            inp.clear();
            inp.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::copy(
            std::istream_iterator<Polygon>(inp),
            std::istream_iterator<Polygon>(),
            std::back_inserter(data)
        );
    }
    std::string cmd;
    std::cout << std::fixed << std::setprecision(1);
    while (std::cin >> cmd) {
        std::string arg;
        std::cin >> arg;
        if (cmd == "AREA") {
            if (arg == "EVEN" || arg == "ODD" || arg == "MEAN" || (is_num(arg) && std::stoi(arg) >= 3)) {
                double out = area(arg, data);
                if (out > 0) {
                    std::cout << area(arg, data) << '\n';
                }
                else {
                    invalid_command();
                }
            }
            else {
                invalid_command();
            }
        }
        else if ((cmd == "MAX" || cmd == "MIN") && data.size() != 0) {
            if (arg == "AREA") {
                std::cout << max_min_area(cmd, data) << '\n';
            }
            else if (arg == "VERTEXES") {
                std::cout << max_min_verts(cmd, data) << '\n';
            }
            else {
                invalid_command();
            }
        }
        else if (cmd == "COUNT") {
            if (arg == "EVEN" || arg == "ODD" || (is_num(arg) && std::stoi(arg) >= 3)) {
                std::cout << count(arg, data) << '\n';
            }
            else {
                invalid_command();
            }
        }
        else if (cmd == "LESSAREA") {
            std::cin.unget();
            std::string line;
            std::getline(std::cin >> std::ws, line);
            std::istringstream iss(line);
            Polygon figure;
            if (iss >> figure) {
                std::string extra;
                if (iss >> extra) {
                    std::cout << "<INVALID COMMAND>\n";
                }
                else {
                    std::cout << less_area(figure, data) << '\n';
                }
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (cmd == "SAME") {
            std::cin.unget();
            std::string line;
            std::getline(std::cin >> std::ws, line);
            std::istringstream iss(line);
            Polygon figure;
            if (iss >> figure) {
                std::string extra;
                if (iss >> extra) {
                    std::cout << "<INVALID COMMAND>\n";
                }
                else {
                    std::cout << same(figure, data) << '\n';
                }
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else {
            invalid_command();
        }
    }
    return 0;
}
