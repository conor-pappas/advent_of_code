//
// Day 5: If You Give A Seed A Fertilizer
// https://adventofcode.com/2023/day/5
//
// Created by Conor Pappas on 12/4/23.
//

#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <regex>

#include "almanac.hpp"
#include "input.hpp"

using namespace std;

// TODO: Move to lib
auto split(const string& str, char delim) {
    return str
        | views::split(delim)
        | views::transform([](auto&& rng) {
            return string_view(rng);
        });
}

const regex SEED_FORMAT = regex(R"(seeds: (\d+( \d+)*))");

vector<Descriptor> parse_seeds(const string& line) {
    vector<Descriptor> descriptors;
    smatch match;
    if (regex_match(line, match, SEED_FORMAT)) {
        const string seeds = match[1].str();
        for (const auto& seed :split(seeds, ' ')) {
            string str(seed.begin(), seed.end());
            descriptors.push_back(Descriptor{"seed", stol(string(seed.begin(), seed.end()))});
        }
    }
    return descriptors;
}

DescriptorSet parse_seeds_set(const string& line) {
    const vector<Descriptor> seeds = parse_seeds(line);
    DescriptorSet descriptor_set("seed");
    for (size_t i = 0; i < seeds.size(); i += 2) {
        const DescriptorRange seed_range = {seeds[i], seeds[i+1].value};
        descriptor_set.insert(seed_range);
    }
    return descriptor_set;
}

const regex DESCRIPTOR_TRANSFORM_HEADER_FORMAT = regex(R"((\w+)-to-(\w+) map:)");
const regex DESCRIPTOR_TRANSFORM_MAP_FORMAT = regex(R"((\d+) (\d+) (\d+))");

DescriptorSetTransform parse_descriptor_set_transform(span<string> lines) {
    string header = lines[0];
    smatch headerMatch;
    if (!regex_search(header, headerMatch, DESCRIPTOR_TRANSFORM_HEADER_FORMAT)) {
        throw runtime_error("Invalid descriptor transform header");
    }
    const Category domain_category = headerMatch[1];
    const Category image_category = headerMatch[2];

    DescriptorSetTransform transform_index(domain_category, image_category);
    for(const string& line : lines.subspan(1)) {
        smatch mapMatch;
        if (regex_search(line, mapMatch, DESCRIPTOR_TRANSFORM_MAP_FORMAT)) {
            const long image_start_value = stol(mapMatch[1]);
            const long domain_start_value = stol(mapMatch[2]);
            const long size = stol(mapMatch[3]);
            const DescriptorRange domain_range = {{domain_category, domain_start_value}, size };
            const DescriptorRange image_range = {{image_category, image_start_value}, size};
            const DescriptorRangeTransform transform = {domain_range, image_range};
            transform_index.add(transform);
        }
    }

    return transform_index;
}

Almanac parse_almanac(const span<string>& lines) {
    Almanac almanac;

    vector<string> mapLines;
    for(const auto& curLine : lines) {
        smatch headerMatch;
        const bool header = regex_search(curLine, headerMatch, DESCRIPTOR_TRANSFORM_HEADER_FORMAT);
        if(header && !mapLines.empty()) {
            const auto transformIndex = parse_descriptor_set_transform(mapLines);
            almanac.transform_sets.insert({transformIndex.domain_category(), transformIndex});
            mapLines.clear();
        }
        if(!curLine.empty()) {
            mapLines.push_back(curLine);
        }
    }
    const auto transformIndex = parse_descriptor_set_transform(mapLines);
    almanac.transform_sets.insert({transformIndex.domain_category(), transformIndex});
    return almanac;
}

long part_1(const vector<Descriptor>& seeds, const Almanac& almanac) {
    set<long> seed_locations;

    for (auto const &seed : seeds) {
        Descriptor seed_result = seed;
        while(seed_result.category != "location") {
            const auto& transform = almanac.transform_sets.at(seed_result.category);
            const auto new_seed_result = transform(seed_result);
            seed_result = new_seed_result;
        }
        seed_locations.insert(seed_result.value);
    }

    return *seed_locations.begin();
}

long part_2(const DescriptorSet& seed_set, const Almanac& almanac) {
    const DescriptorSet locations = almanac(seed_set, "location");
    vector<long> location_starts;
    for (const auto& location_range : locations.ranges()) {
        location_starts.push_back(location_range.start());
    }
    return *ranges::min_element(location_starts);
}

int main(const int argc, const char** argv) {
    vector<string> lines = support::read_input("05", argc, argv);
    const span<string> lines_span = lines;

    const vector<Descriptor> seeds = parse_seeds(lines_span[0]);
    const DescriptorSet seeds_set = parse_seeds_set(lines_span[0]);
    const auto almanac = parse_almanac(lines_span.subspan(1));

    // cout << almanac << endl;
    cout << "Part 1: " << part_1(seeds, almanac) << endl;
    cout << "Part 2: " << part_2(seeds_set, almanac) << endl;
}
