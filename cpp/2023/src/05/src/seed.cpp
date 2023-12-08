//
// Day 3: Scratchcards
// https://adventofcode.com/2023/day/4
//
// Created by Conor Pappas on 12/4/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <ranges>
#include <algorithm>
#include <unordered_map>
#include <cassert>
#include <set>

using namespace std;

const string INPUT_DIR = "/Users/conorpappas/current/advent_of_code/cpp/2023/src/05/data/";

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
auto split(const string& str, char delim) {
    return str
        | views::split(delim)
        | views::transform([](auto&& rng) {
            return string_view(rng);
        });
}

// TODO: Split into multiple files

using Category = string;

struct Descriptor {
    Category category;
    long value;

    friend ostream& operator<<(ostream& os, const Descriptor& d) {
        return os << d.category << ":" << d.value;
    };
};

class DescriptorRangeTransform {
public:
    DescriptorRangeTransform(Descriptor input, Descriptor output, const long size):
        input_range_start(std::move(input)),
        output_range_start(std::move(output)),
        size(size)
    {};

    [[nodiscard]] long input_start() const {
        return input_range_start.value;
    }

    [[nodiscard]] long input_end() const {
        return input_range_start.value + size - 1;
    }

    [[nodiscard]] long output_start() const {
        return output_range_start.value;
    }

    [[nodiscard]] long output_end() const {
        return output_range_start.value + size;
    }

    [[nodiscard]] Category input_category() const {
        return input_range_start.category;
    }

    [[nodiscard]] Category output_category() const {
        return output_range_start.category;
    }

    friend ostream& operator<<(ostream& os, const DescriptorRangeTransform& d) {
        return os << d.input_category() << "(" << d.input_start() << "-" << d.input_end() << ") -> "
            << d.output_category() << "(" << d.output_start() << "-" << d.output_end() << ")";
    }

    Descriptor operator()(const Descriptor& input) const {
        if(!in_range(input)) {
            throw std::runtime_error("Input out of range");
        }
        return Descriptor{output_range_start.category, input.value - input_start() + output_start()};
    }

    [[nodiscard]] bool in_range(const Descriptor& input) const {
        assert(input.category == input.category);
        return input.value >= input_start() && input.value <= input_end();
    }

private:
    Descriptor input_range_start;
    Descriptor output_range_start;
    long size = 0;
};

class DescriptorRangeTransformIndex {
public:
    Category input_category;
    Category output_category;
    vector<DescriptorRangeTransform> transforms{};

    DescriptorRangeTransformIndex(Category input_category, Category output_category):
        input_category(std::move(input_category)),
        output_category(std::move(output_category))
    {}

    void add(const DescriptorRangeTransform& transform) {
        assert(transform.input_category() == input_category);
        assert(transform.output_category() == output_category);
        transforms.push_back(transform);
    }

    // TODO: actually use an interval tree
    [[nodiscard]] optional<DescriptorRangeTransform> find(const Descriptor& input) const {
        assert(input.category == input_category);
        auto in_range = [input](const DescriptorRangeTransform& transform) {
            return transform.in_range(input);
        };
        const auto result = ranges::find_if(transforms, in_range);
        return result == transforms.end() ? nullopt : make_optional(*result);
    }

    Descriptor operator()(const Descriptor& input) const {
        if (const auto transform = find(input)) {
            return transform.value()(input);
        } else {
            return {output_category, input.value};
        }
    }
};

const regex SEED_FORMAT = regex(R"(seeds: (\d+( \d+)*))");

vector<Descriptor> parseSeeds(string& line) {
    vector<Descriptor> descriptors;
    smatch match;
    if (regex_match(line, match, SEED_FORMAT)) {
        string seeds = match[1].str();
        for (const auto& seed :split(seeds, ' ')) {
            string str(seed.begin(), seed.end());
            descriptors.push_back(Descriptor{"seed", stol(string(seed.begin(), seed.end()))});
        }
    }
    return descriptors;
}

const regex DESCRIPTOR_TRANSFORM_HEADER_FORMAT = regex(R"((\w+)-to-(\w+) map:)");
const regex DESCRIPTOR_TRANSFORM_MAP_FORMAT = regex(R"((\d+) (\d+) (\d+))");


DescriptorRangeTransformIndex parseDescriptorMap(span<string> lines) {
    string header = lines[0];
    smatch headerMatch;
    if (!regex_search(header, headerMatch, DESCRIPTOR_TRANSFORM_HEADER_FORMAT)) {
        throw runtime_error("Invalid descriptor transform header");
    }
    const Category input_category = headerMatch[1];
    const Category output_category = headerMatch[2];

    DescriptorRangeTransformIndex transform_index(input_category, output_category);
    for(const string& line : lines.subspan(1)) {
        smatch mapMatch;
        if (regex_search(line, mapMatch, DESCRIPTOR_TRANSFORM_MAP_FORMAT)) {
            const long output_value = stol(mapMatch[1]);
            const long input_value = stol(mapMatch[2]);
            const long size = stol(mapMatch[3]);
            const Descriptor input = {input_category, input_value};
            const Descriptor output = {output_category, output_value};
            const DescriptorRangeTransform transform = {input, output, size};
            transform_index.add(transform);
        }
    }

    return transform_index;
}

using Almanac = unordered_map<Category, DescriptorRangeTransformIndex>;

Almanac parseAlmanac(const span<string>& lines) {
    Almanac almanac;

    vector<string> mapLines;
    for(const auto& curLine : lines) {
        smatch headerMatch;
        const bool header = regex_search(curLine, headerMatch, DESCRIPTOR_TRANSFORM_HEADER_FORMAT);
        if(header && !mapLines.empty()) {
            const auto transformIndex = parseDescriptorMap(mapLines);
            almanac.insert({transformIndex.input_category, transformIndex});
            mapLines.clear();
        }
        if(!curLine.empty()) {
            mapLines.push_back(curLine);
        }
    }
    const auto transformIndex = parseDescriptorMap(mapLines);
    almanac.insert({transformIndex.input_category, transformIndex});
    return almanac;
}

long part_1(const vector<Descriptor>& seeds, const Almanac& almanac) {
    set<long> seed_locations;

    for (auto const &seed : seeds) {
        Descriptor seed_result = seed;
        while(seed_result.category != "location") {
            seed_result = almanac.at(seed_result.category)(seed_result);
        }
        seed_locations.insert(seed_result.value);
    }

    return *seed_locations.begin();
}

int part_2() {
}

int main(int argc, char** argv) {
    const string filename = argc > 1 ? argv[1] : "input.txt";
    vector<string> lines = readFile(INPUT_DIR + filename);
    const span<string> lines_span = lines;

    const auto seeds = parseSeeds(lines_span[0]);
    const auto almanac = parseAlmanac(lines_span.subspan(1));

    cout << part_1(seeds, almanac) << endl;
}
