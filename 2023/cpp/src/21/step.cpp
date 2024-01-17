// Day 21: Step Counter
// https://adventofcode.com/2023/day/21
//
// Created by Conor Pappas on 1/16/24.
//

#include <cassert>
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

using PathSizes = map<vertex_descriptor, size_t>;

// TODO: should have built BFS instead, but using Dijkstra cause I already have it built
PathSizes shortest_paths(const Graph& graph, const Grid& grid, const Point& start) {
    const auto weight = [](const auto&) { return 1; };
    const auto paths = support::graph::algorithms::dijkstra_shortest_paths(graph, get_vertex(grid, start), weight);
    return get<0>(paths);
}

template<typename T>
using parity_pair = std::array<T,2>;

parity_pair<size_t> accessible_count(const PathSizes& path_sizes, const Grid& grid, const size_t target_distance, const Point& start) {
    parity_pair<size_t> result {};
    for(size_t y = 0; y < grid.height(); ++y) {
        for(size_t x = 0; x < grid.width(); ++x) {
            Point point {x, y};
            const auto vertex = get_vertex(grid, point);
            if(path_sizes.contains(vertex) && path_sizes.at(vertex) <= target_distance) {
                ++result[parity(point) != parity(start)];
            }
        }
    }
    return result;
}

size_t part_1(const Grid& grid, const Point& start, const size_t target_distance) {
    const Graph graph = make_graph(grid);
    const bool target_parity = parity(start) != (target_distance % 2);
    return accessible_count(shortest_paths(graph, grid, start), grid, target_distance, start)[target_parity];
}

size_t calculate_tesselated_count(
    const parity_pair<size_t> primary_counts,
    const size_t& total_count,
    const size_t size,
    const size_t target_distance)
{
    const auto tesselation_radius = target_distance / size;
    const auto grid_length = 2*tesselation_radius + 1;
    const auto block_count = tesselation_radius*tesselation_radius;

    const auto primary_slice_number = primary_counts[!(tesselation_radius % 2)];
    const auto secondary_count = total_count - primary_counts[0] - primary_counts[1];

    size_t result = block_count * total_count;
    result += grid_length * primary_slice_number;
    result += tesselation_radius * secondary_count;
    return result;
}

size_t part_2(const Grid& grid, const Point& start, const size_t target_distance) {
    const auto graph = make_graph(grid);

    assert(grid.width() == grid.height());
    const auto size = grid.width();
    assert(size % 2 == 1);

    const auto half_size = size / 2;
    assert(start.x() == half_size && start.y() == half_size);
    assert(target_distance % size == half_size);

    const auto path_sizes = shortest_paths(graph, grid, start);
    size_t total_accessible_count = 0;
    for(size_t y = 0; y < grid.height(); ++y) {
        for(size_t x = 0; x < grid.width(); ++x) {
            const auto vertex = get_vertex(grid, {x,y});
            if(path_sizes.contains(vertex)) {
                assert(path_sizes.at(vertex) <= size);
                ++total_accessible_count;
            }
        }
    }

    const auto primary_counts =  accessible_count(path_sizes, grid, half_size, start);
    return calculate_tesselated_count(primary_counts, total_accessible_count, size, target_distance);
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("21", argc, argv);

    const auto grid = support::parse_grid<Tile>(lines);
    Point start;
    for(size_t y = 0; y < grid.height(); ++y) {
        for(size_t x = 0; x < grid.width(); ++x) {
            if (grid.get(Point {x, y}) == Tile::Start) start = {x, y};
        }
    }
    cout << "Part 1: " << part_1(grid, start, 64) << endl;
    cout << "Part 2: " << endl << part_2(grid, start, 26501365) << endl;
}


