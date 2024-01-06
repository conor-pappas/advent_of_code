// Day 14: Lens Library
// https://adventofcode.com/2023/day/15
//
// Created by Conor Pappas on 1/6/24.
//


#include <cassert>
#include <iostream>

#include <input.hpp>
#include <string.hpp>

#include "hash.hpp"

using namespace std;
using namespace library;

long part_1(const vector<string>& steps) {
    long result = 0;
    for(const auto& step : steps) {
        result += library::hash(step);
    }
    return result;
}

long part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("15", argc, argv);

    assert(lines.size() == 1);
    const auto steps = support::split(lines.front(), ',');

    cout << "Part 1: " << part_1(steps) << endl;
    cout << "Part 2: " << part_2() << endl;
}
