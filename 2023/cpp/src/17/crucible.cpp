// Day 17: Clumsy Crucible
// https://adventofcode.com/2023/day/17
//
// Created by Conor Pappas on 1/7/24.
//

#include <iostream>

#include <input.hpp>
#include <graph/algorithms/dijkstra_shortest_paths.hpp>

#include "data_types/heat_graph.hpp"

using namespace std;
using namespace crucible::data_types;

size_t min_heat(const HeatGraph& heat_graph) {
    constexpr Point start(0,0);
    const auto grid = heat_graph.get_grid();
    const Point end( grid.width()-1,  grid.height()-1);
    const auto heat_loss = [&heat_graph](auto arc) { return arc_data(heat_graph, arc).heat_loss; };
    const auto start_nodes = {
        heat_graph.get_vertex(start, Direction::Horizontal),
        heat_graph.get_vertex(start, Direction::Vertical)
    };
    auto distances = support::graph::algorithms::dijkstra_shortest_paths(heat_graph, start_nodes, heat_loss);
    const auto horiz = std::get<0>(distances)[heat_graph.get_vertex(end, Direction::Horizontal)];
    const auto vert = std::get<0>(distances)[heat_graph.get_vertex(end, Direction::Vertical)];
    return std::min(horiz, vert);
}

size_t part_1(const Grid& grid) {
    return min_heat(HeatGraph(grid, 1, 3));
}

size_t part_2(const Grid& grid) {
    return min_heat(HeatGraph(grid, 4, 10));
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("17", argc, argv);

    auto parse = [](const char c) { return c - '0'; };
    const auto heat_grid = support::parse_grid<heat>(lines, parse);

    cout << "Part 1: " << part_1(heat_grid) << endl;
    cout << "Part 2: " << part_2(heat_grid) << endl;
}
