#include "fileHandler.hpp"
#include "ExplicitGraph.hpp"
#include "HeldKarp.hpp"
#include "TrivialSolver.hpp"
#include "metrics.hpp"

#include <algorithm>
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

        const auto graph = FileHandler::fileRead(input.path().string());
        require(graph->dimension() == 3, "Coordinate graph must have 3 cities");
        require(TrivialSolver().solve(*graph) == std::vector<int>({0, 1, 2}),
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

        const auto graph = FileHandler::fileRead(input.path().string());
        require(graph->distance(1, 0) == 4 && graph->distance(0, 2) == 8 &&
                    graph->distance(2, 1) == 9 && graph->distance(1, 1) == 0,
                "Explicit weights must be read into a symmetric matrix");
        require(TrivialSolver().solve(*graph) == std::vector<int>({0, 1, 2}),
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

    // Tests below generated with Claude (2026-11-09)
    int tourLength(const Graph &graph, const std::vector<int> &tour)
    {
        int length = 0;
        for (std::size_t i = 0; i < tour.size(); ++i)
        {
            length += graph.distance(tour[i], tour[(i + 1) % tour.size()]);
        }
        return length;
    }

    bool isPermutation(std::vector<int> tour, int n)
    {
        std::sort(tour.begin(), tour.end());
        std::vector<int> expected(n);
        for (int i = 0; i < n; ++i)
        {
            expected[i] = i;
        }
        return tour == expected;
    }

    void testMetrics()
    {
        require(metrics::euc2d({0.0, 0.0}, {3.0, 4.0}) == 5,
                "EUC_2D distance between (0,0) and (3,4) must be 5");
        // burma14, cities 1 and 2 (TSPLIB)
        require(metrics::geo({16.47, 96.10}, {16.47, 94.44}) == 153,
                "GEO distance between burma14 cities 1 and 2 must be 153");
    }

    void testHeldKarpOnExample()
    {
        // 4-city example from the project statement (A, B, C, D): optimum is 18
        const ExplicitGraph graph(4, {0, 5, 0, 8, 4, 0, 6, 7, 3, 0});
        const std::vector<int> tour = HeldKarp().solve(graph);
        require(isPermutation(tour, 4), "Held-Karp must visit every city exactly once");
        require(tour.front() == 0, "Held-Karp tour must start at city 0");
        require(tourLength(graph, tour) == 18, "Held-Karp must find the optimal tour (18)");
    }

    void testHeldKarpSmallGraphs()
    {
        for (int n = 1; n <= 3; ++n)
        {
            const ExplicitGraph graph(n, std::vector<int>(n * (n + 1) / 2, 1));
            require(isPermutation(HeldKarp().solve(graph), n),
                    "Held-Karp must return a valid tour for n = " + std::to_string(n));
        }
    }

    void testHeldKarpRejectsLargeGraphs()
    {
        const int n = HeldKarp::MAX_N + 1;
        const ExplicitGraph graph(n, std::vector<int>(n * (n + 1) / 2, 1));
        bool rejected = false;
        try
        {
            static_cast<void>(HeldKarp().solve(graph));
        }
        catch (const std::invalid_argument &)
        {
            rejected = true;
        }
        require(rejected, "Held-Karp must reject graphs larger than MAX_N");
    }

}

int main()
{
    const std::vector<std::pair<std::string, void (*)()>> tests{
        {"coordinate input and trivial tour", testCoordinateInputAndTrivialTour},
        {"explicit lower-diagonal input", testExplicitLowerDiagonalInput},
        {"malformed input rejection", testRejectsMalformedInputs},
        {"valid tour output", testWritesValidTourFile},
        {"TSPLIB metrics", testMetrics},
        {"Held-Karp on statement example", testHeldKarpOnExample},
        {"Held-Karp on tiny graphs", testHeldKarpSmallGraphs},
        {"Held-Karp size limit", testHeldKarpRejectsLargeGraphs},
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
