#pragma once
#include "Solver.hpp"
#include "NearestNeighbour.hpp"

class TwoOpt : public Solver
{
public:
    std::vector<int> solve(const Graph &graph) const override;

private:
    NearestNeighbour _initial;
};