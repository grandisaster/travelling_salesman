#pragma once

#include <string>
#include <utility>
#include <vector>

class CoordinatesGraph
{
public:
    CoordinatesGraph(int dimensions,
                     const std::vector<std::pair<double, double>> &coordinates,
                     const std::string &edge_weight_type,
                     const std::vector<int> &explicit_weights = {});
    std::vector<int> visitCities() const;

private:
    std::vector<std::pair<double, double>> _coordinates;
    std::vector<int> _explicit_weights;
    int _dimensions;
    std::string _edge_weight_type;
};
