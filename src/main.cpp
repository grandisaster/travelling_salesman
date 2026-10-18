#include <exception>
#include <iostream>
#include <string>

#include "fileHandler.hpp"

int main(int argc, char *argv[])
{
  if (argc != 3 && argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " <instance_path> <output_path> [-m <method>]" << std::endl;
    return 1;
  }

  if (argc == 5 && std::string(argv[3]) != "-m")
  {
    std::cerr << "Unknown option: " << argv[3] << std::endl;
    return 1;
  }

  const std::string method = argc == 5 ? argv[4] : "trivial";
  if (method != "trivial")
  {
    std::cerr << "Method not implemented: " << method << std::endl;
    return 1;
  }

  try
  {
    const CoordinatesGraph graph = FileHandler::fileRead(argv[1]);
    const auto tour = graph.visitCities();
    std::cout << "Cities read: " << tour.size() << std::endl;

    if (!FileHandler::fileWrite(argv[2], tour))
    {
      std::cerr << "Cannot write tour file: " << argv[2] << std::endl;
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
