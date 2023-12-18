// Day 8: Haunted Wasteland
// https://adventofcode.com/2023/day/8
//
// Created by Conor Pappas on 12/13/23.
//

#include <cassert>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <regex>
#include <__numeric/gcd_lcm.h>

#include "network.hpp"
#include "instruction_set.hpp"
#include "input.hpp"
#include "cycles/cycle_finder.hpp"


using namespace std;
using namespace wasteland;

// FIXME: why are views so awful?
vector<string> split(const string& str, const char& delim) {
    vector<string> strings;
    string current_string;
    auto push = [&] {
        if(!current_string.empty()) { strings.push_back(current_string); }
        current_string.clear();
    };
    for (const auto& c : str) {
        if (c == delim) { push(); }
        else {  current_string.push_back(c); }
    }
    push();
    return strings;
}

const regex INSTRUCTION_REGEX = regex(R"(([LR]*))");
const regex NODE_REGEX = regex(R"(([\w]{3})\s*=\s*\(([\w]{3})\,\s*([\w]{3})\))");

InstructionSet parse_instructions(const string& line) {
    smatch match;
    if (regex_match(line, match, INSTRUCTION_REGEX)) {
        const string instructions_string = match[1].str();
        const vector<const Instruction> instructions(instructions_string.begin(), instructions_string.end());
        return InstructionSet(instructions);
    }
    throw runtime_error("Could not parse instructions");
}

Network parse_network(const span<string>& lines) {
    Network network;
    for(const auto& line: lines) {
        smatch match;
        if (regex_match(line, match, NODE_REGEX)) {
            const string label = match[1].str();
            const string left = match[2].str();
            const string right = match[3].str();
            network.add_node(label, left, right);
        }
    }
    return network;
}

const Network::Label START_LABEL = "AAA";
const Network::Label END_LABEL = "ZZZ";

size_t part_1(Network& network, const InstructionSet& instructions) {
    size_t distance = 0;
    auto iterator = network.traverse(START_LABEL, instructions);
    while(iterator->label != END_LABEL) {
        ++iterator;
        ++distance;
    }
    return distance;
}

constexpr char START_SUFFIX = 'A';
constexpr char END_SUFFIX = 'Z';

using cycle_finder = support::CycleFinder<Network::TraversalIterator>;
using Cycle = cycle_finder::Cycle;

size_t lcm(const vector<size_t>& numbers) {
    size_t lcm = 1;
    for (const auto& number: numbers) {
        lcm = std::lcm(lcm, number);
    }
    return lcm;
}

size_t part_2(Network& network, const InstructionSet& instructions) {
    // TODO: Can't be const b/c we don't have const CyclicIterator
     auto cycles = network.get_nodes()
        | std::views::filter([](const auto& node) { return node.first.back() == START_SUFFIX; })
        | std::views::transform([&](const auto& node) {
            auto cycle = cycle_finder::find(network.traverse(node.second, instructions));
            assert(cycle.has_value());
            return cycle.value();
        });

    // TODO: would be much easier if clang had support for ranges::enumerate. I could just impl myself I guess.
    // TODO: We are making assumptions about our data here which I'd love to come back and fix:
    // 1) That there is only one end_node per cycle. We should be running calculations for each combination of end nodes.
    // 2) That the tail, when overlayed on the loop, has the start_node right after the end_node.
    //    The data happens to fit this (likely intentionally), but the full solution for this should involve a CRT calculation.
    vector<size_t> cycle_lengths;
    for (const auto& cycle: cycles) {
        int end_node_count = 0;
        int index = 0;
        int end_index = 0;
        for (const auto& node: cycle.loop) {
            if (node.label.back() == END_SUFFIX) {
                ++end_node_count;
                end_index = index;
            }
            ++index;
        }
        assert(end_node_count == 1);
        const size_t total_offset = (end_index + cycle.tail_length) % cycle.cycle_length;
        assert(total_offset == 0);

        cycle_lengths.push_back(cycle.cycle_length);
    }

    return lcm(cycle_lengths);
}

int main(const int argc, const char** argv) {
    vector<string> lines = support::read_input("08", argc, argv);
    const span lines_span = lines;

    const InstructionSet instructions = parse_instructions(lines_span[0]);
    Network network = parse_network(lines_span.subspan(1));


    cout << "Part 1: " << part_1(network, instructions) << endl;
    cout << "Part 2: " << endl << part_2(network, instructions) << endl;
}
