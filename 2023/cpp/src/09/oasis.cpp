// Day 9: Mirage Maintenance
// https://adventofcode.com/2023/day/9
//
// Created by Conor Pappas on 12/18/23.
//

#include <iostream>
#include <ranges>

#include "parse.hpp"
#include "input.hpp"
#include "interpolation/forward_difference_table.hpp"

using namespace std;
using namespace oasis;

long part_1(const vector<History>& histories) {
    long result = 0;
    for (const auto& history : histories) {
        const auto difference_table = ForwardDifferenceTable<History::value_type>(history.get_values());
        const auto polynomial = difference_table.getPolynomial();
        const auto next_result = polynomial.calculate(static_cast<long>(history.size()));
        result += next_result;
    }
    return result;
}

long part_2(const vector<History>& histories) {
    long result = 0;
    for (const auto& history : histories) {
        const auto difference_table = ForwardDifferenceTable<History::value_type>(history.get_values());
        const auto polynomial = difference_table.getPolynomial();
        const auto next_result = polynomial.calculate(-1);
        result += next_result;
    }
    return result;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("09", argc, argv);

    const auto histories = parse::parse_input(lines);

    cout << "Part 1: " << part_1(histories) << endl;
    cout << "Part 2: " << part_2(histories) << endl;
}
