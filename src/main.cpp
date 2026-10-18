#include <iostream>
#include <fstream>
#include <vector>
#include "fileHandler.hpp"
#include <algorithm>
#include <string>
#include <stdexcept>

int main(int argc, char *argv[])
{
  if (argc != 3 && argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " <instance_path> <output_path> [-m <method>]" << std::endl;
    return 1;
  }

  std::string input = argv[1];
  std::string output = argv[2];
  std::string method = "trivial";
  std::string method_list[] = {"trivial", "held-karp"};

  if (argc == 5)
  {
    if (std::string(argv[3]) != "-m")
    {
      std::cerr << "Unknown option: " << argv[3] << std::endl;
      return 1;
    }
    method = argv[4];
    if (std::ranges::find(method_list, method) == std::ranges::end(method_list))
    {
      std::cerr << "This method is not supported" << std::endl;
      return 1;
    }
  }

  try
  {
    CoordinatesGraph graph = FileHandler::fileRead(input);

    std::vector<int> tour;
    if (method == "trivial")
    {
      tour = graph.visitCities();
    }
    else
    {
      throw std::runtime_error("not yet implemented");
    }
    std::cout << "Cities read: " << tour.size() << std::endl;

    bool success = FileHandler::fileWrite(output, tour);
    if (success == 0)
    {
      return 1;
    }
  }
  catch (const std::exception &error)
  {
    std::cerr << error.what() << std::endl;
    return 1;
  }

  return 0;
}
