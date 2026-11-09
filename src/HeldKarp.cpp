#include <algorithm>
#include <vector>
#include <limits>
#include <stdexcept>

#include "HeldKarp.hpp"

std::vector<int> HeldKarp::solve(const Graph &graph) const
{
    const int n = graph.dimension();
    if (n == 1)
    {
        return {0};
    }
    if (n > MAX_N)
    {
        throw std::invalid_argument("n cannot be larger than MAX_N");
    }

    const int m = n - 1;
    const std::size_t numMasks = 1 << m;
    const int INF = std::numeric_limits<int>::max() / 2;

    std::vector<int> dp(numMasks * m, INF);
    std::vector<int> parent(numMasks * m, -1);

    for (int k = 0; k < m; ++k)
        dp[(1 << k) * m + k] = graph.distance(0, k + 1);

    for (std::size_t mask = 1; mask < numMasks; ++mask)
        for (int k = 0; k < m; ++k)
        {
            if (!(mask & (1 << k)))
            {
                continue;
            }
            std::size_t prev = mask ^ (1 << k);
            if (prev == 0)
            {
                continue;
            }
            for (int j = 0; j < m; ++j)
            {
                if (!(prev & (1 << j)))
                {
                    continue;
                }
                int cand = dp[prev * m + j] + graph.distance(j + 1, k + 1);
                if (cand < dp[mask * m + k])
                {
                    dp[mask * m + k] = cand;
                    parent[mask * m + k] = j;
                }
            }
        }

    const std::size_t full = numMasks - 1;
    int best = INF;
    int bestK = -1;
    for (int k = 0; k < m; ++k)
    {
        int cand = dp[full * m + k] + graph.distance(k + 1, 0);
        if (cand < best)
        {
            best = cand;
            bestK = k;
        }
    }

    std::vector<int> tour;
    std::size_t mask = full;
    int k = bestK;
    while (k != -1)
    {
        tour.push_back(k + 1);
        int pk = parent[mask * m + k];
        mask = mask ^ (1 << k);
        k = pk;
    }
    tour.push_back(0);
    std::reverse(tour.begin(), tour.end());
    return tour;
}