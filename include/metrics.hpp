#pragma once

#include <utility>

using Point = std::pair<double, double>;

namespace metrics
{
    int euc2d(const Point &a, const Point &b);
    int geo(const Point &a, const Point &b);
    int att(const Point &a, const Point &b);
}