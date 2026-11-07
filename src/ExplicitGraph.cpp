#include "ExplicitGraph.hpp"
#include <stdexcept>

ExplicitGraph::ExplicitGraph(int dimension, const std::vector<int> &lowerDiagRow)
    : _dimension(dimension),
      _matrix(dimension, std::vector<int>(dimension, 0))
{
    if (!(dimension > 0 && lowerDiagRow.size() == std::size_t(dimension * (dimension + 1) / 2)))
    {
        throw std::invalid_argument("Not valid matrix");
    }

    std::size_t pos = 0;
    for (int i = 0; i < _dimension; ++i)
        for (int j = 0; j <= i; ++j)
        {
            _matrix[i][j] = lowerDiagRow[pos];
            _matrix[j][i] = lowerDiagRow[pos];
            pos++;
        }
}

int ExplicitGraph::dimension() const { return _dimension; }

int ExplicitGraph::distance(int i, int j) const { return _matrix[i][j]; }