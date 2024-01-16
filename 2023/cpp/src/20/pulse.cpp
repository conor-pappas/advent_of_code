// Day 20: Pulse Propagation
// https://adventofcode.com/2023/day/20
//
// Created by Conor Pappas on 1/15/24.
//

#include <iostream>
#include <vector>

#include <input.hpp>

#include "parse.hpp"

using namespace std;
using namespace pulse;

size_t part_1(data_types::Network& network) {
    data_types::PulseCount result;
    for(size_t i = 0; i < 1000; ++i) {
        result = result + press_button(network);
    }
    return result.high * result.low;
}

size_t part_2() {
    return 0;
}


int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("20", argc, argv);

    data_types::Network network = parse::parse_network(lines);

    cout << "Part 1: " << part_1(network) << endl;
    cout << "Part 2: " << part_2() << endl;
}


