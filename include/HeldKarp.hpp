#pragma once
#include "Solver.hpp"

class HeldKarp : public Solver
{
public:
    static constexpr int MAX_N = 22;
    std::vector<int> solve(const Graph &graph) const override;
};