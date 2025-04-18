#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


struct Point
{
    int x, y;
};
struct Polygon
{
    std::vector< Point > points;
};


bool isNumber(std::string& arg);
void areaEvenOdd(std::string& arg, std::vector<Polygon>& data);
void areaMean(std::vector<Polygon>& data);
void areaNum(int arg, std::vector<Polygon>& data);
double calculateArea(std::vector<Point>& points);


int main() {
    const std::string filename = "input.TXT";
    std::vector<Polygon> data;
    std::ifstream input{ filename };

    while (!input.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(input),
            std::istream_iterator<Polygon>(),
            std::back_inserter(data)
        );

        if (input.fail() && !input.eof()) {
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    for (Polygon i : data) {
        for (Point j : i.points) {
            std::cout << j.x << ';' << j.y << ' ';
        }
        std::cout << '\n';
    }

    std::string command;
    while (std::cin >> command) {
        if (command == "AREA") {
            std::string arg;
            std::cin >> arg;
            if (arg == "ODD" || arg == "EVEN") {
                areaEvenOdd(arg, data);
            }
            else if (arg == "MEAN") {
                areaMean(data);
            }
            else if (isNumber(arg)) {
                areaNum(std::stoi(arg), data);
            }
        }
        else if (command == "MAX") {
            // ...
        }
        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}


std::istream& operator>>(std::istream& in, Point& p) {
    char ch;
    if (in >> ch && ch == '(' && in >> p.x >> ch && ch == ';' && in >> p.y >> ch && ch == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
    poly.points.clear();
    int count;

    if (!(in >> count)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i) {
        Point p;
        if (!(in >> p)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        poly.points.push_back(p);
    }

    return in;
}

bool isNumber(std::string& arg)
{
    try {
        std::stoi(arg);
        return true;
    }
    catch (...) {
        return false;
    }
}

void areaEvenOdd(std::string& arg, std::vector<Polygon>& data) {
    const int mod = (arg == "EVEN") ? 0 : 1;

    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [mod](double sum, Polygon& figure) {
            if (figure.points.size() % 2 == mod) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );

    std::cout << output << '\n';
}

void areaMean(std::vector<Polygon>& data) {
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [](double sum, Polygon& figure) {
            return sum + calculateArea(figure.points);
        }
    );

    std::cout << output / data.size() << '\n';
}

void areaNum(int arg, std::vector<Polygon>& data){
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [arg](double sum, Polygon& figure) {
            if (figure.points.size() == arg) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );

    std::cout << output << '\n';
}

double calculateArea(std::vector<Point>& points)
{
    double area = 0.0;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[i].y * points[j].x);
    }

    return std::abs(area) / 2.0;
}
