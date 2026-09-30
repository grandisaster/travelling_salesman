#include <iostream>
#include <fstream>
#include <vector>
#include "fileHandler.hpp"

int main() {
  std::string input = "TSP-instances/coord/berlin52";
  std::string output = "berlin52";

  try {
    CoordinatesGraph graph = FileHandler::fileRead(input);

    std::vector<int> tour = graph.visitCities();
    std::cout << "Cities read: " << tour.size() << std::endl;

    FileHandler::fileWrite(output, tour);
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }

  return 0;
}

