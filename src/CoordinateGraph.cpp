#include "CoordinateGraph.hpp"

#include <stdexcept>

CoordinateGraph::CoordinateGraph(const std::vector<Point> &coordinates, const Metric &metric)
    : _coordinates(coordinates), _metric(metric)
{
    if (_coordinates.empty())
    {
        throw std::invalid_argument("No `coordinates`");
    }
    if (!_metric)
    {
        throw std::invalid_argument("No `metric`");
    }
}

int CoordinateGraph::dimension() const
{
    return static_cast<int>(_coordinates.size());
}

int CoordinateGraph::distance(int i, int j) const
{
    return _metric(_coordinates[i], _coordinates[j]);
}