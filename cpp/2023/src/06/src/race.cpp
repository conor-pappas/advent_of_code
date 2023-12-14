//
// Day 6: Wait for it
// https://adventofcode.com/2023/day/6
//
// Created by Conor Pappas on 12/4/23.
//

#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <regex>

using namespace std;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/2023/src/06/data/";

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

using time_t = long;
using distance_t = long;
using speed_t = long;

class Record {
public:
    time_t time { 0 };
    distance_t distance { 0 };

    friend ostream& operator<< (ostream& os, const Record& record) {
        return os << "Record { time: " << record.time << ", distance: " << record.distance << " }";
    }
};

class BoatState {
public:
    time_t time { 0 };
    distance_t position { 0 };
    speed_t speed { 0 };

    [[nodiscard]] static BoatState hold_button(const time_t time) {
        return { time, 0, static_cast<speed_t>(time) };
    }
};

// TODO: not really needed - we are going to just use a fixed formula
void update(BoatState state, const time_t time = 1) {
    state.time += time;
    state.position += state.speed * time;
}

const regex TIMES_REGEX = regex(R"(Time:\s*(\d+(\s+\d+)*))");
const regex DISTANCES_REGEX = regex(R"(Distance:\s*(\d+(\s+\d+)*))");
vector<Record> parse_records(const span<string> lines) {
    assert(lines.size() == 2);

    vector<time_t> times;
    vector<distance_t> distances;

    smatch times_match, distances_match;
    if (!regex_match(lines[0], times_match, TIMES_REGEX)) { throw runtime_error("Invalid times"); }
    if (!regex_match(lines[1], distances_match, DISTANCES_REGEX)) { throw runtime_error("Invalid distances"); }

    for (const auto& time : split(times_match[1].str(), ' ')) {
        times.push_back(stol(time));
    }
    for (const auto& distance : split(distances_match[1].str(), ' ')) {
        distances.push_back(stol(distance));
    }

    assert(times.size() == distances.size());

    vector<Record> records;
    records.reserve(times.size());
    for (size_t i = 0; i < times.size(); i++) {
        records.push_back({ times[i], distances[i] });
    }
    return records;
}

distance_t distance_for_button(const time_t& hold_time, const time_t& race_duration) {
    return (race_duration - hold_time) * static_cast<speed_t>(hold_time);
}

long part_1(const vector<Record>& records) {
    long result = 1;
    for (const auto& [time, distance] : records) {
        long num_records = 0;
        for (long i = 1; i < time; i++) {
            if(distance_for_button(i, time) > distance) {
                num_records++;
            }
        }
        result *= num_records;
    }

    return result;
}

long part_2() {
    return 0;
}

int main(const int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    vector<string> lines = readFile(INPUT_DIR + filename);
    const span<string> lines_span = lines;

    const vector<Record> records = parse_records(lines_span);

    for (const auto& record : records) {
        cout << record << endl;
    }
    cout << "Part 1: " << part_1(records) << endl;
    cout << "Part 2: " << part_2() << endl;
}
