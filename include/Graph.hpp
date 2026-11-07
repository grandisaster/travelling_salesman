#pragma once

class Graph
{
public:
    virtual ~Graph() = default;

    virtual int dimension() const = 0;
    virtual int distance(int i, int j) const = 0;
};