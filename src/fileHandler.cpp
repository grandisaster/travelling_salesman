#include "fileHandler.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

bool FileHandler::fileWrite(const std::string &file_name,
                            const std::vector<int> &tour_path)
{
  std::ofstream outfile(file_name);
  if (!outfile)
  {
    return false;
  }

  const std::size_t separator = file_name.find_last_of("/\\");
  const std::string name = separator == std::string::npos
                               ? file_name
                               : file_name.substr(separator + 1);

  outfile << "NAME : " << name << "\n";
  outfile << "TYPE : TOUR\n";
  outfile << "DIMENSION : " << tour_path.size() << "\n";
  outfile << "TOUR_SECTION\n";
  for (int city : tour_path)
  {
    outfile << city << "\n";
  }
  outfile << "-1\nEOF\n";

  return static_cast<bool>(outfile);
}

CoordinatesGraph FileHandler::fileRead(const std::string &file_name)
{
  std::ifstream infile(file_name);
  if (!infile)
  {
    throw std::runtime_error("Error opening the file: " + file_name);
  }

  int dimension = 0;
  std::string edge_weight_type;
  std::string edge_weight_format;
  std::vector<std::pair<double, double>> coordinates;
  std::vector<bool> coordinate_seen;
  std::vector<int> explicit_weights;
  std::string section;
  std::string line;

  while (std::getline(infile, line))
  {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream row(line);
    std::string key;
    row >> key;

    if (key.empty())
    {
      continue;
    }
    if (key == "EOF")
    {
      break;
    }
    if (key == "NODE_COORD_SECTION")
    {
      if (dimension <= 0)
      {
        throw std::runtime_error("Missing DIMENSION in file: " + file_name);
      }
      coordinates.resize(dimension);
      coordinate_seen.resize(dimension, false);
      section = "coordinates";
      continue;
    }
    if (key == "EDGE_WEIGHT_SECTION")
    {
      section = "weights";
      continue;
    }
    if (key.find("_SECTION") != std::string::npos)
    {
      section.clear();
      continue;
    }

    if (section == "coordinates")
    {
      std::istringstream coordinate_row(line);
      int id;
      double x, y;
      if (!(coordinate_row >> id >> x >> y) || id < 1 || id > dimension)
      {
        throw std::runtime_error("Invalid coordinate in file: " + file_name);
      }
      coordinates[id - 1] = {x, y};
      coordinate_seen[id - 1] = true;
    }
    else if (section == "weights")
    {
      std::istringstream weight_row(line);
      int weight;
      while (weight_row >> weight)
      {
        explicit_weights.push_back(weight);
      }
    }
    else
    {
      std::string value;
      row >> value;
      if (key == "DIMENSION")
      {
        dimension = std::stoi(value);
      }
      else if (key == "EDGE_WEIGHT_TYPE")
      {
        edge_weight_type = value;
      }
      else if (key == "EDGE_WEIGHT_FORMAT")
      {
        edge_weight_format = value;
      }
    }
  }

  if (dimension <= 0)
  {
    throw std::runtime_error("Missing or invalid DIMENSION in file: " + file_name);
  }
  if (!coordinates.empty())
  {
    if (std::find(coordinate_seen.begin(), coordinate_seen.end(), false) !=
        coordinate_seen.end())
    {
      throw std::runtime_error("Missing coordinates in file: " + file_name);
    }
    if (edge_weight_type.empty())
    {
      edge_weight_type = "EUC_2D";
    }
    return CoordinatesGraph(dimension, coordinates, edge_weight_type);
  }

  if (edge_weight_type != "EXPLICIT" ||
      edge_weight_format != "LOWER_DIAG_ROW" ||
      explicit_weights.size() !=
          static_cast<std::size_t>(dimension) * (dimension + 1) / 2)
  {
    throw std::runtime_error("Invalid explicit weights in file: " + file_name);
  }
  return CoordinatesGraph(dimension, {}, edge_weight_type, explicit_weights);
}
