std::vector<int> HeldKarp::solve(const Graph &graph) const
{
    const int n = graph.dimension();
    // крайние случаи: n == 1 → {0};  n > MAX_N → throw с понятным сообщением

    const int m = n - 1;
    const std::size_t numMasks = std::size_t{1} << m;
    const int INF = std::numeric_limits<int>::max() / 2; // ? почему /2

    // таблицы размера numMasks * m, «плоские»:
    // dp[mask * m + k] = длина лучшего пути 0 → ... → (k+1), проходящего ровно по mask
    std::vector<int> dp(___, INF);
    std::vector<int> parent(___, -1);

    // база: путь 0 → (k+1), маска из одного бита k
    for (int k = 0; k < m; ++k)
        dp[___] = graph.distance(0, k + 1);

    for (std::size_t mask = 1; mask < numMasks; ++mask)
        for (int k = 0; k < m; ++k)
        {
            // пропустить, если бита k нет в mask
            std::size_t prev = ___; // mask без бита k
            if (prev == 0)
                continue; // это база, уже заполнено
            for (int j = 0; j < m; ++j)
            {
                // пропустить, если бита j нет в prev
                // cand = dp[prev, j] + distance(j+1, k+1)
                // если cand лучше dp[mask, k] — запомнить и parent = j
            }
        }

    // замыкание: full = numMasks - 1; лучший k по dp[full, k] + distance(k+1, 0)

    // backtracking: начиная с (full, bestK):
    //   положить k+1 в тур, взять parent, убрать бит k из маски, перейти к parent
    //   пока маска не станет пустой; в конце добавить 0 и развернуть (std::reverse)
}