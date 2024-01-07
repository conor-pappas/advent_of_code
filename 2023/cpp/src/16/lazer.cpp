// Day 16: The Floor Will Be Lava
// https://adventofcode.com/2023/day/16
//
// Created by Conor Pappas on 1/6/24.
//

#include <iostream>
#include <deque>

#include <input.hpp>
#include <grid.hpp>

#include "data_types/mirror.hpp"
#include "data_types/emission.hpp"

using namespace std;
using namespace lazer;
using namespace lazer::data_types;

using Location=support::Point<size_t, 2>;
using MirrorGrid = support::Grid<Mirror>;
using EmissionGrid = support::Grid<Emission>;

EmissionGrid simulate_emission(const MirrorGrid& mirror_grid, Location starting_location, Direction starting_direction) {
    EmissionGrid emission_grid;
    emission_grid.resize(mirror_grid.width(), mirror_grid.height());
    std::deque<tuple<Location,Direction>> lazer_stack;
    lazer_stack.emplace_back(starting_location, starting_direction);
    while(!lazer_stack.empty()) {
        const auto [location, direction] = lazer_stack.back();
        lazer_stack.pop_back();
        if(!mirror_grid.contains(location)) continue;
        const auto new_directions = propogate_lazer(mirror_grid.get(location), direction);
        for(const auto& new_direction : new_directions) {
            auto& emission = emission_grid.get(location);
            if(emission[new_direction]) continue;
            emission[new_direction] = true;
            lazer_stack.emplace_back(location+new_direction, new_direction);
        }
    }
    return emission_grid;
}

size_t count_emitting(const EmissionGrid& emission_grid) {
    size_t result = 0;
    for(const auto& row : emission_grid) {
        for(const auto& emission : row) {
            if(emission.emitting()) result++;
        }
    }
    return result;
}

size_t part_1(const MirrorGrid& mirror_grid) {
    return count_emitting(simulate_emission(mirror_grid, {0,0}, EAST));
}

size_t part_2(const MirrorGrid& mirror_grid) {
    vector<tuple<Location,Direction>> starting_locations;
    for(size_t x=0; x<mirror_grid.width(); x++) {
        starting_locations.emplace_back(Location{x,0}, SOUTH);
        starting_locations.emplace_back(Location{x,mirror_grid.height()-1}, NORTH);
    }
    for(size_t y=0; y<mirror_grid.height(); y++) {
        starting_locations.emplace_back(Location{0,y}, EAST);
        starting_locations.emplace_back(Location{mirror_grid.width()-1,y}, WEST);
    }
    size_t largest_emitting = 0;
    for(const auto& [location, direction] : starting_locations) {
        const auto emission_grid = simulate_emission(mirror_grid, location, direction);
        const auto emitting_count = count_emitting(emission_grid);
        if(emitting_count > largest_emitting) largest_emitting = emitting_count;
    }
    return largest_emitting;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("16", argc, argv);

    const auto grid = support::parse_grid<Mirror>(lines);

    cout << "Part 1: " << part_1(grid) << endl;
    cout << "Part 2: " << part_2(grid) << endl;
}
