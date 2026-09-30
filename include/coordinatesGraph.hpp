#pragma once

#include <string>
#include <utility>
#include <vector>

class CoordinatesGraph {
    public: 
    CoordinatesGraph(int dimensions, const std::vector<std::pair<double, double>> &coordinates);
    // double tourDistance();
    std::vector<int> visitCities();
    
    private:
    std::vector<std::pair<double, double>> _coordinates;
    int _dimensions;
};
