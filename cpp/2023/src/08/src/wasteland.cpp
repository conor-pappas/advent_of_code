// Day 8: Haunted Wasteland
// https://adventofcode.com/2023/day/8
//
// Created by Conor Pappas on 12/13/23.
//

#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <regex>

#include "network.hpp"
#include "instruction_set.hpp"

using namespace std;
using namespace wasteland;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/2023/src/08/data/";

// TODO: Move to lib
vector<string> readFile(const string& fileName) {
    ifstream file(fileName);

    if(!file) throw std::runtime_error("Could not open file");

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// FIXME: why are views so awful?
vector<string> split(const string& str, const char& delim) {
    vector<string> strings;
    string current_string;
    auto push = [&]() {
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
        const vector<Instruction> instructions(instructions_string.begin(), instructions_string.end());
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

size_t part_1(Network& network, InstructionSet& instructions) {
    size_t distance = 0;
    auto iterator = network.traverse(START_LABEL, instructions);
    while(iterator->label != END_LABEL) {
        ++iterator;
        ++distance;
    }
    return distance;
}

int part_2() {
    return 0;
}

int main(const int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    vector<string> lines = readFile(INPUT_DIR + filename);
    const span lines_span = lines;

    InstructionSet instructions = parse_instructions(lines_span[0]);
    Network network = parse_network(lines_span.subspan(1));

    const Network::Label start_label = "AAA";

    cout << "Part 1: " << part_1(network, instructions) << endl;
    cout << "Part 2: " << part_2() << endl;
}
