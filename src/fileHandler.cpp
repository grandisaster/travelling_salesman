#include "fileHandler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "coordinatesGraph.hpp"

bool FileHandler::fileWrite(const std::string &file_name,
                            const std::vector<int> &tour_path)
{
  std::string filename = file_name;

  std::ofstream outfile(filename);

  if (!outfile.is_open())
  {
    std::cerr << "Cannot open file for writing" << std::endl;
    return false;
  }

  outfile << "NAME : " << file_name << "\n";
  outfile << "TYPE : TOUR\n";
  outfile << "DIMENSION : " << tour_path.size() << "\n";
  outfile << "TOUR_SECTION\n";

  for (int node : tour_path)
  {
    outfile << node << "\n";
  }

  outfile << "-1\n";
  outfile << "EOF\n";

  outfile.close();

  std::cout << "File written successfully" << std::endl;
  return true;
}

CoordinatesGraph FileHandler::fileRead(const std::string &file_name)
{
  std::string filename = file_name;
  std::ifstream infile(filename);
  if (!infile)
  {
    throw std::runtime_error("Error opening the file: " + filename);
  }

  std::string line;
  int dimension = 0;
  std::vector<std::pair<double, double>> coords;
  bool reading_coords = false;
  std::string tspName;

  while (std::getline(infile, line))
  {
    if (line.empty())
      continue;
    if (line.find("EOF") != std::string::npos)
      break;

    if (!reading_coords)
    {
      if (line.find("NODE_COORD_SECTION") != std::string::npos)
      {
        reading_coords = true;
        coords.resize(dimension);
        continue;
      }

      std::stringstream ss(line);
      std::string key, colon;
      ss >> key >> colon;
      if (key == "NAME")
      {
        ss >> tspName;
      }
      else if (key == "DIMENSION")
      {
        ss >> dimension;
      }
    }
    else
    {
      std::stringstream ss(line);
      int id;
      double x, y;

      if (ss >> id >> x >> y)
      {
        if (id < 1)
          continue;
        if (static_cast<std::size_t>(id) > coords.size())
        {
          coords.resize(id);
        }
        coords[id - 1] = {x, y};
      }
    }
  }

  infile.close();

  if (coords.empty())
  {
    throw std::runtime_error("No coordinates found in file: " + filename);
  }

  return CoordinatesGraph(static_cast<int>(coords.size()), coords);
}
