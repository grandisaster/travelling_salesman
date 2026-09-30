#pragma once

#include <string>
#include <utility>
#include <vector>

class CoordinatesGraph
{
public:
    CoordinatesGraph(int dimensions, const std::vector<std::pair<double, double>> &coordinates, const std::string &edge_weight_type);
    // double tourDistance();
    std::vector<int> visitCities();

private:
    std::vector<std::pair<double, double>> _coordinates;
    int _dimensions;
    std::string _edge_weight_type;
};
