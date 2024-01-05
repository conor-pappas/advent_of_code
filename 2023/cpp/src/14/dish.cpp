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

#include "parse/platform.hpp"

using namespace std;
using namespace dish;

using Point = support::Point<size_t, 2>;
using PointMap = unordered_map<Point, size_t, Point::Hasher>;

Point resting_location(Point start, const data_types::Platform& platform) {
    while(start.y() >= 1 && platform[start.y()-1][start.x()] != data_types::Entity::CUBE) {
        --start.y();
    }
    return start;
}

PointMap calculate_resting_locations(const data_types::Platform& platform) {
    PointMap resting_locaitons;
    for(size_t y = 0; y < platform.size(); ++y) {
        for(size_t x = 0; x < platform[y].size(); ++x) {
            if(platform[y][x] == data_types::Entity::ROUND) {
                ++resting_locaitons[resting_location({x, y}, platform)];
            }
        }
    }
    return resting_locaitons;
}

size_t calculate_load(const data_types::Platform& platform) {
    auto resting_locations = calculate_resting_locations(platform);
    size_t load = 0;
    const size_t height = platform.size();
    for(const auto& [location, count] : resting_locations) {
        const size_t contributed_load = count*(height - location.y()) - count*(count-1) / 2;
        load += contributed_load;
    }
    return load;
}

size_t part_1(const data_types::Platform& platform) {
    return calculate_load(platform);
}

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("14", argc, argv);

    const auto platform = parse::parse_platform(lines);

    cout << "Part 1: " << part_1(platform) << endl;
    cout << "Part 2: " << part_2() << endl;
}
