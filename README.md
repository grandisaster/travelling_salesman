# TSPsolver

A C++ solver for the **Travelling Salesman Problem** (TSP).
Programming project, M1 AI, Université Grenoble Alpes.

## The problem

You get a set of cities and the distance between every pair of them. The goal is to find the shortest tour that visits each city exactly once and returns to the starting city.

We solve the **metric TSP**: the graph is complete and distances satisfy the triangle inequality. The problem is NP-hard. Exact methods take exponential time, and heuristics find good (not always optimal) tours quickly.

## Usage

```bash
bash build-TSPsolver.sh                                        # build into ./build
./build/TSPsolver <instance_path> <output_path> [-m <method>]  # solve an instance
./build/tests                                                  # run the tests
```

- **Input:** a TSPLIB instance file (`NODE_COORD_SECTION` or `EDGE_WEIGHT_SECTION` with `LOWER_DIAG_ROW`).
- **Output:** a TSPLIB tour file (`TYPE : TOUR`, `TOUR_SECTION`).

## Methods

| `-m`        | Description                                   |
|-------------|-----------------------------------------------|
| `trivial`   | Visits the cities in index order (baseline)   |
| `held-karp` | Exact dynamic programming, O(n² 2ⁿ)           |
| *TBD*       | At least one extra method (e.g. a heuristic)  |
