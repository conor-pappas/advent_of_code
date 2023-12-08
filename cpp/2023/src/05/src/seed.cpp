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

class DescriptorRange {
public:
    DescriptorRange(Descriptor range_start, const long size):
        _range_start(std::move(range_start)),
        _size(size) {
        assert(size > 0);
    };

    [[nodiscard]] long start() const { return _range_start.value; }
    [[nodiscard]] long end() const { return _range_start.value + size() - 1; }
    [[nodiscard]] Category category() const { return _range_start.category; }
    [[nodiscard]] bool empty() const { return size() == 0;}
    [[nodiscard]] long size() const { return _size; }

    [[nodiscard]] DescriptorRange intersect(const DescriptorRange& other) const {
        assert(other.category() == category());
        const long range_start = max(start(), other.start());
        const long range_end = min(end(), other.end());
        const long range_size = range_end - range_start + 1;
        return DescriptorRange({category(), range_start}, range_size);
    }

    [[nodiscard]] bool in_range(const Descriptor& input) const {
        assert(input.category == category());
        return input.value >= start() && input.value <= end();
    }

    friend ostream& operator<<(ostream& os, const DescriptorRange& d) {
        return os << d.category() << "(" << d.start() << "-" << d.end() << ")";
    };
private:
    Descriptor _range_start;
    long _size;
};

class DescriptorRangeTransform {
public:
    DescriptorRange domain_range;
    DescriptorRange image_range;

    DescriptorRangeTransform(DescriptorRange domain_range, DescriptorRange image_range):
        domain_range(std::move(domain_range)),
        image_range(std::move(image_range)) {
        assert(domain_range.category() == image_range.category());
        assert(domain_range.size() == image_range.size());
    };

    [[nodiscard]] long offset() const {
        return image_range.start() - domain_range.start();
    }

    Descriptor operator()(const Descriptor& input) const {
        assert(in_range(input));
        return Descriptor{image_range.category(), input.value + offset()};
    }

    DescriptorRange operator()(const DescriptorRange& input) const {
        const auto intersection = domain_range.intersect(input);
        return { {intersection.category(), intersection.start()}, intersection.size() };
    }

    [[nodiscard]] bool in_range(const Descriptor& input) const {
        return domain_range.in_range(input);
    }

    friend ostream& operator<<(ostream& os, const DescriptorRangeTransform& d) {
        return os << d.domain_range << " -> " << d.image_range;
    }
};

class DescriptorSet {
public:
    void insert(const DescriptorRange& other) {
        if(other.empty()) { return; }
        // TODO: assert that other is not a subset of any existing range
        ranges.insert(other);
    }

    void insert(const DescriptorSet& other) {
        ranges.insert(other.ranges.begin(), other.ranges.end());
    }
private:
    struct DescriptorRangeCompare {
        bool operator() (const DescriptorRange& lhs, const DescriptorRange& rhs) const {
            return lhs.end() < rhs.start();
        }
    };
    set<DescriptorRange, DescriptorRangeCompare> ranges;
};

class DescriptorSetTransform {
public:
    void add(const DescriptorRangeTransform& transform) {
        assert(transform.domain_range.category() == _domain_category);
        assert(transform.image_range.category() == _image_category);
        transforms.push_back(transform);
    }

    DescriptorSetTransform(Category domain_category, Category image_category):
        _domain_category(std::move(domain_category)),
        _image_category(std::move(image_category))
    {}

    [[nodiscard]] Category domain_category() const { return _domain_category; }
    [[nodiscard]] Category image_category() const { return _image_category; }

    Descriptor operator()(const Descriptor& input) const {
        auto result = transforms
            | views::filter([&](const auto& transform) { return transform.in_range(input); })
            | views::transform([&](const auto& transform) { return transform(input); });
        if (result.empty()) {
            return {_image_category, input.value};
        } else {
            assert(distance(result.begin(), result.end()) == 1);
            return *result.begin();
        }
    }

    DescriptorSet operator()(const DescriptorRange& input) const {
        DescriptorSet result;
        for (const auto& transform : transforms) {
            result.insert(transform(input));
        }
        return result;
    }
private:
    Category _domain_category;
    Category _image_category;
    vector<DescriptorRangeTransform> transforms{};
};

const regex SEED_FORMAT = regex(R"(seeds: (\d+( \d+)*))");

vector<Descriptor> parseSeeds(const string& line) {
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

const regex DESCRIPTOR_TRANSFORM_HEADER_FORMAT = regex(R"((\w+)-to-(\w+) map:)");
const regex DESCRIPTOR_TRANSFORM_MAP_FORMAT = regex(R"((\d+) (\d+) (\d+))");


DescriptorSetTransform parseDescriptorSetTransform(span<string> lines) {
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

using Almanac = unordered_map<Category, DescriptorSetTransform>;

Almanac parseAlmanac(const span<string>& lines) {
    Almanac almanac;

    vector<string> mapLines;
    for(const auto& curLine : lines) {
        smatch headerMatch;
        const bool header = regex_search(curLine, headerMatch, DESCRIPTOR_TRANSFORM_HEADER_FORMAT);
        if(header && !mapLines.empty()) {
            const auto transformIndex = parseDescriptorSetTransform(mapLines);
            almanac.insert({transformIndex.domain_category(), transformIndex});
            mapLines.clear();
        }
        if(!curLine.empty()) {
            mapLines.push_back(curLine);
        }
    }
    const auto transformIndex = parseDescriptorSetTransform(mapLines);
    almanac.insert({transformIndex.domain_category(), transformIndex});
    return almanac;
}

long part_1(const vector<Descriptor>& seeds, const Almanac& almanac) {
    set<long> seed_locations;

    for (auto const &seed : seeds) {
        Descriptor seed_result = seed;
        while(seed_result.category != "location") {
            const auto& transform = almanac.at(seed_result.category);
            const auto new_seed_result = transform(seed_result);
            seed_result = new_seed_result;
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
