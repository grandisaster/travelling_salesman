#include "metrics.hpp"
#include <cmath>

namespace
{
    int nint(double x)
    {
        return static_cast<int>(std::round(x));
    }
    double toRadians(double coordinate)
    {
        const double PI = 3.141592;
        int deg = static_cast<int>(coordinate);
        double min = coordinate - deg;
        return (PI * (deg + 5.0 * min / 3.0) / 180);
    }
}

namespace metrics
{
    int euc2d(const Point &a, const Point &b)
    {
        double dx = a.first - b.first;
        double dy = a.second - b.second;
        return nint(std::sqrt(dx * dx + dy * dy));
    }
    int att(const Point &a, const Point &b)
    {
        double dx = a.first - b.first;
        double dy = a.second - b.second;
        double r = std::sqrt((dx * dx + dy * dy) / 10.0);
        int t = nint(r);
        if (t < r)
        {
            return t + 1;
        }
        else
        {
            return t;
        }
    }
    int geo(const Point &a, const Point &b)
    {
        double lat_x = toRadians(a.first);
        double lon_x = toRadians(a.second);
        double lat_y = toRadians(b.first);
        double lon_y = toRadians(b.second);
        double q1 = std::cos(lon_x - lon_y);
        double q2 = std::cos(lat_x - lat_y);
        double q3 = std::cos(lat_x + lat_y);
        double RRR = 6378.388;
        return static_cast<int>(RRR * std::acos(0.5 * ((1 + q1) * q2 - (1 - q1) * q3)) + 1.0);
    }
}