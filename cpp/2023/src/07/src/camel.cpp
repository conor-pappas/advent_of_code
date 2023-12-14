//
// Day 7: Camel Cards
// https://adventofcode.com/2023/day/7
//
// Created by Conor Pappas on 12/13/23.
//

#include <algorithm>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <regex>
#include <set>

#include "hand.hpp"

using namespace std;
using namespace camel;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/2023/src/07/data/";

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

const regex HAND_REGEX = regex(R"((\w{5})\s*(\d+))");
vector<Hand> parse_hands(const span<string> lines) {

    vector<Hand> hands;

    for (const string& line : lines) {
        smatch match;

        if (regex_match(line, match, HAND_REGEX)) {
            const string& hand_string = match[1];
            const string& bid_string = match[2];

            assert(hand_string.size() == Hand::HAND_SIZE);

            Hand hand;
            for (int i = 0; i <Hand::HAND_SIZE; i++) {
                hand.cards[i] = Card(hand_string[i]);
            }
            hand.bid = stol(bid_string);

            hands.push_back(hand);
        } else {
            throw runtime_error("Invalid hand");
        }
    }

    return hands;
}

Hand::bid_t part_1(vector<Hand>& hands) {
    ranges::sort(hands);

    Hand::bid_t total_bid = 0;
    for (int i = 0; i < hands.size(); i++) {
        total_bid += hands[i].bid * (i + 1);
    }

    return total_bid;
}

int part_2() {
    return 1;
}

int main(const int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    vector<string> lines = readFile(INPUT_DIR + filename);
    const span<string> lines_span = lines;

    vector<Hand> hands = parse_hands(lines_span);

    cout << "Part 1: " << part_1(hands) << endl;
    cout << "Part 2: " << part_2() << endl;
}
