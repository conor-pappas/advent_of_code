// Day 12: Hot Springs
// https://adventofcode.com/2023/day/12
//
// Created by Conor Pappas on 12/22/23.
//

#include <iostream>

#include "input.hpp"
#include "parse.hpp"
#include "arrangement_finder/finder.hpp"

using namespace std;
using namespace spring;

size_t count_arrangements(const vector<parse::Row>& rows) {
    size_t result = 0;
    for (auto& [record, pattern] : rows) {
        result += arrangement_finder::Finder{}.find_count(record, pattern);;
    }
    return result;
}

size_t part_1(const vector<parse::Row>& rows) {
    return count_arrangements(rows);
}

size_t part_2(vector<parse::Row> rows) {
    for (auto& row: rows) {
        row.record.expand(5, {data_types::Condition::UNKNOWN});
        row.pattern.expand(5);
    }
    return count_arrangements(rows);
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("12", argc, argv);

    const auto rows = parse::parse_rows(lines);

    cout << "Part 1: " << part_1(rows) << endl;
    cout << "Part 2: " << part_2(rows) << endl;
}
