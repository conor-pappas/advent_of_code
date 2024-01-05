// Day 12: Point of Incidence
// https://adventofcode.com/2023/day/13
//
// Created by Conor Pappas on 12/23/23.
//

#include <iostream>
#include <optional>

#include "input.hpp"
#include "parse/pattern.hpp"
#include "grid.hpp"

#include "data_types/pattern.hpp"
#include "iterator/symmetric.hpp"

using namespace std;
using namespace incidence;

constexpr char VERTICAL = 1;

optional<long> find_symmetry_line(data_types::Pattern pattern, const size_t axis) {
    if (axis == VERTICAL) pattern = pattern.transpose();
    long symmetry_line = 1;
    for (auto itr = pattern.begin(); itr + 1 != pattern.end(); ++itr, ++symmetry_line) {
        if(incidence::iterator::is_even_symmetric(itr, itr+1, pattern)) {
            return symmetry_line;
        }
    }
    return {};
}

long part_1(const vector<data_types::Pattern>& patterns) {
    long total = 0;
    for(const data_types::Pattern& pattern : patterns) {
        auto horizontal_line_index = find_symmetry_line(pattern, 0);
        auto vertical_line_index = find_symmetry_line(pattern, 1);
        total += horizontal_line_index.value_or(0) * 100;
        total += vertical_line_index.value_or(0);
    }
    return total;
}

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("13", argc, argv);

    const auto patterns = parse::parse_patterns(lines);

    cout << "Part 1: " << part_1(patterns) << endl;
    cout << "Part 2: " << part_2() << endl;
}
