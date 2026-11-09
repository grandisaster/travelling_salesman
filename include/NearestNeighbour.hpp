#pragma once
#include "Solver.hpp"

class NearestNeighbour : public Solver
{
public:
    std::vector<int> solve(const Graph &graph) const override;
};