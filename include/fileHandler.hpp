#pragma once

#include <string>
#include <utility>
#include <vector>
#include "coordinatesGraph.hpp"

class FileHandler {
    public:
        static CoordinatesGraph fileRead(const std::string& file_name);

        static bool fileWrite(const std::string& file_name, const std::vector<int>& tour_path);

    private:
        static const std::string _input_extension;
        static const std::string _output_extension;
};
