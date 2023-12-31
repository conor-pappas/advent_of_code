//
// Day 4: Scratchcards
// https://adventofcode.com/2023/day/4
//
// Created by Conor Pappas on 12/4/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <set>
#include <ranges>
#include <algorithm>
#include <numeric>

#include "input.hpp"

using namespace std;

// TODO: Move to lib
auto split(const string& str, char delim) {
    return str
        | views::split(delim)
        | views::transform([](auto&& rng) { return string(rng.begin(), rng.end()); });
}

struct Scratchcard {
    int id = 0;
    set<int> winning_numbers;
    set<int> scratch_numbers;

    [[nodiscard]] set<int> getMatchedNumbers() const {
        set<int> matched_numbers;

        ranges::set_intersection(winning_numbers, scratch_numbers, inserter(matched_numbers, matched_numbers.begin()));

        for (const int& number : scratch_numbers) {
            if (winning_numbers.find(number) != winning_numbers.end()) {
                matched_numbers.insert(number);
            }
        }
        return matched_numbers;
    }

    [[nodiscard]] int getPoints() const {
        if (getMatchedNumbers().empty()) return 0;
        return static_cast<int>(pow(2, getMatchedNumbers().size() - 1));
    }
};

class ScatchcardProcessor {
public:
    vector<Scratchcard> scratchcards;
    vector<int> card_counts;

    void process() {
        card_counts.assign(scratchcards.size(), 1);

        for (size_t i= 0; i < scratchcards.size(); i++) {
            const Scratchcard& scratchcard = scratchcards[i];
            const size_t copy_count = static_cast<int>(scratchcard.getMatchedNumbers().size());
            for (size_t j = 1; j <= copy_count && i+j < card_counts.size(); j++) {
                card_counts[i+j] += card_counts[i];
            }
        }
    }

};

const regex CARD_FORMAT = regex(R"(Card\s*(\d+):\s*(\d+(?:\s+\d+)+)+\s*\|\s*(\d+(?:\s+\d+)+)+)");

Scratchcard parseScratchcard(const string& line) {
    Scratchcard scratchcard;

    auto ittr = sregex_iterator( line.begin(), line.end(), CARD_FORMAT);
    for (const auto end = sregex_iterator(); ittr != end; ++ittr) {
        const std::smatch& match = *ittr;
        scratchcard.id = stoi(match[1]);

        const string winning_numbers_str = match[2];
        const string scratch_numbers_str = match[3];

        for (const string& number_str : split(winning_numbers_str, ' ')) {
            try {
                scratchcard.winning_numbers.insert(stoi(number_str));
            } catch (const invalid_argument&) { }
        }
        for (const string& number_str : split(scratch_numbers_str, ' ')) {
            try {
                scratchcard.scratch_numbers.insert(stoi(number_str));
            } catch (const invalid_argument&) { }
        }
    }

    return scratchcard;
}

void printScratchCard(const Scratchcard& scratchcard) {
    cout << "Scratchcard " << scratchcard.id << endl;
    cout << "Winning Numbers: ";
    for (const int& number : scratchcard.winning_numbers) {
        cout << number << " ";
    }
    cout << endl;
    cout << "Scratch Numbers: ";
    for (const int& number : scratchcard.scratch_numbers) {
        cout << number << " ";
    }
    cout << endl << "Points: " << scratchcard.getPoints() << " ";
    cout << endl;
}

int part_1(const vector<Scratchcard>& scratchcards) {
    int points = 0;
    for (const Scratchcard& scratchcard : scratchcards) {
        points += scratchcard.getPoints();
    }
    return points;
}

int part_2(const vector<Scratchcard>& scratchcards) {
    ScatchcardProcessor processor;
    processor.scratchcards = scratchcards;
    processor.process();

    int card_count = 0;
    for (const int& count : processor.card_counts) {
        card_count += count;
    }
    return card_count;
}

int main(const int argc, const char** argv) {
    const vector<string> lines = support::read_input("04", argc, argv);

    vector<Scratchcard> scratchcards;
    for (const string& line : lines) {
        auto x = parseScratchcard(line);
        scratchcards.push_back(x);
        printScratchCard(x);
        cout << endl;
    }

    cout << part_2(scratchcards) << endl;
}
