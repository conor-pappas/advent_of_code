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

std::tuple<size_t, Pipe> part_1(const Grid& grid) {
    size_t max_size = 0;
    std::vector<Pipe::Direction> found_directions;

    for (const auto& direction: all_directions()) {
        const auto size = static_cast<size_t>(std::distance(grid.begin(direction), grid.end()));
        if (size > max_size) {
            max_size = size;
            found_directions = {direction};
        } else if (size == max_size) {
            found_directions.push_back(direction);
        }
    }
    assert(found_directions.size() == 2);
    return {max_size / 2, {found_directions[0], found_directions[1]}};
}

enum class Coloring: char {
    OUT = -1,
    ON = 0,
    IN = 1
};

using ColoringMap = unordered_map<Grid::Point, Coloring, Grid::Point::Hasher>;

void handle_pipe(const Pipe& pipe, Pipe::Direction& bend_direction, Coloring& parity) {
    bool swap_parity = false;
    if(pipe == pipes::horizontal) return;
    if(pipe == pipes::vertical) swap_parity = true;
    else if (pipe == pipes::north_east) bend_direction = Pipe::Direction::NORTH;
    else if(pipe == pipes::south_east) bend_direction = Pipe::Direction::SOUTH;
    else if(pipe == pipes::north_west) {
        assert(bend_direction != Pipe::Direction::EAST);
        if(bend_direction == Pipe::Direction::SOUTH) swap_parity = true;
        bend_direction = Pipe::Direction::EAST;
    } else if(pipe == pipes::south_west) {
        assert(bend_direction != Pipe::Direction::EAST);
        if(bend_direction == Pipe::Direction::NORTH) swap_parity = true;
        bend_direction = Pipe::Direction::EAST;
    }
    if (swap_parity) {
        parity = static_cast<Coloring>(-static_cast<char>(parity));
    }
}

ColoringMap init_coloring(Grid& grid) {
    ColoringMap coloring_map;
    const auto start_pipe = get<1>(part_1(grid));
    auto ittr = grid.begin(start_pipe.get_directions()[0]);
    while(ittr != grid.end()) {
        coloring_map[*ittr] = Coloring::ON;
        ++ittr;
    }
    grid.add_pipe(grid.get_start(), start_pipe);
    return coloring_map;
}

size_t part_2(Grid& grid) {
    auto coloring_map = init_coloring(grid);

    size_t total_in = 0;
    for(int y = 0; y<grid.get_height(); ++y) {
        auto parity = Coloring::OUT;
        auto bend_direction = Pipe::Direction::EAST;
        for(int x = 0; x<grid.get_width(); ++x) {
            Grid::Point point(x, y);
            if(coloring_map.contains(point)) {
                handle_pipe(grid.get_pipe(point).value(), bend_direction, parity);
            } else {
                coloring_map[point] = parity;
                if(parity == Coloring::IN) ++total_in;
            }
        }
    }
    return total_in;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("10", argc, argv);

    Grid grid = parse::parse_grid(lines);

    cout << "Part 1: " << get<0>(part_1(grid)) << endl;
    cout << "Part 2: " << endl << part_2(grid) << endl;
}
