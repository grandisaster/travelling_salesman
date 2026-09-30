#include "coordinatesGraph.hpp"
#include <iostream>

CoordinatesGraph::CoordinatesGraph(
    int dimensions,
    const std::vector<std::pair<double, double>> &coordinates) {
    _dimensions = dimensions;
    _coordinates = coordinates;
}

// double CoordinatesGraph::tourDistance() { 
//     for (int i = 0; i < _dimensions; ++i) { 
//         return 0.0;
//     }
// }

std::vector<int> CoordinatesGraph::visitCities() { 
    std::vector<int> cities;
    for (int i = 0; i < _dimensions; ++i) { 
        cities.push_back(i+1);
    }
    return cities;
}