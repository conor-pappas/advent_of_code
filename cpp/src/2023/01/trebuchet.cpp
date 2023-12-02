//
// Day 1: Trebuchet?!
// https://adventofcode.com/2023/day/1
//
// Created by Conor Pappas on 12/1/23.

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/2023/cpp/input/2023/01/";

#define REPLACE_TEXT true

template<typename VALUE>
map<string, VALUE> reverse_string_map_keys(map<string, VALUE> input_map);

const map<string, string> digit_text = {
    {"one", "1"},
    {"two", "2"},
    {"three", "3"},
    {"four", "4"},
    {"five", "5"},
    {"six", "6"},
    {"seven", "7"},
    {"eight", "8"},
    {"nine", "9"}
};
const map<string, string> reverse_digit_text = reverse_string_map_keys(digit_text);

// TODO: Move to lib
template<typename VALUE>
map<string, VALUE> reverse_string_map_keys(map<string, VALUE> input_map) {
    auto result = map<string, VALUE>();
    for(auto const& [key, value] : input_map) {
        auto new_key = key;
        reverse(new_key.begin(), new_key.end());
        result[new_key] = value;
    }
    return result;
}

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

// TODO: Move to lib
bool isDigit(char c) {
    return '0' <= c && c <= '9';
}

tuple<char,int> findFirstDigit(const string& line) {
    auto digit = find_if(line.begin(), line.end(), isDigit);
    return { *digit, digit - line.begin() };
}

tuple<char,int> findLastDigit(const string& line) {
    auto digit = find_if(line.rbegin(), line.rend(), isDigit);
    return { *digit, line.length() - (digit - line.rbegin()) - 1 };
}

void replace_first_digit_string(string* line, const map<string, string>& digit_text) {
    map<string, size_t> digit_positions;
    for (auto const& [text, digit] : digit_text) {
        digit_positions[text] = line->find(text);
    }

    auto position_compare = [](const auto& a, const auto& b) {
        return a.second < b.second;
    };

    auto [text, position] = *min_element(digit_positions.begin(), digit_positions.end(), position_compare);

    if (position != string::npos) {
        line->replace(position, text.length(), digit_text.at(text));
    }
}

int calculateLine(const string& line, bool replace_digits = true) {
    auto [first, first_position] = findFirstDigit(line);
    auto [last, last_position] =  findLastDigit(line);

    if(replace_digits) {
        auto left = string(line, 0, first_position + 1);
        auto right = string(line.rbegin(), line.rend() - last_position);

        replace_first_digit_string(&left, digit_text);
        replace_first_digit_string(&right, reverse_digit_text);
        first = get<0>(findFirstDigit(left));
        last = get<0>(findFirstDigit(right));
    };

    if (first && last) {
        return stoi(string(1,first) + string(1,last));
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    const vector<string> lines = readFile(INPUT_DIR + filename);
    int result = 0;
    for(const auto& line : lines) {
        result += calculateLine(line, REPLACE_TEXT);
    }
    cout << result;
}