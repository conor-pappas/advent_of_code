// Day 20: Pulse Propagation
// https://adventofcode.com/2023/day/20
//
// Created by Conor Pappas on 1/15/24.
//

#include <iostream>
#include <vector>

#include <input.hpp>
#include <data_types/conjunction_module.hpp>
#include <__numeric/gcd_lcm.h>

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

unsigned long long part_2(data_types::Network& network) {
    size_t nand_count = 0;
    for(const auto& [label, module] : network) {
        if(module->is_nand()) ++nand_count;
    }

    size_t button_count = 0;
    bool done = false;
    std::unordered_map<data_types::label, size_t> nand_cycles;
    const auto visitor = [&](const data_types::Signal& signal) {
        if(network.contains(signal.origin) && network[signal.origin]->is_nand()) {
            if(signal.pulse == data_types::Pulse::High && !nand_cycles.contains(signal.origin)) {
                nand_cycles[signal.origin] = button_count;
                if(nand_cycles.size() == nand_count) done = true;
            }
        }
    };

    while(!done) {
        ++button_count;
        press_button(network, visitor);
    }
    size_t result = 1;
    for(const auto& [label, cycle] : nand_cycles) {
        result = std::lcm(result, cycle);
    }
    return result;
}


int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("20", argc, argv);

    data_types::Network network1 = parse::parse_network(lines);
    data_types::Network network2 = parse::parse_network(lines);

    cout << "Part 1: " << part_1(network1) << endl;
    cout << "Part 2: " << part_2(network2) << endl;
}


