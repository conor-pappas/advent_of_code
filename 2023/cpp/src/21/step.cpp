// Day 21: Step Counter
// https://adventofcode.com/2023/day/21
//
// Created by Conor Pappas on 1/16/24.
//

#include <iostream>
#include <vector>

#include <input.hpp>
#include <graph/algorithms/dijkstra_shortest_paths.hpp>

#include "data_types/tile.hpp"
#include "data_types/graph.hpp"

using namespace std;
using namespace step;
using namespace step::data_types;

bool parity(const Point& p) {
    return (p.x() % 2 == p.y() % 2);
}

size_t part_1(const Graph& graph, const Grid& grid, const size_t target_distance) {
    Point start;
    for(size_t y = 0; y < grid.height(); ++y) {
        for(size_t x = 0; x < grid.width(); ++x) {
            if (grid.get(Point {x, y}) == Tile::Start) start = {x, y};
        }
    }

    const bool target_parity = parity(start) != (target_distance % 2);

    const auto weight = [](const auto&) { return 1; };
    const auto paths = support::graph::algorithms::dijkstra_shortest_paths(graph, get_vertex(grid, start), weight);
    const auto path_sizes = get<0>(paths);

    size_t result = 0;
    for(size_t y = 0; y < grid.height(); ++y) {
        for(size_t x = 0; x < grid.width(); ++x) {
            Point point {x, y};
            const auto vertex = get_vertex(grid, point);
            if(path_sizes.contains(vertex) && path_sizes.at(vertex) <= target_distance) {
                if(parity(point) == target_parity) ++ result;
            }
        }
    }
    return result;
}

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("21", argc, argv);

    const auto grid = support::parse_grid<data_types::Tile>(lines);
    const auto graph = make_graph(grid);

    cout << "Part 1: " << part_1(graph, grid, 64) << endl;
    cout << "Part 2: " << part_2() << endl;
}


