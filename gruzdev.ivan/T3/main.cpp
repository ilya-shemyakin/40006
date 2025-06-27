#include "Polygon.h"
#include "Commands.h"
#include <fstream>
#include <vector>
#include <iterator>
#include <limits>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << argv[0] << "\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);

    if (!inputFile.is_open())
    {
        std::cout << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    while (!inputFile.eof())
    {
        if (inputFile.fail())
        {
            inputFile.clear();
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::copy(
            std::istream_iterator<Polygon>(inputFile),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );
    }

    process_commands(polygons);
    return 0;
}
