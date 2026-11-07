#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Graph.hpp"

class FileHandler
{
public:
    static std::unique_ptr<Graph> fileRead(const std::string &file_name);

    static bool fileWrite(const std::string &file_name, const std::vector<int> &tour_path);
};
