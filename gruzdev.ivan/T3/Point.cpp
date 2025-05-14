#include "Point.h"

std::istream& operator>>(std::istream& in, Point& point)
{
    char symbol;
    if (!(in >> symbol && symbol == '(' &&
        in >> point.x >> symbol && symbol == ';' &&
        in >> point.y >> symbol && symbol == ')'))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}
