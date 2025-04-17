#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct Point
{
	int x, y;
};
struct Polygon
{
	std::vector< Point > points;
};



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
