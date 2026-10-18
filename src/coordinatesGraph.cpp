#include "coordinatesGraph.hpp"

#include <stdexcept>

CoordinatesGraph::CoordinatesGraph(
    int dimensions,
    const std::vector<std::pair<double, double>> &coordinates,
    const std::string &edge_weight_type,
    const std::vector<int> &explicit_weights)
    : _coordinates(coordinates),
      _explicit_weights(explicit_weights),
      _dimensions(dimensions),
      _edge_weight_type(edge_weight_type)
{
    if (_dimensions <= 0)
    {
        throw std::invalid_argument("Graph dimension must be positive");
    }
}

std::vector<int> CoordinatesGraph::visitCities() const
{
    std::vector<int> cities;
    for (int i = 0; i < _dimensions; ++i)
    {
        cities.push_back(i + 1);
    }
    return cities;
}