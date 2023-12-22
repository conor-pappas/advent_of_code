// Day 11: Cosmic Expansion
// https://adventofcode.com/2023/day/11
//
// Created by Conor Pappas on 12/22/23.
//

#include <iostream>

#include "input.hpp"
#include "parse.hpp"

using namespace std;
using namespace cosmic;

Space::coord_type calculate_distances(const Space& space) {
    // I just want C++23 :(
    // auto pairs = std::cartesian_product(space, space);

    Space::coord_type distance_sum = 0;
    for(const Space::Galaxy& galaxy_a: space) {
        for(const Space::Galaxy& galaxy_b: space) {
            const auto difference = galaxy_a - galaxy_b;
            distance_sum += abs(difference.x());
            distance_sum += abs(difference.y());
        }
    }

    // We double counted pairs.
    return distance_sum / 2;
}

Space::coord_type part_1(Space space) {
    space.expand();
    return calculate_distances(space);
}

unsigned long part_2(Space space) {
    space.expand(999'999);
    return calculate_distances(space);
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("11", argc, argv);

    const Space space = parse::parse_space(lines);

    cout << "Part 1: " << part_1(space) << endl;
    cout << "Part 2: " << part_2(space) << endl;
}
