#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include "fileHandler.hpp"
#include "TrivialSolver.hpp"
#include "HeldKarp.hpp"
#include "NearestNeighbour.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_path> <output_path> [-m <method>]" << std::endl;
        return 1;
    }

    const std::string input = argv[1];
    const std::string output = argv[2];
    std::string method = "trivial";
    const std::string method_list[] = {"trivial", "held-karp"};

    if (argc == 5)
    {
        if (std::string(argv[3]) != "-m")
        {
            std::cerr << "Unknown option: " << argv[3] << std::endl;
            return 1;
        }
        method = argv[4];
        if (std::ranges::find(method_list, method) == std::ranges::end(method_list))
        {
            std::cerr << "This method is not supported" << std::endl;
            return 1;
        }
    }

    try
    {
        std::unique_ptr<Graph> graph = FileHandler::fileRead(input);
        std::unique_ptr<Solver> solver;

        if (method == "trivial")
        {
            solver = std::make_unique<TrivialSolver>();
        }
        else if (method == "held-karp")
        {
            solver = std::make_unique<HeldKarp>();
        }
        else if (method == "nn")
        {
            solver = std::make_unique<NearestNeighbour>();
        }
        else
        {
            throw std::runtime_error("not yet implemented");
        }

        std::vector<int> tour = solver->solve(*graph);
        for (int &city : tour)
        {
            city += 1;
        }

        std::cout << "Cities read: " << tour.size() << std::endl;

        if (!FileHandler::fileWrite(output, tour))
        {
            std::cerr << "Cannot write tour file: " << output << std::endl;
            return 1;
        }
        std::cout << "File written successfully" << std::endl;
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    return 0;
}
