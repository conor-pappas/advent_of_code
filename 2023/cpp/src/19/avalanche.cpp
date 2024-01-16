// Day 19: Aplenty
// https://adventofcode.com/2023/day/19
//
// Created by Conor Pappas on 1/15/24.
//

#include <iostream>
#include <vector>

#include <input.hpp>

#include "parse.hpp"
#include "data_types/plan.hpp"

using namespace std;
using namespace avalanche;

size_t part_1(const data_types::Plan& plan, const parse::PartCollection& parts) {
    size_t result = 0;
    for(const auto& part : parts) {
        if(test(plan, part)) {
            result += part.x + part.m + part.a + part.s;
        }
    }
    return result;
}

size_t part_2(const data_types::Plan& plan) {
    constexpr data_types::RatingRange start_range {1, 4000};
    constexpr data_types::PartRange start { start_range, start_range, start_range, start_range };

    const auto test_results = test(plan, start);
    data_types::rating result = 0;
    for(const auto& [x, m, a, s] : test_results.accept) {
        result +=  size(x) * size(m) * size(a) * size(s);
    }
    return result;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("19", argc, argv);

    const auto [plan, parts] = parse::parse_input(lines);

    cout << "Part 1: " << part_1(plan, parts) << endl;
    cout << "Part 2: " << part_2(plan) << endl;
}


