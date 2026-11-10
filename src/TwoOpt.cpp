#include "TwoOpt.hpp"
#include <algorithm>

std::vector<int> TwoOpt::solve(const Graph &graph) const
{
    std::vector<int> tour = _initial.solve(graph);
    const int n = static_cast<int>(tour.size());
    if (n < 4)
    {
        return tour;
    }

    bool improved = true;

    while (improved)
    {
        improved = false;
        for (int i = 0; i < n - 1; ++i)
        {
            for (int j = i + 2; j < n; ++j)
            {
                if (i == 0 && j == n - 1)
                    continue;

                const int a = tour[i];
                const int b = tour[i + 1];
                const int c = tour[j];
                const int d = tour[(j + 1) % n];

                const int delta = graph.distance(a, c) + graph.distance(b, d) - graph.distance(a, b) - graph.distance(c, d);
                if (delta < 0)
                {
                    std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
    return tour;
}