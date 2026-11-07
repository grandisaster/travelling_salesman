#pragma once
#include <vector>
#include "Graph.hpp"

class Solver
{
public:
    virtual ~Solver() = default;
    virtual std::vector<int> solve(const Graph &graph) const = 0;
};