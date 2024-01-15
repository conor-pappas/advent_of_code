// Day 19: Aplenty
// https://adventofcode.com/2023/day/19
//
// Created by Conor Pappas on 1/15/24.
//

#include <iostream>
#include <vector>

#include <input.hpp>

#include "data_types/rule.hpp"
#include "parse.hpp"

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

size_t part_2() {
    return 0;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("19", argc, argv);

    const auto [plan, parts] = parse::parse_input(lines);

    cout << "Part 1: " << part_1(plan, parts) << endl;
    cout << "Part 2: " << part_2() << endl;
}


