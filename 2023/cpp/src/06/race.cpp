//
// Day 6: Wait for it
// https://adventofcode.com/2023/day/6
//
// Created by Conor Pappas on 12/13/23.
//

#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <regex>

#include "input.hpp"
#include "string.hpp"

using namespace std;

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

const regex TIMES_REGEX = regex(R"(Time:\s*(\d+(\s+\d+)*))");
const regex DISTANCES_REGEX = regex(R"(Distance:\s*(\d+(\s+\d+)*))");
vector<Record> parse_records(const span<string> lines) {
    assert(lines.size() == 2);

    vector<time_t> times;
    vector<distance_t> distances;

    smatch times_match, distances_match;
    if (!regex_match(lines[0], times_match, TIMES_REGEX)) { throw runtime_error("Invalid times"); }
    if (!regex_match(lines[1], distances_match, DISTANCES_REGEX)) { throw runtime_error("Invalid distances"); }

    for (const auto& time : support::split(times_match[1].str(), ' ')) {
        times.push_back(stol(time));
    }
    for (const auto& distance : support::split(distances_match[1].str(), ' ')) {
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

Record parse_record(const span<string>& lines) {
    assert(lines.size() == 2);

    vector<time_t> times;
    vector<distance_t> distances;

    smatch times_match, distances_match;
    if (!regex_match(lines[0], times_match, TIMES_REGEX)) { throw runtime_error("Invalid times"); }
    if (!regex_match(lines[1], distances_match, DISTANCES_REGEX)) { throw runtime_error("Invalid distances"); }

    string time_str, distance_str;
    for (const auto& time : support::split(times_match[1].str(), ' ')) {
        time_str += time;
    }
    for (const auto& distance : support::split(distances_match[1].str(), ' ')) {
        distance_str += distance;
    }

    return { stol(time_str), stol(distance_str) };
}

distance_t distance_for_button(const time_t& hold_time, const time_t& race_duration) {
    return (race_duration - hold_time) * static_cast<speed_t>(hold_time);
}

time_t num_records_brute(const Record& record) {
    time_t num_records = 0;
    for (time_t i = 1; i < record.time; i++) {
        if(distance_for_button(i, record.time) > record.distance) {
            num_records++;
        }
    }

    return num_records;
}

time_t num_records_algebraic(const Record& record) {
    const double discriminant = sqrt((record.time * record.time) - (4 * (record.distance + 1)));
    const time_t max = floor((static_cast<double>(record.time) + discriminant) / 2);
    const time_t min = ceil((static_cast<double>(record.time) - discriminant) / 2);

    return max - min + 1;
}

time_t num_records(const Record& record) {
   return num_records_algebraic(record);
}

time_t part_1(const vector<Record>& records) {
    time_t result = 1;
    for (const Record& record : records) {
        result *= num_records(record);
    }

    return result;
}

time_t part_2(const Record& record) {
    return num_records((record));
}

int main(const int argc, const char** argv) {
    vector<string> lines = support::read_input("06", argc, argv);
    const span<string> lines_span = lines;

    const vector<Record> records = parse_records(lines_span);
    const Record record = parse_record(lines_span);

    cout << "Part 1: " << part_1(records) << endl;
    cout << "Part 2: " << part_2(record) << endl;
}
