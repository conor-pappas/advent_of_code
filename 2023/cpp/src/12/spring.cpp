// Day 12: Hot Springs
// https://adventofcode.com/2023/day/12
//
// Created by Conor Pappas on 12/22/23.
//

#include <iostream>

#include "input.hpp"
#include "parse.hpp"
#include "arrangement_finder.hpp"

using namespace std;
using namespace spring;

size_t part_1(const vector<parse::Row>& rows) {
    size_t result = 0;
    for (auto& [record, pattern] : rows) {
        const auto arrangements = ArrangementFinder::find(record, pattern);
        result += arrangements.size();;
    }
    return result;
}

unsigned int part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("12", argc, argv);

    const auto rows = parse::parse_rows(lines);

    cout << "Part 1: " << part_1(rows) << endl;
    cout << "Part 2: " << part_2() << endl;
}
