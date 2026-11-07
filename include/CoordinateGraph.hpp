#pragma once

#include <functional>
#include <vector>
#include "Graph.hpp"
#include "metrics.hpp"

class CoordinateGraph : public Graph
{
public:
    using Metric = std::function<int(const Point &, const Point &)>;

    CoordinateGraph(const std::vector<Point> &coordinates, const Metric &metric);

    int dimension() const override;
    int distance(int i, int j) const override;

private:
    std::vector<Point> _coordinates;
    Metric _metric;
};
