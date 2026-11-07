#pragma once
#include <vector>

#include "Graph.hpp"

class ExplicitGraph : public Graph
{
public:
    ExplicitGraph(int dimension, const std::vector<int> &lowerDiagRow);

    int dimension() const override;
    int distance(int i, int j) const override;

private:
    int _dimension;
    std::vector<std::vector<int>> _matrix;
};