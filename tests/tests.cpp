#include "fileHandler.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
class TemporaryFile
{
public:
    explicit TemporaryFile(const std::string &extension)
    {
        const auto timestamp = std::chrono::steady_clock::now()
                                  .time_since_epoch()
                                  .count();
        _path = std::filesystem::temp_directory_path() /
                ("tsp_solver_test_" + std::to_string(timestamp) + extension);
    }

    ~TemporaryFile()
    {
        std::error_code error;
        std::filesystem::remove(_path, error);
    }

    const std::filesystem::path &path() const
    {
        return _path;
    }

    void write(const std::string &contents) const
    {
        std::ofstream output(_path);
        if (!output)
        {
            throw std::runtime_error("Could not create temporary test file");
        }
        output << contents;
        if (!output)
        {
            throw std::runtime_error("Could not write temporary test file");
        }
    }

private:
    std::filesystem::path _path;
};

void require(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

void testCoordinateInputAndTrivialTour()
{
    TemporaryFile input(".tsp");
    input.write(
        "NAME : tiny\n"
        "TYPE : TSP\n"
        "DIMENSION : 3\n"
        "EDGE_WEIGHT_TYPE : GEO\n"
        "NODE_COORD_SECTION\n"
        "3 3.0 3.0\n"
        "1 1.0 1.0\n"
        "2 2.0 2.0\n"
        "EOF\n");

    const CoordinatesGraph graph = FileHandler::fileRead(input.path().string());
    require(graph.visitCities() == std::vector<int>({1, 2, 3}),
            "Trivial method must return city indices in ascending order");
}

void testExplicitLowerDiagonalInput()
{
    TemporaryFile input(".tsp");
    input.write(
        "NAME: tiny_explicit\n"
        "TYPE: TSP\n"
        "DIMENSION: 3\n"
        "EDGE_WEIGHT_TYPE: EXPLICIT\n"
        "EDGE_WEIGHT_FORMAT: LOWER_DIAG_ROW\n"
        "EDGE_WEIGHT_SECTION\n"
        "0 4 0\n"
        "8 9 0\n"
        "DISPLAY_DATA_SECTION\n"
        "1 10.0 10.0\n"
        "EOF\n");

    const CoordinatesGraph graph = FileHandler::fileRead(input.path().string());
    require(graph.visitCities() == std::vector<int>({1, 2, 3}),
            "Explicit instances must be accepted by the trivial method");
}

void testRejectsMalformedInputs()
{
    TemporaryFile input(".tsp");
    input.write(
        "NAME: incomplete\n"
        "TYPE: TSP\n"
        "DIMENSION: 3\n"
        "NODE_COORD_SECTION\n"
        "1 1.0 1.0\n"
        "2 2.0 2.0\n"
        "EOF\n");

    bool rejected = false;
    try
    {
        static_cast<void>(FileHandler::fileRead(input.path().string()));
    }
    catch (const std::runtime_error &)
    {
        rejected = true;
    }
    require(rejected, "Input with too few coordinates must be rejected");
}

void testWritesValidTourFile()
{
    TemporaryFile output(".tour");
    const std::vector<int> tour{2, 1, 3};
    require(FileHandler::fileWrite(output.path().string(), tour),
            "Writing a valid tour should succeed");

    std::ifstream input(output.path());
    std::ostringstream contents;
    contents << input.rdbuf();

    const std::string expected =
        "NAME : " + output.path().filename().string() +
        "\nTYPE : TOUR\nDIMENSION : 3\nTOUR_SECTION\n2\n1\n3\n-1\nEOF\n";
    require(contents.str() == expected,
            "Tour output must contain the required TSPLIB header and terminators");
}

}

int main()
{
    const std::vector<std::pair<std::string, void (*)()>> tests{
        {"coordinate input and trivial tour", testCoordinateInputAndTrivialTour},
        {"explicit lower-diagonal input", testExplicitLowerDiagonalInput},
        {"malformed input rejection", testRejectsMalformedInputs},
        {"valid tour output", testWritesValidTourFile},
    };

    std::size_t failures = 0;
    for (const auto &test : tests)
    {
        try
        {
            test.second();
            std::cout << "[PASS] " << test.first << '\n';
        }
        catch (const std::exception &error)
        {
            ++failures;
            std::cerr << "[FAIL] " << test.first << ": " << error.what() << '\n';
        }
    }

    if (failures != 0)
    {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << tests.size() << " test(s) passed\n";
    return 0;
}
