// Day 10: Pipe Maze
// https://adventofcode.com/2023/day/10
//
// Created by Conor Pappas on 12/21/23.
//

#include <cassert>
#include <iostream>
#include <unordered_map>

#include "parse.hpp"
#include "input.hpp"

using namespace std;
using namespace maze;

constexpr std::array<Pipe::Direction, 4> all_directions() {
    return {
        Pipe::Direction::NORTH,
        Pipe::Direction::EAST,
        Pipe::Direction::SOUTH,
        Pipe::Direction::WEST
    };
}

std::tuple<size_t, Pipe::Direction> part_1(const Grid& grid) {
    size_t max_size = 0;
    Pipe::Direction found_direction;

    for (const auto& direction: all_directions()) {
        const auto size = std::distance(grid.begin(direction), grid.end());
        if (size > max_size) {
            max_size = size;
            found_direction = direction;
        }
    }
    return {max_size / 2, found_direction};
}

size_t part_2(const Grid&) {
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("10", argc, argv);

    const Grid grid = parse::parse_grid(lines);

    cout << "Part 1: " << get<0>(part_1(grid)) << endl;
    cout << "Part 2: " << part_2(grid) << endl;
}
