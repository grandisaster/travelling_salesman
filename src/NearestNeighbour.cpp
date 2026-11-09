#include <vector>
#include "NearestNeighbour.hpp"

std::vector<int> NearestNeighbour::solve(const Graph &graph) const
{
    const int n = graph.dimension();
    std::vector<bool> visited(n, false);
    std::vector<int> tour;
    tour.reserve(n);

    int current = 0;
    visited[current] = true;
    tour.push_back(current);
    while (static_cast<int>(tour.size()) < n)
    {
        int best = -1;
        for (int city = 0; city < n; city++)
        {
            if (visited[city])
            {
                continue;
            }

            if (best == -1 || graph.distance(current, city) < graph.distance(current, best))
            {
                best = city;
            }
        }
        visited[best] = true;
        tour.push_back(best);
        current = best;
    }
    return tour;
}