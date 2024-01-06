// Day 14: Parabolic Reflector Dish
// https://adventofcode.com/2023/day/14
//
// Created by Conor Pappas on 1/4/24.
//


#include <iostream>
#include <unordered_map>

#include <point.hpp>
#include <input.hpp>
#include <grid.hpp>
#include <cycles/cycle_finder.hpp>
#include <utility>

#include "parse/platform.hpp"
#include "data_types/direction.hpp"
#include "data_types/roll_iterator.hpp"

using namespace std;
using namespace dish;
using namespace dish::data_types;

using Point = support::Point<size_t, 2>;
using PointMap = unordered_map<Point, size_t, Point::Hasher>;

void print_platform(const data_types::Platform& platform) {
    for(const auto& row : platform.entities()) {
        for(const auto& entity : row) {
            cout << static_cast<char>(entity);
        }
        cout << endl;
    }
    cout << endl;
}

size_t calculate_load(Platform platform) {
    size_t load = 0;
    const size_t height = platform.height();
    for(size_t y = 0; y < height; ++y) {
        for(size_t x = 0; x < platform.width(); ++x) {
            if(platform.entities()[y][x] == Entity::ROUND) {
                load += height - y;
            }
        }
    }
    return load;
}

size_t part_1(Platform platform) {
    platform.roll(Direction::NORTH);
    return calculate_load(platform);
}

constexpr long TOTAL_ROLLS = 4'000'000'000;

size_t part_2(const Platform& platform) {
    constexpr std::array<Direction, 4> dirs = {
        Direction::NORTH,
        Direction::WEST,
        Direction::SOUTH,
        Direction::EAST
    };
    RollIterator itr(platform, dirs.begin(), dirs.end());
    const auto cycle = support::CycleFinder<decltype(itr)>::find(itr);
    if(!cycle) throw runtime_error("Could not find cycle");

    const long advance_count = (TOTAL_ROLLS - cycle-> tail_length) % cycle->cycle_length;
    auto result = cycle->loop.begin();
    std::advance(result, advance_count);
    return calculate_load(*result);
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("14", argc, argv);

    const auto platform = parse::parse_platform(lines);

    cout << "Part 1: " << part_1(platform) << endl;
    cout << "Part 2: " << part_2(platform) << endl;
}
