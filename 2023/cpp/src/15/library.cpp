// Day 15: Lens Library
// https://adventofcode.com/2023/day/15
//
// Created by Conor Pappas on 1/6/24.
//


#include <cassert>
#include <iostream>
#include <unordered_map>

#include <input.hpp>
#include <string.hpp>

#include "parse/step.hpp"
#include "data_types/hasher.hpp"
#include "data_types/step.hpp"

using namespace std;
using namespace library;
using namespace library::data_types;

unsigned long part_1(const vector<Step>& steps) {
    long result = 0;
    for(const auto& [label, op, lense] : steps) {
        auto str = label + static_cast<char>(op);
        if(op == SET) { str += to_string(lense); }
        result += Hasher()(str);
    }
    return result;
}

unsigned long calculate_focusing_power(const size_t box, const size_t slot, const Lense lense) {
    return (box + 1) * (slot) * lense;
}

unsigned long part_2(const vector<Step>& steps) {
    unsigned long result = 0;
    unordered_map<Label, Lense, Hasher> map;
    map.rehash(256);
    map.max_load_factor(steps.size() / 256.0);
    assert(map.bucket_count() == 256);
    for(const auto& [label, op, lense] : steps) {
        if(op == SET) map[label] = lense;
        else map.erase(label);
        assert(map.bucket_count() == 256);
    }
    for(size_t box = 0; box < 256; ++box) {
        size_t slot = 0;
        const auto box_size = std::distance(map.begin(box), map.end(box));
        for(auto itr = map.begin(box); itr != map.end(box); ++itr,++slot) {
            result += calculate_focusing_power(box, box_size - slot, itr->second);
        }
    }
    return result;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("15", argc, argv);

    assert(lines.size() == 1);
    const auto steps = parse::parse_steps(lines.front());

    cout << "Part 1: " << part_1(steps) << endl;
    cout << "Part 2: " << part_2(steps) << endl;
}
