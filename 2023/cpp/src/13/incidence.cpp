// Day 13: Point of Incidence
// https://adventofcode.com/2023/day/13
//
// Created by Conor Pappas on 12/23/23.
//

#include <iostream>
#include <optional>
#include <utility>

#include "input.hpp"
#include "parse/pattern.hpp"
#include "grid.hpp"

#include "data_types/pattern.hpp"
#include "iterator/symmetric.hpp"

using namespace std;
using namespace incidence;
using namespace incidence::iterator;

constexpr char VERTICAL = 1;

using Row = support::Grid<data_types::Entity,1>;

size_t row_difference_count(const Row& row1, const Row& row2) {
    size_t count = 0;
    for(size_t i = 0; i < row1.size(); ++i) {
        if(row1[i] != row2[i]) ++count;
    }
    return count;
}

optional<long> find_symmetry_line(data_types::Pattern pattern, const size_t axis, size_t tolerance) {
    if (axis == VERTICAL) pattern = pattern.transpose();
    long symmetry_line = 1;
    for (auto itr = pattern.begin(); itr + 1 != pattern.end(); ++itr, ++symmetry_line) {
        const value_compare<support::Grid<data_types::Entity,2>,size_t> x(&row_difference_count);
        if(even_symmetric_difference(itr,itr+1, pattern, x) == tolerance) {
            return symmetry_line;
        }
    }
    return {};
}

optional<long> find_symmetry_line(data_types::Pattern pattern, const size_t axis) {
    return find_symmetry_line(std::move(pattern), axis, 0);
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

size_t part_2(const vector<data_types::Pattern>& patterns) {
    long total = 0;
    for(const data_types::Pattern& pattern : patterns) {
        auto horizontal_line_index = find_symmetry_line(pattern, 0, 1);
        auto vertical_line_index = find_symmetry_line(pattern, 1, 1);
        total += horizontal_line_index.value_or(0) * 100;
        total += vertical_line_index.value_or(0);
    }
    return total;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("13", argc, argv);

    const auto patterns = parse::parse_patterns(lines);

    cout << "Part 1: " << part_1(patterns) << endl;
    cout << "Part 2: " << part_2(patterns) << endl;
}
