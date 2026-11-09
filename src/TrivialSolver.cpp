#include "TrivialSolver.hpp"

std::vector<int> TrivialSolver::solve(const Graph &graph) const
{
    std::vector<int> tour;
    const int n = graph.dimension();
    for (int i = 0; i < n; ++i)
    {
        tour.push_back(i);
    }
    return tour;
}