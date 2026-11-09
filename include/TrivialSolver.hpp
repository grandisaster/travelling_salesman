#pragma once
#include "Solver.hpp"

class TrivialSolver : public Solver
{
public:
    std::vector<int> solve(const Graph &graph) const override;
};